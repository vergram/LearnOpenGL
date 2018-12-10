#shader vertex
#version 330 core

layout(location = 0) in vec2 aPos;
layout(location = 1) in vec3 aColor;

out VS_OUT{
	vec4 gl_Position;
	vec3 vColor;
}vs_out;

layout(std140) uniform Matrices{
	mat4 view;
	mat4 projection;
};

uniform mat4 model;

void main()
{
	gl_Position = vec4(aPos, 0.0f, 1.0f);
	vs_out.gl_Position = gl_Position;
	vs_out.vColor = aColor;
}


#shader fragment
#version 330 core

in vec3 fColor;

out vec4 FragColor;

void main()
{
	FragColor = vec4(fColor, 1.0f);
}


#shader geometry
#version 330 core

layout(points) in;
layout(triangle_strip, max_vertices = 5) out;

in VS_OUT{
	vec4 gl_Position;
	vec3 vColor;
}gl_in[];

out vec3 fColor;

void buildHouse(vec4 position, vec3 color)
{
	fColor = color;
	gl_Position = position + vec4(-0.2, -0.2, 0.0, 0.0);    // 1:左下
	EmitVertex();
	gl_Position = position + vec4(0.2, -0.2, 0.0, 0.0);    // 2:右下
	EmitVertex();
	gl_Position = position + vec4(-0.2, 0.2, 0.0, 0.0);    // 3:左上
	EmitVertex();
	gl_Position = position + vec4(0.2, 0.2, 0.0, 0.0);    // 4:右上
	EmitVertex();
	gl_Position = position + vec4(0.0, 0.4, 0.0, 0.0);    // 5:顶部
	fColor = vec3(1.0, 1.0, 1.0);
	EmitVertex();
	EndPrimitive();
}

void main()
{
	buildHouse(gl_in[0].gl_Position, gl_in[0].vColor);
}
 