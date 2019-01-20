#shader vertex
#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoords;

out vec2 TexCoords;
out vec2 ViewRay;

uniform float gProjectionTangentHalfFov;
uniform float gProjectionAspect;

void main()
{
	gl_Position = vec4(aPos.x, aPos.y, 0.0f, 1.0f);
	TexCoords = aPos.xy * 0.5f + 0.5f;

	ViewRay.x = aPos.x * gProjectionTangentHalfFov * gProjectionAspect;
	ViewRay.y = aPos.y * gProjectionTangentHalfFov;
}


#shader fragment
#version 330 core

out vec3 FragColor;

in vec2 ViewRay;
in vec2 TexCoords;

uniform mat4 projection;

uniform sampler2D gDepthMap;

// everything math in here http://ogldev.atspace.co.uk/www/tutorial46/tutorial46.html 
// NOTE: here is a bit wrong with above link show that formula, I calculate myself to correct it base on the link below.
// more math about projection matrix here http://www.songho.ca/opengl/gl_projectionmatrix.html
// calculate view space z component base on projection matrix and depthmap
float CalcViewZ(vec2 texCoords)
{
	float depth = texture(gDepthMap, texCoords).x;
	float ViewZ = -projection[3][2] / (2.0f * depth - 1.0f + projection[2][2]);
	return ViewZ;
}

// calculate texCoord for gBuffer
vec2 CalcTexCoord()
{
	return gl_FragCoord.xy / vec2(800, 600);
}

void main()
{
	//vec2 TexCoords  = aTexCoords;

	//float depth = texture(gDepthMap, TexCoords).x * 2.0f - 1.0f;

	//vec4 fragPos = inverseProjection * vec4(TexCoords.xy, depth, 1.0);

	// reconstruct view-space position base on depthmap
	float ViewZ = CalcViewZ(TexCoords);
	float ViewX = ViewRay.x * (-ViewZ);
	float ViewY = ViewRay.y * (-ViewZ);
	vec3 fragPos = vec3(ViewX, ViewY, ViewZ);

	//FragColor = fragPos;
	//FragColor = vec3(texture(gDepthMap, TexCoords).x);
	FragColor = fragPos;
}