#shader vertex
#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 texCoord;

uniform mat4 u_mvp;

out vec2 v_TexCoord;

void main()
{
	gl_Position = u_mvp * vec4(aPos, 1.0f);
	v_TexCoord = vec2(texCoord.x, texCoord.y);
}


#shader fragment
#version 330 core

in vec2 v_TexCoord;
out vec4 t_color;

uniform sampler2D u_Texture0;
uniform sampler2D u_Texture1;

void main()
{
	t_color = mix(texture(u_Texture0, v_TexCoord), texture(u_Texture1, v_TexCoord), 0.2);
}