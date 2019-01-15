#shader vertex
#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0f);
}


#shader fragment
#version 330 core

out vec4 FragColor;

struct Light
{
	vec3 Position;
	vec3 Color;

	float Linear;
	float Quadratic;

	float Radius;
};
uniform Light light;
uniform vec3 viewPos;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;

vec2 CalcTexCoord()
{
	return gl_FragCoord.xy / vec2(800, 600);
}

void main()
{
	vec2 TexCoords = CalcTexCoord();

	vec3 FragPos = texture(gPosition, TexCoords).xyz;
	vec3 Normal = normalize(texture(gNormal, TexCoords).xyz);
	vec3 Albedo = texture(gAlbedoSpec, TexCoords).xyz;
	float SpecularIntensity = texture(gAlbedoSpec, TexCoords).a;

	vec3 lighting = Albedo * 0.1f; // hardcode for ambient
	
	float distance = length(light.Position - FragPos);

	float attenuation = 1.0f / (1.0f + light.Linear * distance + light.Quadratic * distance * distance);

	// diffuse
	vec3 lightDir = normalize(light.Position - FragPos);
	vec3 diffuse = max(dot(lightDir, Normal), 0.0f) * light.Color;
	// specular
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 halfwayDir = normalize(viewDir + lightDir);
	vec3 specular = pow(max(dot(halfwayDir, Normal), 0.0f), 32.0f) * light.Color * SpecularIntensity;

	vec3 result = specular + diffuse;
	result *= attenuation * Albedo;

	lighting += result;
	
	FragColor = vec4(lighting, 1.0f);

}