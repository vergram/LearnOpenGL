#shader vertex
#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

out VS_OUT{
	vec3 Normal;
	vec3 FragPos;
	vec2 TexCoords;
}vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform bool reverse_normals;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0f);
	if (reverse_normals) 
		vs_out.Normal = transpose(inverse(mat3(model))) * (-1.0 * aNormal);
	else
		vs_out.Normal = transpose(inverse(mat3(model))) * aNormal;
	vs_out.FragPos = vec3(model * vec4(aPos, 1.0f));
	vs_out.TexCoords = aTexCoords;
}


#shader fragment
#version 330 core
out vec4 FragColor;

in VS_OUT{
	vec3 Normal;
	vec3 FragPos;
	vec2 TexCoords;
}fs_in;

uniform sampler2D diffuseTexture;
uniform samplerCube shadowMap;
uniform vec3 viewPos;
uniform vec3 lightPos;
uniform float far_plane;

// array of offset sampling
vec3 sampleOffsetDirections[20] = vec3[]
(
	vec3(1, 1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1, 1,  1),
	vec3(1, 1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
	vec3(1, 1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1, 1,  0),
	vec3(1, 0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1, 0, -1),
	vec3(0, 1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0, 1, -1)
	);

float ShadowCaculation(vec3 fragPos)
{
	vec3 lightToFragPos = fragPos - lightPos;
	
	float currentDepth = length(lightToFragPos);

	// original shadow without soft
	//float closestDepth = texture(shadowMap, lightToFragPos).r;     // in range of [0, 1]
	//closestDepth *= far_plane;                                     // back to [0, farplane]
	//float shadow = (currentDepth - bais) > closestDepth ? 1.0f : 0.0f;

	// traditional pcf way to soft shadow, but it samples 64 point which its too much for our scene
	//float bais = 0.05f;
	//float shadow = 0.0f;
	//float samples = 4.0f;
	//float offset = 0.1f;
	//for (float x = -offset; x < offset; x += offset / (samples * 0.5f))
	//{
	//	for (float y = -offset; y < offset; y += offset / (samples * 0.5f))
	//	{
	//		for (float z = -offset; z < offset; z += offset / (samples * 0.5f))
	//		{
	//			float pcfDepth = texture(shadowMap, lightToFragPos + vec3(x, y, z)).r;
	//			pcfDepth *= far_plane;
	//			shadow += currentDepth - bais > pcfDepth ? 1.0f : 0.0f;
	//		}
	//	}
	//}
	//shadow /= samples * samples * samples;               // average

	// only sample in perpendicular directions of the sample direction vector
	// which can reduce our samples and take a good result
	float bais = 0.05f;
	float shadow = 0.0f;
	int samples = 20;

	// change the diskRadius based on how far the viewer is away from a fragment
	// making the shadows softer when far away and sharper when close by.
	float viewDistance = length(viewPos - fragPos);
	float diskRadius = (1.0 + viewDistance / far_plane) / 20.0f;
	//float diskRadius = 0.05;
	for (int i = 0; i < samples; i++)
	{
		float pcfDepth = texture(shadowMap, lightToFragPos + sampleOffsetDirections[i] * diskRadius).r;
		pcfDepth *= far_plane;
		shadow += currentDepth - bais > pcfDepth ? 1.0f : 0.0f;
	}
	shadow /= float(samples);

	// for debug
	//FragColor = vec4(vec3(closestDepth / far_plane), 1.0f);

	return shadow;
}

void main()
{
	vec3 color = texture(diffuseTexture, fs_in.TexCoords).rgb;
	vec3 normal = normalize(fs_in.Normal);
	vec3 lightColor = vec3(0.3f);
	
	// ambient
	vec3 ambient = 0.3 * color;
	// diffuse
	vec3 lightDir = normalize(lightPos - fs_in.FragPos);
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;
	// specular
	vec3 viewDir = normalize(viewPos - fs_in.FragPos);
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(halfwayDir, normal), 0.0), 64.0);
	vec3 specular = spec * lightColor;

	float distance = length(lightPos - fs_in.FragPos);
	float attenuation = 1 / (distance * distance);
	float shadowValue = ShadowCaculation(fs_in.FragPos);

	color *= (ambient + ((1.0 - shadowValue) * (diffuse + specular)));
	
	FragColor = vec4(color, 1.0f);
}