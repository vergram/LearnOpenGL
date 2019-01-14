#shader vertex
#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

out VS_OUT{
	vec3 Normal;
	vec3 FragPos;
	vec2 TexCoords;
}vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform bool reverse_normals;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0f);
	if (reverse_normals) 
		vs_out.Normal = transpose(inverse(mat3(model))) * (-1.0f * aNormal);
	else
		vs_out.Normal = transpose(inverse(mat3(model))) * aNormal;
	vs_out.FragPos = vec3(model * vec4(aPos, 1.0f));
	vs_out.TexCoords = aTexCoords;
}


#shader fragment
#version 330 core
layout(location = 0) out vec4 FragColor;
layout(location = 1) out vec4 BrightColor;


in VS_OUT{
	vec3 Normal;
	vec3 FragPos;
	vec2 TexCoords;
}fs_in;

struct Light
{
	vec3 Position;
	vec3 Color;
};

uniform Light lights[4];
uniform sampler2D diffuseTexture;
uniform vec3 viewPos;

void main()
{
	vec3 color = texture(diffuseTexture, fs_in.TexCoords).rgb;
	vec3 normal = normalize(fs_in.Normal);
	// ambient
	vec3 ambient = 0.05 * color;
	// light
	vec3 lighting = vec3(0.0f);
	for (int i = 0; i < 4; i++)
	{
		// diffuse
		vec3 lightDir = normalize(lights[i].Position - fs_in.FragPos);
		float diff = max(dot(normal, lightDir), 0.0);
		vec3 diffuse = diff * lights[i].Color;
		// specular
		vec3 viewDir = normalize(viewPos - fs_in.FragPos);
		vec3 halfwayDir = normalize(lightDir + viewDir);
		float spec = pow(max(dot(halfwayDir, normal), 0.0), 64.0);
		vec3 specular = spec * lights[i].Color;

		float distance = length(lights[i].Position - fs_in.FragPos);
		float attenuation = 1 / (distance * distance);
		
		vec3 result = (diffuse + specular) * attenuation;
		lighting += result;
	}

	color *= (ambient + lighting);
	
	FragColor = vec4(color, 1.0f);

	// check whether fragment output greater than threshold, if so output as bright color
	float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
	if (brightness > 1.0f)
		BrightColor = vec4(FragColor.rgb, 1.0f);
	// it is necessary for this else statement, since NV card automatically use the first buffer value to initlize the other.
	// for this reason, without this else statement, no matter the `brightness` pass the if statement or not the BrightColor would qeuals to that FragColor.
	else
		BrightColor = vec4(0.0f, 0.0f, 0.0f, 1.0f); // since we use additive blend finally, we can set the pixel pure black for make no effort to blend.
}