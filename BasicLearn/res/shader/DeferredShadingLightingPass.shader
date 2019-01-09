#shader vertex
#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

void main()
{
	gl_Position = vec4(aPos.x, aPos.y, 0.0f, 1.0f);
	TexCoords = aTexCoords;
}


#shader fragment
#version 330 core

out vec4 FragColor;

in vec2 TexCoords;

struct Light
{
	vec3 Position;
	vec3 Color;

	float Linear;
	float Quadratic;
};
const int NR_LIGHTS = 32;
uniform Light lights[NR_LIGHTS];
uniform vec3 viewPos;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;

void main()
{
	const float gamma = 2.2f;
	vec3 FragPos = texture(gPosition, TexCoords).xyz;
	vec3 Normal = normalize(texture(gNormal, TexCoords).xyz);
	vec3 Albedo = texture(gAlbedoSpec, TexCoords).xyz;
	float SpecularIntensity = texture(gAlbedoSpec, TexCoords).a;

	vec3 lighting = Albedo * 0.1f; // hardcode for ambient
	
	for (int i = 0; i < NR_LIGHTS; i++)
	{
		// diffuse
		vec3 lightDir = normalize(lights[i].Position - FragPos);
		vec3 diffuse = max(dot(lightDir, Normal), 0.0f) * lights[i].Color;
		// specular
		vec3 viewDir = normalize(viewPos - FragPos);
		vec3 halfwayDir = normalize(viewDir + lightDir);
		vec3 specular = pow(max(dot(halfwayDir, Normal), 0.0f), 32.0f) * lights[i].Color * SpecularIntensity;

		float distance = length(lights[i].Position - FragPos);
		float attenuation = 1.0f / (1.0f + lights[i].Linear * distance + lights[i].Quadratic * distance * distance);
		
		vec3 result = specular + diffuse;
		result *= attenuation * Albedo;

		lighting += result;
	}

	// Gamma correction
	//lighting = pow(lighting, vec3(1.0f / gamma));
	
	FragColor = vec4(lighting, 1.0f);

}