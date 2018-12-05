#shader vertex
#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 Normal;
out vec3 Position;
out vec2 TexCoords;

void main()
{
	Normal = mat3(transpose(inverse(model))) * aNormal;
	Position = vec3(model * vec4(aPos, 1.0f));
	gl_Position = projection * view * model * vec4(aPos, 1.0f);
	TexCoords = aTexCoords;
}


#shader fragment
#version 330 core

out vec4 FragColor;

in vec3 Normal;
in vec3 Position;
in vec2 TexCoords;

uniform vec3 cameraPos;
uniform samplerCube envmap;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_reflection1;

void main()
{
	// diffuse
	vec4 diffuse = texture(texture_diffuse1, TexCoords);
	
	// reflect
	vec3 I = normalize(Position - cameraPos);
	vec3 R = reflect(I, normalize(Normal));   
	float reflect_intensity = texture(texture_reflection1, TexCoords).x;
	vec4 reflect_color;
	if (reflect_intensity > 0.1)
	{
		reflect_color = texture(envmap, R) * reflect_intensity;
	}

	FragColor = reflect_color + diffuse;

}