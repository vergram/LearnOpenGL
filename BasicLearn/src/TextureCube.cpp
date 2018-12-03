#include "TextureCube.h"
#include "Renderer.h"
#include "stb_image/stb_image.h"

#include <iostream>

TextureCube::TextureCube(const std::vector<std::string> & paths)
	: m_RendererID(0), m_FilePaths(paths), m_LocalBuffer(nullptr), 
	m_Width(0), m_Height(0), m_BPP(0)
{
	//stbi_set_flip_vertically_on_load(1);      // flip the image by default 
	
	GLCall(glGenTextures(1, &m_RendererID));
	GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID));
	GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));

	for (unsigned int i = 0; i < 6; i++)
	{
		std::string image = m_FilePaths[i];
		m_LocalBuffer = stbi_load(image.c_str(), &m_Width, &m_Height, &m_BPP, 0);
		GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, m_LocalBuffer));
		if (m_LocalBuffer)
			stbi_image_free(m_LocalBuffer);
	}
	GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));
}

TextureCube::~TextureCube()
{
	GLCall(glDeleteTextures(1, &m_RendererID));
}

void TextureCube::Bind(unsigned int slot) const
{
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID));
}

void TextureCube::Unbind() const
{
	GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));
}
