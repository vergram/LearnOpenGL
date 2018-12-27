#shader vertex
#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;
layout(location = 3) in vec3 aTangent;

out VS_OUT{
	vec3 FragPos;
	vec2 TexCoords;
	vec3 TangentLightPos;
	vec3 TangentViewPos;
	vec3 TangentFragPos;
}vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 viewPos;
uniform vec3 lightPos;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0f);
	
	mat3 normalMatrix = transpose(inverse(mat3(model)));
	vec3 T = normalize(normalMatrix * aTangent);
	vec3 N = normalize(normalMatrix * aNormal);

	T = normalize(T - dot(T, N) * N);
	vec3 B = cross(N, T);

	mat3 TBN = mat3(T, B, N);
	mat3 inverseTBN = transpose(TBN);
	
	vs_out.FragPos = vec3(model * vec4(aPos, 1.0f));
	vs_out.TexCoords = aTexCoords;
	vs_out.TangentLightPos = inverseTBN * lightPos;
	vs_out.TangentViewPos = inverseTBN * viewPos;
	vs_out.TangentFragPos = inverseTBN * vs_out.FragPos;
}


#shader fragment
#version 330 core
out vec4 FragColor;

in VS_OUT{
	vec3 FragPos;
	vec2 TexCoords;
	vec3 TangentLightPos;
	vec3 TangentViewPos;
	vec3 TangentFragPos;
}fs_in;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;

void main()
{
	vec3 color = texture(texture_diffuse1, fs_in.TexCoords).rgb;
	vec3 normal = texture(texture_normal1, fs_in.TexCoords).rgb;
	normal = normalize(normal * 2.0f - 1.0f);                   // form [0, 1] back to [-1, 1]
	
	vec3 lightColor = vec3(0.5f);
	// ambient
	vec3 ambient = 0.1 * color;
	// diffuse
	vec3 lightDir = normalize(fs_in.TangentLightPos - fs_in.TangentFragPos);
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = diff * lightColor * color;
	// specular
	vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(halfwayDir, normal), 0.0), 64.0);
	vec3 specular = spec * lightColor * texture(texture_specular1, fs_in.TexCoords).rgb;

	//float distance = length(lightPos - fs_in.FragPos);
	//float attenuation = 1 / (distance * distance);

	color = ambient + diffuse + specular;
	
	FragColor = vec4(color, 1.0f);
}