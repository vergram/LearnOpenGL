#shader vertex
#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out VS_OUT{
	vec4 gl_Position;
	vec3 normal;
}vs_out;

void main()
{
	vec3 Normal = mat3(transpose(inverse(view * model))) * aNormal;
	gl_Position = projection * view * model * vec4(aPos, 1.0f);
	vs_out.gl_Position = gl_Position;
	vs_out.normal = normalize(vec3(projection * vec4(Normal, 0.0f)));
}


#shader fragment
#version 330 core

out vec4 FragColor;

void main()
{
	FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);
}


#shader geometry
#version 330 core

layout(triangles) in;
layout(line_strip, max_vertices = 6) out;

in VS_OUT{
	vec4 gl_Position;
	vec3 normal; 
}gl_in[];

void GenerateLine(int index)
{
	float magnitude = 0.2;
	gl_Position = gl_in[index].gl_Position;
	EmitVertex();
 
	gl_Position = gl_in[index].gl_Position + vec4(gl_in[index].normal * magnitude, 0.0f) * magnitude;
	EmitVertex();
	EndPrimitive();
}

void main()
{
	GenerateLine(0);
	GenerateLine(1);
	GenerateLine(2);
}
