#shader vertex
#version 330 core

layout(location = 0) in vec3 aPos;

out vec3 LocalPos;

uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * vec4(aPos, 1.0f);
	LocalPos = aPos;
}

// --------------------------------------- image based lighting ------------------------------------------------
// here is the render equation again, and in image based lighting, our goal is find a way slove the integral.
//
//                          ∫        c                DFG
//             Lo(p, ωo) =  ∫ (kd * --- + ks * -------------------) Li(p, ωi) n • ωi dωi
//                          ∫Ω       π           4(ωo • n)(ωi • n)
//
// take a closer look and we can find it is easy divide to 2 part integral, like this:
//
//                      diffuse irradiance                                             specular part
//   ----------------------------------------------------- 	   -----------------------------------------------------------------
//   |                     c    ∫ 					     | 	   |                ∫ 	         DFG	  			               |
//   |   Lo(p, ωo) = kd * --- * ∫  Li(p, ωi) n • ωi dωi  |  +  |   Lo(p, ωo) =  ∫  ks -------------------Li(p, ωi) n • ωi dωi  | 
//   |                     π    ∫Ω       				 | 	   |                ∫Ω     4(ωo • n)(ωi • n)    				   |
//   ----------------------------------------------------- 	   -----------------------------------------------------------------
//
// Epic Games' split sum approximation solves the issue by splitting the pre-computation into 2 individual parts 
// that we can later combine to get the resulting pre-computed result we're after. 
// The split sum approximation splits the specular integral into two separate integrals
//	                         
//   ----------------------------------------------------------------
//   |                ∫  	            ∫                           |
//   |   Lo(p, ωo) =  ∫  Li(p, ωi)dωi * ∫  fr(p, ωi, ωo)n • ωidωi   | 
//   |                ∫Ω    			∫Ω	                        |
//   ----------------------------------------------------------------
//
// The first part (when convoluted) is known as the 'pre-filtered environment map' which is (similar to the irradiance map) 
// a pre-computed environment convolution map, but this time taking roughness into account. For increasing roughness levels, 
// the environment map is convoluted with more scattered sample vectors, creating more blurry reflections. 
// For each roughness level we convolute, we store the sequentially blurrier results in the pre-filtered map's mipmap levels. 
// 
// The second part of the equation equals the BRDF part of the specular integral. 
// If we pretend the incoming radiance is completely white for every direction (thus L(p,x)=1.0) we can pre-calculate
// the BRDF's response given an input roughness and an input angle between the normal n and light direction ωi, or n⋅ωi. 
// Epic Games stores the pre-computed BRDF's response to each normal and light direction combination
// on varying roughness values in a 2D lookup texture (LUT) known as the BRDF integration map. 
//

#shader fragment
#version 330 core

const float PI = 3.14159265359;

// we'll pre-compute the specular portion of the indirect reflectance equation using importance sampling
// given a random low-discrepancy sequence based on the Quasi-Monte Carlo method.
// The sequence we'll be using is known as the Hammersley Sequence as carefully described by [Holger Dammertz](http://holger.dammertz.org/stuff/notes_HammersleyOnHemisphere.html)
// The Hammersley sequence is based on the Van Der Corpus sequence which mirrors a decimal binary representation around its decimal point.
float RadicalInverse_VdC(uint bits)
{
	bits = (bits << 16u) | (bits >> 16u);
	bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
	bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
	bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
	bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
	return float(bits) * 2.3283064365386963e-10; // / 0x100000000
}
// The GLSL Hammersley function gives us the low-discrepancy sample i of the total sample set of size N. [0, 1]
vec2 Hammersley(uint i, uint N)
{
	return vec2(float(i) / float(N), RadicalInverse_VdC(i));
}

// to build a sample vector, we need some way of orienting and biasing the sample vector towards the specular lobe of some surface roughness.
vec3 ImportanceSampleGGX(vec2 Xi, vec3 N, float roughness)
{
	float a = roughness * roughness;

	float phi = 2.0 * PI * Xi.x;
	float cosTheta = sqrt((1.0 - Xi.y) / (1.0 + (a*a - 1.0) * Xi.y));        // basically here is based on the soomthness make the sample vector more accurate to the reflection 
	float sinTheta = sqrt(1.0 - cosTheta * cosTheta);

	vec3 H;
	H.x = sinTheta * cos(phi);
	H.y = sinTheta * sin(phi);
	H.z = cosTheta;

	vec3 up        = abs(N.z) < 0.999 ? vec3(0.0, 0.0, 1.0) : vec3(1.0, 0.0, 0.0); 
	vec3 tangent   = normalize(cross(up, N));
	vec3 bitangent = cross(N, tangent);

	// tangent space to world space(think of the TBN)
	vec3 sampleVec = tangent * H.x + bitangent * H.y + N * H.z;
	return normalize(sampleVec);
}

out vec3 FragColor;

in vec3 LocalPos;

uniform samplerCube environmentMap;
uniform float roughness;

void main()
{
	// from Epic Game appoximation: N = R = V
	vec3 N = normalize(LocalPos);
	vec3 R = N;
	vec3 V = R;

	vec3  prefilter   = vec3(0.0);
	float totalWeight = 0.0;

	const uint SAMPLE_COUNT = 4096u;
	for (uint i = 0u; i < SAMPLE_COUNT; i++)
	{
		vec2 Xi = Hammersley(i, SAMPLE_COUNT);
		vec3 H  = ImportanceSampleGGX(Xi, N, roughness);
		vec3 L  = normalize(2.0 * dot(V, H) * H - V);              // in specular part we need reflect vector as our incident light

		float NdotL = max(dot(N, L), 0.0);
		if (NdotL > 0.0)
		{
			prefilter   += texture(environmentMap, L).rgb * NdotL;
			totalWeight += NdotL;
		}
	}

	prefilter = prefilter / totalWeight;
	FragColor = prefilter;
}
