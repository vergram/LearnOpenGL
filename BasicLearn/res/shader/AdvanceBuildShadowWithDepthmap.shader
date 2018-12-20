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

float ShadowCaculation(vec4 fragPosLightSpace)
{
	vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;     // perform perspective divide
	projCoords = projCoords * 0.5f + 0.5f;                             // transform to [0, 1] range

	float closestDepth = texture(shadowMap, projCoords.xy).r;          // uv also in [0, 1] range
	float currentDepth = projCoords.z;                                 // depth in [0, 1] range

	float shadow = currentDepth > closestDepth ? 1.0f : 0.0f;
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
	float shadowValue = ShadowCaculation(fs_in.FragPosLightSpace);

	color *= (ambient + ((1.0 - shadowValue) * (diffuse + specular)));
	
	FragColor = vec4(color, 1.0f);
}