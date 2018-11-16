#shader vertex
#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoords;

uniform mat4 u_ViewProjection;
uniform mat4 u_Model;

void main()
{
	gl_Position = u_ViewProjection * u_Model * vec4(aPos, 1.0f);
	Normal = aNormal;
	FragPos = vec3(u_Model * vec4(aPos, 1.0f));
	TexCoords = aTexCoords;
}


#shader fragment
#version 330 core

struct Light
{
	// use for point light
	vec3 position;

	// use for direction light
	//vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	// use for point light Attenuation
	float constant;
	float liner;
	float quadratic;
};

struct Material
{
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

out vec4 FragColor;

uniform Material material;
uniform Light light;
uniform vec3 viewPos;
uniform vec3 lightColor;
uniform sampler2D emission;

void main()
{
	// ˥����ʽ
	float distance = length(light.position - FragPos);
	float attenuation = 1 / (light.constant + (light.liner * distance) + (light.quadratic * distance * distance));

	// ������
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

	// ������
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.position - FragPos);     // point light
	//vec3 lightDir = normalize(-light.direction);             // direction light
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = attenuation * light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

	// ���淴��
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, Normal);
	float spec = pow(max(dot(reflectDir, viewDir), 0.0f), material.shininess);
	vec3 specular = attenuation * light.specular * spec * vec3(texture(material.specular, TexCoords));

	// ������ɫ
	vec3 emit = vec3(texture(emission, TexCoords));

	vec3 result = ambient + diffuse + specular;
	FragColor = vec4(result, 1.0f);
}