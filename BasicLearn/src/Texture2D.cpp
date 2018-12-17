#include "Texture2D.h"
#include "Renderer.h"
#include "stb_image/stb_image.h"

#include <iostream>


Texture2D::Texture2D(const std::string & path, bool gammaCorrection)
	: m_RendererID(0), m_FilePath(path), m_LocalBuffer(nullptr),
	m_Width(0), m_Height(0), m_BPP(0)
{
	stbi_set_flip_vertically_on_load(1);      // flip the image by default 
	m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 0);

	GLCall(glGenTextures(1, &m_RendererID));

	if (m_LocalBuffer)
	{
		GLenum internalFormat;
		GLenum dataFormat;
		if (m_BPP == 1)
		{
			internalFormat = dataFormat = GL_RED;
		}
		else if (m_BPP == 3)
		{
			internalFormat = gammaCorrection ? GL_SRGB : GL_RGB;
			dataFormat = GL_RGB;
		}
		else if (m_BPP == 4)
		{
			internalFormat = gammaCorrection ? GL_SRGB_ALPHA : GL_RGBA;
			dataFormat = GL_RGBA;
		}
		GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_Width, m_Height, 0, dataFormat, GL_UNSIGNED_BYTE, m_LocalBuffer));
		GLCall(glGenerateMipmap(GL_TEXTURE_2D));

		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));

		GLCall(glBindTexture(GL_TEXTURE_2D, 0));

		stbi_image_free(m_LocalBuffer);
	}
	else
	{
		std::cout << "[STB::ERROR::] Texture failed to load at path " << path << std::endl;
	}
}

Texture2D::~Texture2D()
{
	GLCall(glDeleteTextures(1, &m_RendererID));
}

void Texture2D::Bind(unsigned int slot) const
{
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
}

void Texture2D::Unbind() const
{
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}
