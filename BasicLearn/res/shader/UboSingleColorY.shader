#shader vertex
#version 330 core

layout(location = 0) in vec3 aPos;

layout(std140) uniform Matrices{
	mat4 view;
	mat4 projection;
};

uniform mat4 model;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0f);
}


#shader fragment
#version 330 core

out vec4 FragColor;

void main()
{
	vec3 red = vec3(1.0, 0.0, 0.0);
	vec3 green = vec3(0.0, 1.0, 0.0);
	vec3 blue = vec3(0.0, 0.0, 1.0);

	vec3 color = (pow(red, vec3(2.2)) + pow(green, vec3(2.2))) / 2;    // 变回物理线性空间插值

	color = pow(color, vec3(1.0 / 2.2));                         // 再伽马校正
	
	FragColor = vec4(color, 1.0f);
}