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
	const float gamma = 2.2f;
	vec3 hdrColor = texture(diffuseTexture, TexCoords).rgb;

	// Reinhard tone mapping
	//vec3 mapped = hdrColor / (hdrColor + vec3(1.0f));

	// Exposure tone mapping
	vec3 mapped = vec3(1.0f) - exp(-hdrColor * exposure);

	// Gamma correction
	mapped = pow(mapped, vec3(1.0f / gamma));
	
	FragColor = vec4(mapped, 1.0f);

}