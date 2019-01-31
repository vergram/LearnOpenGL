#shader vertex
#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

out VS_OUT{
	vec3 Normal;
	vec3 WorldPos;
	vec2 TexCoords;
}vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform bool reverse_normals;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0f);
	mat3 NormalMatrix = transpose(inverse(mat3(model)));
	if (reverse_normals)
		vs_out.Normal = NormalMatrix * (-1.0 * aNormal);
	else
		vs_out.Normal = NormalMatrix * aNormal;
	vs_out.WorldPos = vec3(model * vec4(aPos, 1.0f));
	vs_out.TexCoords = aTexCoords;

}


// ---------------------------------------- physically based rendering -------------------------------------------------
//
// 1. Be based on the microfacet surface model.
//    No surface is completely smooth on a microscopic level, but seeing as these microfacets are small enough 
//    that we can't make a distinction between them on a per-pixel basis we statistically approximate 
//    the surface's microfacet roughness given a roughness parameter. 
//
// 2. Be energy conserving.
//    The microfacet approximation employs a form of energy conservation: 
//    outgoing light energy should never exceed the incoming light energy (excluding emissive surfaces).
//    For energy conservation to hold we need to make a clear distinction between diffuse and specular light. 
//    The moment a light ray hits a surface, it gets split in both a refraction part and a reflection part. 
//
// 3. Use a physically based BRDF.
//    The BRDF, or bidirectional reflective distribution function approximates how much each individual light
//    ray ωi contributes to the final reflected light of an opaque surface given its material properties.
//
// ----------------------------------------------------------------------------------------------------------------------


// ---------------------------------------- here is pbr render equation ------------------------------------------------
//
//                                           ∫  
//                              Lo(p, ωo) =  ∫ fr(p, ωi, ωo) Li(p, ωi) n • ωi dωi
//                                           ∫Ω 
//
// ---------------------------------------------------------------------------------------------------------------------
// 
// Lo(p, ωo): 
// The reflectance equation calculates the sum of reflected radiance Lo(p,ωo) of a point p in direction ωo which is the outgoing direction to the viewer.
//
// Li(p, ωi):
// here we use radiance to measure light power, represented the radiance come from ωi.
//
// n • ωi:
// Radiance is a radiometric measure of the amount of light in an area scaled by the incident (or incoming) angle θ of the light to the surface's normal as cosθ: 
// light is weaker the less it directly radiates onto the surface and strongest when it is directly perpendicular to the surface.
//
// --------------------------------------------------------------------------------------------------------------------------------
// | NOTE: this variable above describe the light part of pbr, they both describe light energy from a radiometry perspective,	  |
// |      and fr(p, ωi, ωo) is going to bring the surface roughness into accout, describe how surface roughness impact reflection |
// |      and refraction base on the microfacet surface model.																	  |
// --------------------------------------------------------------------------------------------------------------------------------
//
// fr(p, ωi, ωo):
// here we use Cook-Torrance BRDF to approximate the surface's reaction to light.
//
// The Cook-Torrance BRDF contains both a diffuse and specular part:
//          --------------------------------------------
//          | fr = kd * flambert + ks * fcook−torrance |
//          --------------------------------------------
// kd is the ratio of incoming light energy that gets refracted with ks being the ratio that gets reflected. 
//
//                             -------------------
//                             |              c  |
// diffuse part:               | flambert =  --- |
//                             |              π  |
//                             -------------------
// With c being the albedo or surface color(think of the diffuse surface texture).
// The divide by pi is there to normalize the diffuse light as the earlier denoted integral that contains the BRDF is scaled by π.
//
//                       ------------------------------------------
//                       |                          DFG	     	  |
// specular part:        |  fcook-torrance = -------------------  |
//						 |					 4(ωo • n)(ωi • n)	  |
//                       ------------------------------------------
// The Cook-Torrance specular BRDF consists of three functions and a normalization factor in the denominator.
// Each of the D, F and G symbols represent a type of function that approximates a specific part of the surface's reflective properties.
// These are defined as the normal Distribution function, the Fresnel equation and the Geometry function:
// 
// Normal distribution function: approximates the amount the surface's microfacets are aligned to the halfway vector influenced by the roughness of the surface;
// this is the primary function approximating the microfacets.
//
// Geometry function : describes the self - shadowing property of the microfacets.
// When a surface is relatively rough the surface's microfacets can overshadow other microfacets thereby reducing the light the surface reflects.
//
// Fresnel equation : The Fresnel equation describes the ratio of surface reflection at different surface angles.
//
// AND FINALLY, we can get the render equation like this:
//
//                          ∫        c            DFG
//             Lo(p, ωo) =  ∫ (kd * --- + -------------------) Li(p, ωi) n • ωi dωi
//                          ∫Ω       π     4(ωo • n)(ωi • n)
//
// ----------------------------------------------------------------------------------------------------------------------


