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

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D diffuseTexture;
uniform float exposure;

void main()
{
	//const float gamma = 2.2f;
	vec3 color = vec3(texture(diffuseTexture, TexCoords).r);

	// Reinhard tone mapping
	//color = color / (color + vec3(1.0f));

	// Exposure tone mapping
	//color = vec3(1.0f) - exp(-color * exposure);

	// Gamma correction
	//color = pow(color, vec3(1.0f / gamma));
	
	FragColor = vec4(color, 1.0f);

}