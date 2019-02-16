#shader vertex
#version 330 core

layout(location = 0) in vec3 aPos;

uniform mat4 view;
uniform mat4 projection;

out vec3 TexCoords;

void main()
{
	TexCoords = aPos;
	mat4 rotView = mat4(mat3(view));
	vec4 pos = projection * rotView * vec4(aPos, 1.0f);
	gl_Position = pos.xyww;
}


#shader fragment
#version 330 core

out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox;

void main()
{
	vec3 color = texture(skybox, TexCoords).rgb;

	color = color / (color + 1.0);
	color = pow(color, vec3(1.0 / 2.2));
	FragColor = vec4(color, 1.0);
}