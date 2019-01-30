#shader vertex
#version 330 core

layout(location = 0) in vec3 aPos;

out vec3 WorldPos;

uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * vec4(aPos, 1.0f);
	WorldPos = aPos;
}


#shader fragment
#version 330 core

// since equirectangular is mapped a sphere to a flat rectangle,
// we use longitude and latitude to represent the point on that sphere surface.
// u = atan(v.z, v.x)    [  -π,   π] map to [0, 1]     u = u * 1/2π + 0.5
// v = asin(v.y)         [-π/2, π/2] map to [0, 1]     v = u * 1/π  + 0.5
// here: 1/2π = 0.1591    1/π = 0.3183
const vec2 mapFactor = vec2(0.1591, 0.3183);

// here we take a normalize vector on unit sphere and map it to 2D uv coordinates. DETAILS: https://en.wikipedia.org/wiki/Equirectangular_projection
vec2 SampleSphericalMap(vec3 v)
{
	vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
	uv *= mapFactor;
	uv += 0.5;
	return uv;
}

out vec3 FragColor;

in vec3 WorldPos;

uniform sampler2D equirectangularMap;

void main()
{
	vec2 uv    = SampleSphericalMap(normalize(WorldPos));
	vec3 color = texture(equirectangularMap, uv).rgb;
	FragColor  = color;
}
