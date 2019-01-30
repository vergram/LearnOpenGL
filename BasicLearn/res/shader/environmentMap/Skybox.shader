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
	FragColor = vec4(texture(skybox, TexCoords).rgb, 1.0);
}