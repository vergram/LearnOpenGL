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
	vec2 texCoords;
}vs_out;

out vec3 Normal;
out vec3 Position;
//out vec2 TexCoords;

void main()
{
	Normal = mat3(transpose(inverse(model))) * aNormal;
	Position = vec3(model * vec4(aPos, 1.0f));
	gl_Position = projection * view * model * vec4(aPos, 1.0f);
	//TexCoords = aTexCoords;
	vs_out.gl_Position = gl_Position;
	vs_out.texCoords = aTexCoords;
}


#shader fragment
#version 330 core

in vec2 TexCoords;

out vec4 FragColor;

uniform sampler2D texture_diffuse1;

void main()
{
	FragColor = texture(texture_diffuse1, TexCoords);
}


#shader geometry
#version 330 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in VS_OUT{
	vec4 gl_Position;
	vec2 texCoords; 
}gl_in[];

out vec2 TexCoords;

uniform float time;

vec3 GetNormal()
{
	vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
	vec3 b = vec3(gl_in[1].gl_Position) - vec3(gl_in[2].gl_Position);
	return normalize(cross(a, b));
}

vec4 Explode(vec4 position, vec3 normal)
{
	float magnitude = 2.0f;
	vec3 direction = normal * ((sin(time) + 1.0) / 2.0) * magnitude;
	return position + vec4(direction, 0.0f);
}

void main()
{
	vec3 normal = GetNormal();

	gl_Position = Explode(gl_in[0].gl_Position, normal);
	TexCoords = gl_in[0].texCoords;
	EmitVertex();

	gl_Position = Explode(gl_in[1].gl_Position, normal);
	TexCoords = gl_in[1].texCoords;
	EmitVertex();

	gl_Position = Explode(gl_in[2].gl_Position, normal);
	TexCoords = gl_in[2].texCoords;
	EmitVertex();

	EndPrimitive();
}
