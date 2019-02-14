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
// since solid angle(ω) is hard to work with, we use spherical coordinates θ and ϕ to solve the integral.
//
//   -------------------------------------------------------------------------- 
//   |                         c    ∫2π  ∫π/2 					              | 
//   |   Lo(p, φo, θo) = kd * --- * ∫    ʃ    Li(p, φi, θi) cos(θ)sin(θ)dθdφ  | 
//   |                         π    ∫φ=0 ʃθ=0      				              | 
//   -------------------------------------------------------------------------- 
//
// NOTE:Note that (due to the general properties of a spherical shape) the hemisphere's discrete sample area gets smaller 
// the higher the zenith angle θ as the sample regions converge towards the center top. To compensate for the smaller areas, 
// we weigh its contribution by scaling the area by sinθ clarifying the added sin.
//
// here we are going to compute it with Reimann sum definition here. MORE DETAILS https://www.youtube.com/watch?v=gFpHHTxsDkI
//
//     -----------------------------------------------------
//     |  ∫b               n		       	       b - a   |
//     |  ∫ f(x)dx = lim   ∑ f(xi*)△x		 △x = -------  |
//     |  ∫a         n->∞ i=1			,	         n	   |
//     -----------------------------------------------------
//
// This translates the integral to the following discrete version as based on the Riemann sum
// given n1 and n2 discrete samples on each spherical coordinate respectively:
//
//   ---------------------------------------------------------------------- 
//   |                         c   n1  n2 		                          | 
//   |   Lo(p, φo, θo) = kd * ---  ∑   ∑  Li(p, φi, θi) cos(θ)sin(θ)△θ△φ  | 
//   |                         π  φ=0 θ=0      			                  | 
//   ---------------------------------------------------------------------- 
//	
//         2π - 0		         π/2 - 0
//   △φ = ---------		   △θ = ---------
//           n1		,               n2
//
// we can finally simplify this equation to:  
//
//   ---------------------------------------------------------------------------------- 
//   |                         c        π²      n1  n2 					              | 
//   |   Lo(p, φo, θo) = kd * --- * ---------   ∑   ∑  Li(p, φi, θi) cos(θ)sin(θ)     | 
//   |                         π      n1*n2    φ=0 θ=0      			              | 
//   ---------------------------------------------------------------------------------- 


#shader fragment
#version 330 core

const float PI = 3.14159265359;

out vec3 FragColor;

in vec3 LocalPos;

uniform samplerCube environmentMap;

void main()
{
	// the sample direction is the hemisphere's orientation
	vec3 normal = normalize(LocalPos);

	vec3 irradiance = vec3(0.0);

	vec3 up    = vec3(0.0, 1.0, 0.0);
	vec3 right = normalize(cross(up, normal));
	up         = cross(normal, right);
	
	float sampleDelta = 0.025;
	int     nrSamples = 0;
	for (float phi = 0.0; phi < 2.0 * PI; phi += sampleDelta)
	{
		for (float theta = 0.0; theta < 0.5 * PI; theta += sampleDelta)
		{
			float sinTheta = sin(theta);                                   // remember angle always grow from positive to negative
			float cosTheta = cos(theta);
			// sample vector in tangent space
			vec3 tangentSample = vec3(cos(phi) * sinTheta,
									 sin(phi) * sinTheta,
									 cosTheta);

			// tangent to world space 
			vec3 sampleVec = tangentSample.x * right + tangentSample.y * up + tangentSample.z * normal;

			// Note that we scale the sampled color value by cos(theta) due to the light being weaker at larger angles
			// and by sin(theta) to account for the smaller sample areas in the higher hemisphere areas.
			irradiance += texture(environmentMap, sampleVec).rgb * cosTheta * sinTheta;
			nrSamples++;                                                   // represent the (n1 * n2)
		}
	}
	
	// multiple with π here for we dont need divide π outside on the lambert diffuse term
	irradiance = PI * irradiance * (1.0 / float(nrSamples));

	FragColor  = irradiance;
}
