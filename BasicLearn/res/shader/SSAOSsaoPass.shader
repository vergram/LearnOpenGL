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

out float FragColor;

in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D texNoise;

uniform vec3 samples[64];                // all samples are in tangent space 
uniform mat4 projection;

// variables to tweak the ssao effect
const float radius = 0.5f;                
const float bias = 0.025f;
const int kernalSize = 64;

const vec2 noiseTexScale = vec2(800.0f / 4.0f, 600.0f / 4.0f);

void main()
{
	vec3 fragPos    = texture(gPosition, TexCoords).xyz;
	vec3 normal     = texture(gNormal  , TexCoords).xyz;
	vec3 randomVec  = texture(texNoise , TexCoords * noiseTexScale).xyz;
				    
	vec3 tangent    = normalize(randomVec - normal * dot(normal, randomVec));
	vec3 bitangent  = cross(normal, tangent);
	mat3 TBN        = mat3(tangent, bitangent, normal);

	float occlusion = 0.0f;
	for (int i = 0; i < kernalSize; i++)
	{
		vec3 sample       = TBN * samples[i];                                             // get tangent space sample offset to view space
		sample            = sample * radius + fragPos;                                    // get the sample position
					      								                    	          
		vec4 offset       = vec4(sample, 1.0f);			                    	          
		offset            = projection * offset;                                          // transform to clip spce
		offset.xy        /= offset.w;                                                     // transform to NDC [-1, 1]
		offset.xy         = offset.xy * 0.5f + 0.5f;                                      // transform to screen space [0, 1]
																				          
		float sampleDepth = texture(gPosition, offset.xy).z;                              // get the depth form gBuffer at that sample position

		float rangeCheck  = smoothstep(0.0f, 1.0f, radius / abs(fragPos.z - sampleDepth));// smooth the rangecheck for a smooth effect


		// NOTE: 
		// here we use `>=` instead of `<=` is because we are in the view space,
		// everything in our scene have a negetive `z` component, or we can't see it at all.
		// remember in view matrix we never actually stand on the camera, but make the world change to suit us.
		// so we are in that origional point (0.0f, 0.0f, 0.0f) and the world is behind us.
		occlusion        += (sampleDepth >= (sample.z + bias) ? 1.0f : 0.0f) * rangeCheck;// accumulate the occusion
	}

	// subtract the occlusion factor from 1.0 
	// so we can directly use the occlusion factor to scale the ambient lighting component.
	occlusion = 1.0f - (occlusion / float(kernalSize));

	FragColor = occlusion;
}