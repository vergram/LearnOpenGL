#shader vertex
#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;
layout(location = 3) in vec3 aTangent;

out VS_OUT{
	vec3 Position;
	vec3 Normal;
	vec3 Tangent;
	vec3 Bitangent;
	vec2 TexCoords;
}vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0f);

	// here we use another way to use normal map, 
	// we do not tranform light or view position to tangent space,
	// but transform normal in normal map into world space,
	// since we use deferred shading for mutiplie light souces,
	// it is not so bad for transform normal form tangent space to world space.
	mat3 normalMatrix = transpose(inverse(mat3(model)));
	vec3 T = normalize(normalMatrix * aTangent);
	vec3 N = normalize(normalMatrix * aNormal);
	
	T = normalize(T - dot(T, N) * N);
	vec3 B = cross(N, T);

	vs_out.Position = vec3(model * vec4(aPos, 1.0f));
	vs_out.Normal    = N;
	vs_out.Tangent   = T;
	vs_out.Bitangent = B;
	vs_out.TexCoords = aTexCoords;
}


#shader fragment
#version 330 core
layout(location = 0) out vec3 gPosition;
layout(location = 1) out vec3 gNormal;
layout(location = 2) out vec4 gAlbedoSpec;

in VS_OUT{
	vec3 Position;
	vec3 Normal;
	vec3 Tangent;
	vec3 Bitangent;
	vec2 TexCoords;
}fs_in;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;


void main()
{
	vec3 normal = texture(texture_normal1, fs_in.TexCoords).rgb;   // get normal form normal map
	normal = normalize(normal * 2.0f - 1.0f);                      // form [0, 1] back to [-1, 1]

	// tranform the normal form normal map to world space
	mat3 TBN = mat3(fs_in.Tangent, fs_in.Bitangent, fs_in.Normal);
	normal = TBN * normal;
	
	vec3 diffuse = texture(texture_diffuse1, fs_in.TexCoords).rgb;
	float specularIntensity = texture(texture_specular1, fs_in.TexCoords).r;

	gPosition        = fs_in.Position;
	gNormal          = normal;
	gAlbedoSpec      = vec4(diffuse, specularIntensity);
}