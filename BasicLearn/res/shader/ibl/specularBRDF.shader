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
//   for BRDF integral part:
//
//   ---------------------------------------------------
//   |                     ∫                           |
//   |   Lo(p, φo, θo) =   ∫  fr(p, ωi, ωo)n • ωidωi   |
//   |                     ∫Ω	                       |
//   ---------------------------------------------------
//
// NOTE:The second sum includes everything else. This is the same as integrating the specular BRDF with
// a solid - white environment, i.e.Li(lk) = 1. By substituting in Schlick’s Fresnel approximation—
// F(v, h) = F0 + (1 − F0)(1 − v · h)⁵   we find that F0 can be factored out of the integral :
//
//     ----------------------------------------------------------------------------------------------------
//     |  ∫               	 	∫  f(l, v)								∫  f(l, v)						  |
//     |  ∫ f(l, v)cosθldl = F0 ∫ --------- (1 - (1 − v · h)⁵) cosθldl + ∫ --------- (1 − v · h)⁵ cosθldl |
//     |  ∫H         			∫H F(v, h)								∫H F(v, h)						  |
//     ----------------------------------------------------------------------------------------------------
//
// This leaves two inputs (Roughness and cos θv) and two outputs (a scale and bias to F0), all of which
// are conveniently in the range[0, 1].We precalculate the result of this function and store it in a 2D look-up texture(LUT).
//
// since we are actually using importance sampling, the whole equation can be write as this:
// DETAILS: https://blog.csdn.net/i_dovelemon/article/details/79598921
//
//	                         -----------------------------------------   --------------------------------------
//    ∫               	     | 	 1     N   ([1−(1−v⋅h)⁵])G	  (v⋅h)	 |   |   1     N    (1−v⋅h)⁵G	  (v⋅h)   |
//    ∫ f(l, v)cosθldl ≈  F0 | [--- •  ∑  ----------------- • -----] | + | [--- •  ∑  ------------- • ----- ] |
//    ∫H         		     | 	 N    k=1       (n⋅v)		  (n⋅h)	 |   |   N    k=1     (n⋅v)		  (n⋅h)   |
//	                         -----------------------------------------   --------------------------------------
//        brdf         =  F0 *                  scale                  +                    bias
//  
//



#shader fragment
#version 330 core

const float PI = 3.14159265359;

// The geometry function statistically approximates the relative surface area where its micro surface-details overshadow 
// each other causing light rays to be occluded.
//                                    n⋅v
//       GSchlickGGX(n, v, k) = -----------------
//                                (n⋅v)(1−k)+k
// Here k is a remapping of α based on whether we're using the geometry function for either direct lighting or IBL lighting:
// --------------------------------------------------------------
// |              (α+1)²  	    |		           α²		    |
// |   kdirect = --------		|	   kIBL   = --------	    |
// |                8			|	               2		    |
// --------------------------------------------------------------
// To effectively approximate the geometry we need to take account of both the view direction (geometry obstruction) 
// and the light direction vector (geometry shadowing). We can take both into account using Smith's method:
//
//       G(n, v, l, k) = Gsub(n, v, k)Gsub(n, l, k)
//
// The geometry function is a multiplier between [0.0, 1.0] with white or 1.0 measuring no microfacet shadowing
// and black or 0.0 complete microfacet shadowing.
float GeometrySchlickGGX(float NdotV, float roughness)
{
	float a = roughness;
	float k = (a * a) / 2.0;    // here is ibl

	float nom = NdotV;
	float denom = NdotV * (1.0 - k) + k;
	return nom / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
	float NdotV = max(dot(N, V), 0.0);
	float NdotL = max(dot(N, L), 0.0);
	float ggx1 = GeometrySchlickGGX(NdotV, roughness);
	float ggx2 = GeometrySchlickGGX(NdotL, roughness);
	return ggx1 * ggx2;
}

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

	vec3 up = abs(N.z) < 0.999 ? vec3(0.0, 0.0, 1.0) : vec3(1.0, 0.0, 0.0);
	vec3 tangent = normalize(cross(up, N));
	vec3 bitangent = cross(N, tangent);

	// tangent space to world space(think of the TBN)
	vec3 sampleVec = tangent * H.x + bitangent * H.y + N * H.z;
	return normalize(sampleVec);
}

vec2 IntegrateBRDF(float NdotV, float roughness)
{
	// we calculate BRDF in tangent space which is z-up.
	vec3 N = vec3(0.0, 0.0, 1.0);

	vec3 V;
	V.x = sqrt(1.0f - NdotV * NdotV);      // sin
	V.y = 0.0;                             // it is no matter which direction on exactly x-y since we assume all direction are 1.0
	V.z = NdotV;                           // cos

	float A = 0.0;
	float B = 0.0;

	const uint SAMPLE_COUNT = 1024u;
	for (uint i = 0u; i < SAMPLE_COUNT; i++)
	{
		vec2 Xi = Hammersley(i, SAMPLE_COUNT);
		vec3 H  = ImportanceSampleGGX(Xi, N, roughness);
		vec3 L  = normalize(2.0 * dot(H, V) * H - V);

		float NdotL = max(L.z, 0.0);
		float NdotH = max(H.z, 0.0);
		float VdotH = max(dot(H, V), 0.0);

		if (NdotL > 0.0)
		{
			float G     = GeometrySmith(N, V, L, roughness);
			float G_Vis = G * VdotH / (NdotH * NdotV);
			float Fc    = pow(1.0 - VdotH, 5.0);

			A += G_Vis * (1.0 - Fc);
			B += G_Vis * Fc;
		}
	}

	A /= float(SAMPLE_COUNT);
	B /= float(SAMPLE_COUNT);
	return vec2(A, B);
}

out vec2 FragColor;

in vec2 TexCoords;

void main()
{
	vec2 integrateBRDF = IntegrateBRDF(TexCoords.x, TexCoords.y);
	FragColor = integrateBRDF;
}
