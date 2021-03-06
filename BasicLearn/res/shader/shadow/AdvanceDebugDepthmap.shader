#shader vertex
#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

void main()
{
	gl_Position = vec4(aPos, 1.0f);
	TexCoords = aTexCoords;
}


#shader fragment
#version 330 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D depthMap;
uniform float near_plane;
uniform float far_plane;

float LinearizeDepth(float depth)
{
	float z = depth * 2.0 - 1.0; // back to NDC
	return (2.0 * far_plane * near_plane) / (far_plane + near_plane - z * (far_plane - near_plane));    // return Zeye
}

void main()
{
	float depthValue = texture(depthMap, TexCoords).r;
	//float gray = (LinearizeDepth(gl_FragCoord.z) - near) / (far - near);  // ���ԻҶ�

	FragColor = vec4(vec3(depthValue), 1.0f);
}