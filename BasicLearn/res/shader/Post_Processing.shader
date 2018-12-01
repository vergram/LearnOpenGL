#shader vertex
#version 330 core

layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

void main()
{
	gl_Position = vec4(aPos.x, aPos.y, 0.0f, 1.0f);
	TexCoords = aTexCoords;
}


#shader fragment
#version 330 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture0;

void main()
{
	// Inversion
	//FragColor = vec4(vec3(1.0 - texture(texture0, TexCoords)), 1.0);

	// grayscale
	FragColor = texture(texture0, TexCoords);
	float average = (FragColor.r + FragColor.g + FragColor.b) / 3.0;
	FragColor = vec4(average, average, average, 1.0);
}