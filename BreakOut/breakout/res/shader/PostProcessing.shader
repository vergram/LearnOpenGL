#shader vertex
#version 330 core

layout(location = 0) in vec4 vertex;

out vec2 TexCoords;

uniform bool chaos;
uniform bool confuse;
uniform bool shake;
uniform float time;

void main()
{
	gl_Position = vec4(vertex.xy, 0.0, 1.0);

	if (chaos)
	{
		float strength = 0.3;
		TexCoords = vec2(vertex.z + sin(time) * strength, vertex.w + cos(time) * strength);
	}
	else if (confuse)
	{
		TexCoords = vec2(1.0 - vertex.z, 1.0 - vertex.w);
	}
	else
	{
		TexCoords = vertex.zw;
	}

	if (shake)
	{
		float strength = 0.01;
		gl_Position.x += cos(time * 10.0) * strength;
		gl_Position.y += sin(time * 15.0) * strength;
	}
}


#shader fragment
#version 330 core

in vec2 TexCoords;
out vec4 color;

uniform bool chaos;
uniform bool confuse;
uniform bool shake;

uniform sampler2D scene;
uniform vec2      offsets[9];
uniform int       edge_kernel[9];
uniform float     blur_kernel[9];


void main()
{
	color = vec4(0.0, 0.0, 0.0, 1.0);
	vec3 samples[9];
	if (chaos || shake)
	{
		for (int i = 0; i < 9; i++)
		{
			samples[i] = texture(scene, TexCoords + offsets[i]).rgb;
		}
	}

	// process effect
	if (chaos)
	{
		for (int i = 0; i < 9; i++)
		{
			color += vec4(samples[i] * edge_kernel[i], 0.0);
		}
		color.a = 1.0f;
	}
	else if (confuse)
	{
		color = vec4(1.0 - texture(scene, TexCoords).rgb, 1.0);
	}

	else if (shake)
	{
		for (int i = 0; i < 9; i++)
		{
			color += vec4(samples[i] * blur_kernel[i], 0.0);
		}
		color.a = 1.0f;
	}
	else
	{
		color = texture(scene, TexCoords);
	}
}