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

const float offset = 1.0 / 300.0;

void main()
{
	// Inversion
	//FragColor = vec4(vec3(1.0 - texture(texture0, TexCoords)), 1.0);

	// grayscale
	//FragColor = texture(texture0, TexCoords);
	//float average = (FragColor.r + FragColor.g + FragColor.b) / 3.0;
	//FragColor = vec4(average, average, average, 1.0);

	vec2 offsets[9] = vec2[](
		vec2(-offset,  offset),  // top left
		vec2(   0.0f,  offset),  // top mid
		vec2( offset,  offset),  // top right
		vec2(-offset,    0.0f),	 // mid left
		vec2(   0.0f,    0.0f),	 // mid mid
		vec2( offset,    0.0f),	 // mid right
		vec2(-offset, -offset),	 // bottom left
		vec2(   0.0f, -offset),	 // bottom mid
		vec2( offset, -offset)	 // bottom right
		);

	float kernel[9] = float[](
		-1, -1, -1,
		-1,  9, -1,
		-1, -1, -1
		);

	vec3 texSample[9];
	for (int i = 0; i < 9; i++)
	{
		texSample[i] = vec3(texture(texture0, TexCoords.st + offsets[i]));
	}

	vec3 sum = vec3(0.0f);
	for (int i = 0; i < 9; i++)
	{
		sum += texSample[i] * kernel[i];
	}

	FragColor = vec4(sum, 1.0f);
}