#shader fragment
#version 330 core

const float PI = 3.14159265359;

// The normal distribution function D statistically approximates the relative surface area of microfacets exactly aligned to the (halfway) vector h.
//                                          α²    
//           NDFGGXTR(n, h, α) = ------------------------
//                                π((n⋅h)²(α² − 1) + 1)²
// When the roughness is low (thus the surface is smooth) a highly concentrated number of microfacets are aligned to halfway vectors over a small radius. 
// Due to this high concentration the NDF displays a very bright spot
float DistributionGGX(vec3 N, vec3 H, float a)
{
	float a2     = a * a;
	float NdotH  = dot(N, H);
	float NdotH2 = NdotH * NdotH;

	float nom    = a2;
	float denom  = NdotH2 * (a2 - 1) + 1;
	denom        = PI * denom * denom;

	return nom / denom;
}

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
float GeometrySchlickGGX(float NdotV, float k)
{
	float nom   = NdotV;
	float denom = NdotV * (1.0 - k) + k;
	return nom / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float k)
{
	float NdotV = max(dot(N, V), 0.0);
	float NdotL = max(dot(N, L), 0.0);
	float ggx1  = GeometrySchlickGGX(NdotV, k);
	float ggx2  = GeometrySchlickGGX(NdotL, k);
	return ggx1 * ggx2;
}

