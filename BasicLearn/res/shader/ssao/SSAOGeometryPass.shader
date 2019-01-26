#shader vertex
#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

out VS_OUT{
	vec3 Position;
	vec3 Normal;
	vec2 TexCoords;
}vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform bool inverseNormal;

void main()
{
	// Since SSAO is a screen - space technique where occlusion is calculated 
	// based on the visible view it makes sense to implement the algorithm in view - space.
	mat4 viewModel = view * model;
	gl_Position = projection * viewModel * vec4(aPos, 1.0f);

	mat3 normalMatrix = transpose(inverse(mat3(viewModel)));
	vs_out.Normal    = normalize(normalMatrix * (inverseNormal ? -aNormal : aNormal));

	vs_out.Position  = vec3(viewModel * vec4(aPos, 1.0f));
	vs_out.TexCoords = aTexCoords;
}


#shader fragment
#version 330 core
layout(location = 0) out vec3 gPosition;
layout(location = 1) out vec3 gNormal;
layout(location = 2) out vec3 gAlbedoSpec;

in VS_OUT{
	vec3 Position;
	vec3 Normal;
	vec2 TexCoords;
}fs_in;

void main()
{	
	gPosition        = fs_in.Position;
	gNormal          = fs_in.Normal;
	gAlbedoSpec      = vec3(0.95f);
}