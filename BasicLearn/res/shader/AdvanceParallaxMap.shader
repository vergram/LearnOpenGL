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

	mat3 normalMatrix = mat3(transpose(inverse(model)));
	vec3 T = normalize(normalMatrix * aTangent);
	vec3 N = normalize(normalMatrix * aNormal);

	T = normalize(T - dot(T, N) * N);
	vec3 B = cross(N, T);

	mat3 inverseTBN = transpose(mat3(T, B, N));

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
uniform sampler2D texture_normal1;
uniform sampler2D texture_depth1;

uniform float parallax_scale;
uniform bool self_shadowing;

// -----------------------------------------------------------
// Parallax Mapping is relate to two things:
// 1. viewDir (in tangent space)
// 2. height of the real geometry
// everthing about parallax mapping is base on this two things
// ------------------------------------------------------------
vec2 ParallaxMapping(vec2 texCoords, vec3 viewDir)
{
	float height = texture(texture_depth1, texCoords).x;
	vec2 p = viewDir.xy / viewDir.z * height * parallax_scale;     // 除以 z 是因为观察角度和平面越陡峭，offset 就越小
	return texCoords - p;                                        // 相减而不是相加是因为 viewDir 取 FragPos 为原点
}

// ----------------------------------------------- SteepParallaxMapping -------------------------------------------------------------
// The main idea of this method is to divide depth of the surface into number of layers of same height.
// Then starting from the topmost layer you have to sample the heightmap, each time shifting texture coordinates along view vector V.
// If point is under the surface(depth of the current layer is greater than depth sampled from texture), 
// stop the checks and use last used texture coordinates as result of Steep Parallax Mapping.
// --------- Parallax Occlusion Mapping ------------
// base on SteepParallaxMapping add linearly interpolate between the depth layer after and before the collision
// ----------------------------------------------------------------------------------------------------------------------------------
vec2 SteepParallaxMapping(vec2 texCoords, vec3 viewDir)
{
	const float minLayer = 8.0;
	const float maxLayer = 32.0;
	float layerNums = mix(maxLayer, minLayer, abs(dot(vec3(0.0, 0.0, 1.0), viewDir)));

	vec2 p = viewDir.xy / viewDir.z * parallax_scale;          // do not need height value, because we gonna try step by step to accurate the offset 

	// get delta value
	vec2 deltaTexCoords = p / layerNums;
	float layerDepth = 1.0 / layerNums;

	// get inital value
	float currentLayerDepth = 0.0;
	vec2 currentTexCoords = texCoords;
	float currentDepthMapValue = texture(texture_depth1, texCoords).x;

	while (currentLayerDepth < currentDepthMapValue)
	{
		currentTexCoords -= deltaTexCoords;
		currentLayerDepth += layerDepth;
		currentDepthMapValue = texture(texture_depth1, currentTexCoords).x;
	}
	
	// Parallax Occlusion Mapping part
	vec2 preTexCoords = currentTexCoords + deltaTexCoords;

	float afterDepth = currentLayerDepth - currentDepthMapValue;
	float beforeDepth = texture(texture_depth1, preTexCoords).x - currentLayerDepth + layerDepth;

	float weight = afterDepth / (afterDepth + beforeDepth);
	vec2 finalTexCoords = preTexCoords * weight + currentTexCoords * (1.0 - weight);

	return finalTexCoords;
}

// -------------------------------------------- self-shadowing ---------------------------------------------------------------
// Soft shadows take into account multiple values along light vector L.
// Only points under the surface are taken into consideration.
// Partial shadowing factor is calculated as difference between depth of current layer and depth from texture.
// You also have to take into account distance from the point to the fragment.
// So partial factor is multiplied by (1.0 - stepIndex/numberOfSteps).
// To calculate final shadowing factor you have to select one of the calculated partial shadow factors that has maximum value.
// [reference](http://sunandblackcat.com/tipFullView.php?topicid=28)
// ---------------------------------------------------------------------------------------------------------------------------
float parallaxSoftShadowMultiplier(vec3 lightDir, vec2 texCoords)
{
	float shadowMultiplier = 1.0;

	const float minLayers = 15;
	const float maxLayers = 30;

	// calculate lighting only for surface oriented to the light source
	if (dot(vec3(0.0, 0.0, 1.0), lightDir) > 0.0)
	{
		// set init value
		shadowMultiplier = 0.0;
		int numSamplesUnderSurface = 0;
		float numLayers = mix(maxLayers, minLayers, abs(dot(vec3(0.0, 0.0, 1.0), lightDir)));
		float originHeight = texture(texture_depth1, texCoords).x;

		// set delta value
		float deltaLayerHeight = originHeight / numLayers;
		vec2 deltaTexCoords = parallax_scale * lightDir.xy / lightDir.z / numLayers;

		vec2 currentTexCoords = texCoords + deltaTexCoords;
		float heightFormTexture = texture(texture_depth1, currentTexCoords).x;
		float currentLayerHeight = originHeight - deltaLayerHeight;
		int stepIndex = 1;

		while (currentLayerHeight > 0)
		{
			// point is under suface which mean it is in shadow
			if (heightFormTexture < currentLayerHeight)
			{
				numSamplesUnderSurface++;
				// 阴影系数和高度差成正比和距离片元的位置成反比
				float newShadowMultiplier = (currentLayerHeight - heightFormTexture) * (1.0 - stepIndex / numLayers);
				shadowMultiplier = max(newShadowMultiplier, shadowMultiplier);
			}

			// update value
			stepIndex++;
			currentLayerHeight -= deltaLayerHeight;
			currentTexCoords += deltaTexCoords;
			heightFormTexture = texture(texture_depth1, currentTexCoords).x;
		}

		if (numSamplesUnderSurface < 1)
		{
			shadowMultiplier = 1.0;
		}
		else // 注意这里上面计算的是阴影的系数，我们要转化为光照的系数
		{
			shadowMultiplier = 1.0 - shadowMultiplier;
		}

	}
	return shadowMultiplier;
}


void main()
{

	vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
	vec2 texCoords = SteepParallaxMapping(fs_in.TexCoords, viewDir);

	// texture coordinates could oversample outside the range [0, 1] and this gives unrealistic results based on the texture's wrapping mode(s). 
	// A cool trick to solve this issue is to discard the fragment whenever it samples outside the default texture coordinate range
	if (texCoords.x < 0.0 || texCoords.y < 0.0 || texCoords.x > 1.0 || texCoords.y > 1.0)
		discard;

	vec3 lightDir = normalize(fs_in.TangentLightPos - fs_in.TangentFragPos);
	float shadowFactor = parallaxSoftShadowMultiplier(lightDir, texCoords);

	vec3 color = texture(texture_diffuse1, texCoords).rgb;
	vec3 normal = texture(texture_normal1, texCoords).rgb;
	normal = normalize(normal * 2.0f - 1.0f);                   // form [0, 1] back to [-1, 1]

	vec3 lightColor = vec3(0.8f);
	// ambient
	vec3 ambient = 0.1 * color;
	// diffuse
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = diff * lightColor * color;
	// specular
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(halfwayDir, normal), 0.0), 32.0);
	vec3 specular = spec * lightColor * color;

	//float distance = length(lightPos - fs_in.FragPos);
	//float attenuation = 1 / (distance * distance);

	color = self_shadowing ? ambient + shadowFactor * (diffuse + specular) : (ambient + diffuse + specular);

	FragColor = vec4(color, 1.0f);
}