// The Fresnel equation describes the ratio of light that gets reflected over the light that gets refracted, 
// which varies over the angle we're looking at a surface. The moment light hits a surface,
// based on the surface to view angle the Fresnel equation tells us the percentage of light that gets reflected.
// here we use Fresnel-Schlick approximation:
//             FSchlick(h, v, F0) = F0 + (1 − F0)(1 − (h⋅v))⁵
// F0 represents the base reflectivity of the surface, which we calculate using something called the indices of refraction
// or IOR and as you can see on a sphere surface, the more we look towards the surface's grazing angles 
// (with the halfway-view angle reaching 90 degrees) the stronger the Fresnel and thus the reflections.
//
// One is that the Fresnel-Schlick approximation is only really defined for dielectric or non-metal surfaces. 
// For conductor surfaces (metals) calculating the base reflectivity using their indices of refraction 
// doesn't properly hold and we need to use a different Fresnel equation for conductors altogether. 
// As this is inconvenient we further approximate by pre-computing the surface's response at normal incidence (F0) 
// (at a 0 degree angle as if looking directly onto a surface) and interpolate this value based on the view angle
// as per the Fresnel-Schlick approximation such that we can use the same equation for both metals and non-metals.
//
// A base reflectivity of 0.04 holds for most dielectrics and produces physically plausible results without having to author an additional surface parameter.
// since metallic surfaces absorb all refracted light they have no diffuse reflections and we can directly use the surface color texture as their base reflectivity.
vec3 FresnelSchlick(float cosTheta, vec3 F0)
{
	return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

out vec4 FragColor;

in VS_OUT{
	vec3 Normal;
	vec3 WorldPos;
	vec2 TexCoords;
}fs_in;

uniform vec3 camPos;

// material parameters
//uniform vec3 albedo;
//uniform float metallic;
//uniform float roughness;
//uniform float ao;

// material map
uniform sampler2D albedoMap;
uniform sampler2D metallicMap;
uniform sampler2D roughnessMap;
uniform sampler2D aoMap;
uniform sampler2D normalMap;

// light parameters
uniform vec3 lightPos[4];
uniform vec3 lightColor[4];

// all about tangent space here: http://www.terathon.com/code/tangent.html
vec3 GetNormalFromMap()
{
	// dFdx means the paramter variable in two fragment's value difference. 
	// More detail here: https://stackoverflow.com/questions/16365385/explanation-of-dfdx
	vec3 Q1  = dFdx(fs_in.WorldPos);
	vec3 Q2  = dFdy(fs_in.WorldPos);
	vec2 st1 = dFdx(fs_in.TexCoords);
	vec2 st2 = dFdy(fs_in.TexCoords);

	vec3 N = normalize(fs_in.Normal);
	vec3 T = normalize(st2.t * Q1 - st1.t * Q2);
	
	T = T - N * dot(N, T);
	vec3 B = cross(N, T);

	mat3 TBN = mat3(T, B, N);

	vec3 normal = texture(normalMap, fs_in.TexCoords).xyz * 2.0 - 1.0;
	return normalize(TBN * normal);
}


void main()
{
	vec3  albedo    = texture(albedoMap, fs_in.TexCoords).rgb;
	float metallic  = texture(metallicMap, fs_in.TexCoords).r;
	float roughness = texture(roughnessMap, fs_in.TexCoords).r;
	float ao        = texture(aoMap, fs_in.TexCoords).r;

	vec3 N = GetNormalFromMap();
	vec3 V = normalize(camPos - fs_in.WorldPos);

	// calculate reflectance at normal incidence; if dia-electric (like plastic) use F0 
	// of 0.04 and if it's a metal, use the albedo color as F0 (metallic workflow) 	 
	vec3 F0 = vec3(0.04);
	F0 = mix(F0, albedo, metallic);

	vec3 Lo = vec3(0.0);
	for (int i = 0; i < 1; i++)
	{
		// calculate per-light radiance
		vec3  L           = normalize(lightPos[i] - fs_in.WorldPos);
		vec3  H           = normalize(L + V);

		float distance    = length(fs_in.WorldPos - lightPos[i]);
		float attenuation = 1.0 / (distance * distance);                        // attenuate the light sources by the more physically correct inverse-square law.
		vec3  radiance    = lightColor[i] * attenuation;

		// cook-torrance BRDF
		float NDF         = DistributionGGX(N, H, roughness);
		float G           = GeometrySmith(N, V, L, roughness);
		vec3  F           = FresnelSchlick(max(dot(H, V), 0.0), F0);

		vec3  numerator   = NDF * G * F;
		float denominator = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0);
		vec3  specular    = numerator / max(denominator, 0.001);                // constrain the denominator to 0.001 to prevent a divide by zero in case any dot product ends up 0.0.

		vec3  kS          = F;
		vec3  kD          = vec3(1.0) - kS;

		// metallic surfaces don't refract light and thus have no diffuse reflections we enforce this property by nullifying kD if the surface is metallic.
		kD                = kD * (1.0 - metallic);
		
		float NdotL       = max(dot(N, L), 0.0);

		Lo               += (kD * albedo / PI + specular) *  radiance * NdotL;
	}

	vec3 ambient = vec3(0.03) * albedo * ao;
	vec3 color   = ambient + Lo;

	// since Lo can vary wildly, we use hdr mapping the radiance
	color = color / (color + vec3(1.0));
	color = pow(color, vec3(1.0 / 2.2));

	FragColor = vec4(color, 1.0);
}