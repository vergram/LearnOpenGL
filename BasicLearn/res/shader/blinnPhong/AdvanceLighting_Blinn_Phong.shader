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

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0f);
	vs_out.Normal = mat3(transpose(inverse(model))) * aNormal;
	vs_out.FragPos = vec3(model * vec4(aPos, 1.0f));
	vs_out.TexCoords = aTexCoords;
}


#shader fragment
#version 330 core

in VS_OUT{
	vec3 Normal;
	vec3 FragPos;
	vec2 TexCoords;
}fs_in;

out vec4 FragColor;

uniform sampler2D plane;

uniform vec3 lightPositions[4];
uniform vec3 lightColors[4];
uniform vec3 viewPos;
uniform bool gamma;
uniform bool blinn;

vec3 BlinnPhong(vec3 normal, vec3 fragPos, vec3 lightPos, vec3 lightColor)
{
	vec3 lightDir = normalize(lightPos - fragPos);
	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 ambient = 0.05 * lightColor;
	
	float distance = length(lightPos - fragPos);
	float attenuation = 1 / (gamma ? (distance * distance) : distance);

	float diff = max(dot(normal, lightDir), 0);
	vec3 diffuse = diff * lightColor;

	float spec = 0.0;
	if (blinn)
	{
		vec3 halfwayDir = normalize(lightDir + viewDir);
		spec = pow(max(dot(halfwayDir, normal), 0), 32.0);
	}
	else
	{
		vec3 reflectDir = normalize(reflect(-lightDir, normal));
		spec = pow(max(dot(reflectDir, viewDir), 0), 8.0);
	}
	vec3 specular = spec * lightColor;

	vec3 color = attenuation * (ambient + diffuse + specular);
	return color;
}

void main()
{
	vec3 color = texture(plane, fs_in.TexCoords).rgb;
	vec3 normal = normalize(fs_in.Normal);
	
	vec3 lighting = vec3(0.0);
	for(int i = 0; i < 4; i++)
	{
		lighting += BlinnPhong(normal, fs_in.FragPos, lightPositions[i], lightColors[i]);
	}
	color *= lighting;
	if (gamma)
	{
		color = pow(color, vec3(1.0 / 2.2));
	}
	FragColor = vec4(color, 1.0f);
}