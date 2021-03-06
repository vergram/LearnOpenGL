#shader vertex
#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 TexCoords;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0f);
	TexCoords = aTexCoords;
}


#shader fragment
#version 330 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture0;

void main()
{
	vec4 texColor = texture(texture0, TexCoords);
	//if (texColor.a < 0.1f)      // when enable blend, we dont need discard fragment anymore
	//	discard;
	FragColor = texColor;
}