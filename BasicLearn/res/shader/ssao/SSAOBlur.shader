#shader vertex
#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

void main()
{
	gl_Position = vec4(aPos.x, aPos.y, 0.0f, 1.0f);
	TexCoords = aTexCoords;
}


#shader fragment
#version 330 core

out float FragColor;

in vec2 TexCoords;

uniform sampler2D ssaoInput;

void main()
{
	vec2 texelSize = 1.0f / vec2(textureSize(ssaoInput, 0));
	float result = 0.0f;
	
	// here we do a simple blur take advantage form noise rotation is a 4 * 4 texture
	// so we just sample offset in [-2, 2)
	for (int x = -2; x < 2; x++)
	{
		for (int y = -2; y < 2; y++)
		{
			vec2 offset = vec2(float(x), float(y)) * texelSize;
			result += texture(ssaoInput, offset + TexCoords).x;
		}
	}
	result /= 16.0f;
	
	FragColor = result;

}