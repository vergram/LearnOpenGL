#shader vertex
#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

out VS_OUT{
	vec3 Normal;
	vec3 FragPos;
	vec2 TexCoords;
	vec4 FragPosLightSpace;
}vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpaceMatrix;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0f);
	vs_out.Normal = mat3(transpose(inverse(model))) * aNormal;
	vs_out.FragPos = vec3(model * vec4(aPos, 1.0f));
	vs_out.TexCoords = aTexCoords;
	vs_out.FragPosLightSpace = lightSpaceMatrix * vec4(vs_out.FragPos, 1.0f);
}


#shader fragment
#version 330 core

in VS_OUT{
	vec3 Normal;
	vec3 FragPos;
	vec2 TexCoords;
	vec4 FragPosLightSpace;
}fs_in;

out vec4 FragColor;

uniform sampler2D shadowMap;
uniform sampler2D diffuseTexture;
uniform vec3 viewPos;
uniform vec3 lightPos;

float ShadowCaculation(vec4 fragPosLightSpace, vec3 lightDir, vec3 normal)
{
	vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;     // perform perspective divide
	projCoords = projCoords * 0.5f + 0.5f;                             // transform to [0, 1] range

	if (projCoords.z > 1.0f)                                           // things do not in the projection Matrix we set it shadow eaqual 0
	{
		return 0.0f;
	}

	float closestDepth = texture(shadowMap, projCoords.xy).r;          // uv also in [0, 1] range
	float currentDepth = projCoords.z;                                 // depth in [0, 1] range

	float bais = max(0.05 * (1 - dot(lightDir, normal)), 0.005);

	//float shadow = (currentDepth - bais) > closestDepth ? 1.0f : 0.0f;

	//  pcf way to soft shadow
	float shadow = 0.0f;
	vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
	for (int x = -1; x <= 1; x++)
	{
		for (int y = -1; y <= 1; y++)
		{
			float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
			shadow += currentDepth - bais > pcfDepth ? 1.0f : 0.0f;
		}
	}
	shadow /= 9.0f;               // average

	return shadow;
}

void main()
{
	vec3 color = texture(diffuseTexture, fs_in.TexCoords).rgb;
	vec3 normal = normalize(fs_in.Normal);
	vec3 lightColor = vec3(1.0f);
	
	// ambient
	vec3 ambient = 0.15 * color;
	// diffuse
	vec3 lightDir = normalize(lightPos - fs_in.FragPos);
	float diff = max(dot(normal, lightDir), 0);
	vec3 diffuse = diff * lightColor;
	// specular
	vec3 viewDir = normalize(viewPos - fs_in.FragPos);
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(halfwayDir, normal), 0), 32.0);
	vec3 specular = spec * lightColor;

	float distance = length(lightPos - fs_in.FragPos);
	float attenuation = 1 / (distance * distance);
	float shadowValue = ShadowCaculation(fs_in.FragPosLightSpace, lightDir, normal);

	color *= (ambient + ((1.0 - shadowValue) * (diffuse + specular)));
	
	FragColor = vec4(color, 1.0f);
}