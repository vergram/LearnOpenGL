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
	Normal = mat3(transpose(inverse(u_Model))) * aNormal;
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
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float cutOff;
	float outerCutOff;

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
	vec3 lightDir = normalize(light.position - FragPos);     // point light
	//vec3 lightDir = normalize(-light.direction);             // direction light
	vec3 viewDir = normalize(viewPos - FragPos);

	// 聚光灯内外圆锥因子
	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.cutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0f, 1.0f);
	
	// 点光源衰减因子
	float distance = length(light.position - FragPos);
	float attenuation = 1 / (light.constant + (light.liner * distance) + (light.quadratic * distance * distance));

	// 环境光
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

	// 漫反射
	vec3 norm = normalize(Normal);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = attenuation * light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

	// 镜面反射
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(reflectDir, viewDir), 0.0f), material.shininess);
	vec3 specular = attenuation * light.specular * spec * vec3(texture(material.specular, TexCoords));

	diffuse = intensity * diffuse;
	specular = intensity * specular;


	FragColor = vec4((ambient + diffuse + specular), 1.0f);

	// 发光颜色
	//vec3 emit = vec3(texture(emission, TexCoords));
}