#shader vertex
#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoords;

uniform mat4 u_mvp;
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


float near = 0.1f;
float far = 100.0f;

float LinearizeDepth(float depth)
{
	float z = depth * 2.0 - 1.0; // back to NDC
	return (2.0 * far * near) / (far + near - z * (far - near));    // return Zeye
}

void main()
{
	FragColor = texture(texture0, TexCoords);
	//float gray = (LinearizeDepth(gl_FragCoord.z) - near) / (far - near);  // ÏßÐÔ»Ò¶È
	//FragColor = vec4(vec3(gray), 1.0f);
}