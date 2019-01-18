#shader vertex
#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoords;

void main()
{
	gl_Position = vec4(aPos, 1.0f);
}


#shader fragment
#version 330 core

out vec4 FragColor;

struct PointLight
{
	vec3 Color;
	vec3 Position;

	// attenuation
	// float Constant = 1.0f; 
	float Linear;
	float Quadratic;
};

// calculate texCoord for gBuffer
vec2 CalcTexCoord()
{
	return gl_FragCoord.xy / vec2(800, 600);
}

uniform PointLight pointLight;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;
uniform sampler2D ssao;

void main()
{
	vec2  TexCoords         =  CalcTexCoord();
	vec3  FragPos           =  texture(gPosition, TexCoords).xyz;
	vec3  Normal            =  texture(gNormal, TexCoords).xyz;
	vec3  Albedo            =  texture(gAlbedoSpec, TexCoords).rgb;
	float AmbientOcclusion  =  texture(ssao, TexCoords).x;

	vec3 ambient = vec3(0.3f * Albedo * AmbientOcclusion);

	vec3 lighting = ambient;
	vec3 viewDir = normalize(-FragPos); // viewpos is (0.0.0)
	// diffuse
	vec3 lightDir = normalize(pointLight.Position - FragPos);
	vec3 diffuse = max(dot(Normal, lightDir), 0.0) * Albedo * pointLight.Color;
	// specular
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(Normal, halfwayDir), 0.0), 8.0);
	vec3 specular = pointLight.Color * spec;
	// attenuation
	float distance = length(pointLight.Position - FragPos);
	float attenuation = 1.0 / (1.0 + pointLight.Linear * distance + pointLight.Quadratic * distance * distance);
	diffuse *= attenuation;
	specular *= attenuation;
	lighting += diffuse + specular;

	FragColor = vec4(lighting, 1.0f);

}