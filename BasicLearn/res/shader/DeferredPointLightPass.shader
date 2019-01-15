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

struct PointLight
{
	vec3 Color;
	vec3 Position;

	// attenuation
	// float Constant = 1.0f; 
	float Linear;
	float Quadratic;
};

struct DirectionLight
{
	vec3 Color;
	vec3 Direction;
};
uniform PointLight pointLight;
uniform vec3 viewPos;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;

// Blining-Phong lighting model
vec3 CalcLightInternal(vec3 lightColor, 
					   vec3 lightDir, 
	                   vec3 fragPos,
					   vec3 normal, 
					   vec3 viewPos, 
	                   float diffuseIntensity,
	                   float specularIntensity)
{
	vec3 ambient = lightColor * 0.1f; // hardcode for ambient

	vec3 diffuse = max(dot(lightDir, normal), 0.0f) * lightColor * diffuseIntensity;

	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 halfwayDir = normalize(viewDir + lightDir);
	vec3 specular = pow(max(dot(halfwayDir, normal), 0.0f), 32.0f) * lightColor * specularIntensity;

	vec3 lighting = ambient + diffuse + specular;

	return lighting;
}

vec4 CalcPointLight(PointLight pointLight,
	                vec3 fragPos,
	                vec3 normal,
	                vec3 viewPos,
	                float diffuseIntensity,
	                float specularIntensity)
{
	vec3 lightDir = normalize(pointLight.Position - fragPos);
	vec3 lighting = CalcLightInternal(pointLight.Color, lightDir, fragPos, normal, viewPos, diffuseIntensity, specularIntensity);

	float distance = length(pointLight.Position - fragPos);
	float Attenuation = 1.0f + pointLight.Linear * distance + pointLight.Quadratic * distance * distance;

	return vec4(lighting / Attenuation, 1.0f);
}

vec4 CalcDirectionLight(DirectionLight directionLight,
					    vec3 fragPos,
					    vec3 normal,
					    vec3 viewPos,
					    float diffuseIntensity,
					    float specularIntensity)
{
	vec3 lightDir = -directionLight.Direction;
	vec3 lighting = CalcLightInternal(directionLight.Color, lightDir, fragPos, normal, viewPos, diffuseIntensity, specularIntensity);
	return vec4(lighting, 1.0f);
}

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

	vec4 lighting = CalcPointLight(pointLight, FragPos, Normal, viewPos, 1.0f, SpecularIntensity);
	
	vec4 color = vec4(Albedo, 1.0f) * lighting;

	FragColor = color;

}