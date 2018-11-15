#shader vertex
#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;

out vec3 Normal;
out vec3 FragPos;

uniform mat4 u_ViewProjection;
uniform mat4 u_Model;

void main()
{
	gl_Position = u_ViewProjection * u_Model * vec4(aPos, 1.0f);
	Normal = aNormal;
	FragPos = vec3(u_Model * vec4(aPos, 1.0f));
}


#shader fragment
#version 330 core

struct Light
{
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

uniform Material material;
uniform Light light;
uniform vec3 viewPos;
uniform vec3 lightPos;
uniform vec3 lightColor;

void main()
{
	// 环境光
	//float ambientStrength = 0.1f;
	//vec3 ambient = ambientStrength * lightColor;
	vec3 ambient = light.ambient * material.ambient;

	// 漫反射
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.position - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * (diff * material.diffuse);

	// 镜面反射
	//float specularStrength = 0.5f;
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, Normal);
	float spec = pow(max(dot(reflectDir, viewDir), 0.0f), material.shininess);
	vec3 specular = light.specular * (spec * material.specular);


	vec3 result = ambient + diffuse + specular;
	FragColor = vec4(result, 1.0f);
}