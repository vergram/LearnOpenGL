#shader vertex
#version 330 core

layout(location = 0) in vec3 aPos;

uniform mat4 model;

void main()
{
	gl_Position = model * vec4(aPos, 1.0f);
}

#shader geometry
#version 330 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 18) out;

out vec4 FragPos;

uniform mat4 lightSpaceMatrices[6];

void main()
{
	for (int face = 0; face < 6; face++)
	{
		gl_Layer = face;  //  built-in variable called gl_Layer that specifies which cubemap face to emit a primitive to.
		for (int i = 0; i < 3; i++)
		{
			FragPos = gl_in[i].gl_Position;
			gl_Position = lightSpaceMatrices[face] * FragPos;
			EmitVertex();
		}
		EndPrimitive();
	}
}


#shader fragment
#version 330 core

in vec4 FragPos;

uniform vec3 lightPos;
uniform float far_plane;

void main()
{
	float distance = length(FragPos.xyz - lightPos);
	distance = distance / far_plane;
	gl_FragDepth = distance;             // map to [0, 1] range
}