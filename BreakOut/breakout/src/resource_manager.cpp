/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#include "resource_manager.h"

#include <iostream>
#include <sstream>
#include <fstream>

#include "stb_image/stb_image.h"

namespace BreakOut {

// Instantiate static variables
	std::map<std::string, Texture2D>    ResourceManager::Textures;
	std::map<std::string, Shader>       ResourceManager::Shaders;


	Shader ResourceManager::LoadShader(const std::string& shaderpath, std::string name)
	{
		Shaders[name] = loadShaderFromFile(shaderpath);
		return Shaders[name];
	}

	Shader& ResourceManager::GetShader(std::string name)
	{
		return Shaders[name];
	}

	Texture2D ResourceManager::LoadTexture(const GLchar *file, std::string name, GLboolean gammaCorrection)
	{
		Textures[name] = loadTextureFromFile(file, gammaCorrection);
		return Textures[name];
	}

	Texture2D& ResourceManager::GetTexture(const std::string& name)
	{
		return Textures[name];
	}

	void ResourceManager::Clear()
	{
		// (Properly) delete all shaders	
		for (auto iter : Shaders)
			glDeleteProgram(iter.second.GetID());
		// (Properly) delete all textures
		for (auto iter : Textures)
			glDeleteTextures(1, &iter.second.ID);
	}

	Shader ResourceManager::loadShaderFromFile(const std::string& shaderpath)
	{
		Shader shader(shaderpath);
		return shader;
	}

	Texture2D ResourceManager::loadTextureFromFile(const GLchar *file, GLboolean gammaCorrection)
	{
		// Create Texture object
		Texture2D texture;

		// Load image
		int width, height, bpp;

		//stbi_set_flip_vertically_on_load(1);      // flip the image by default 
		unsigned char* data = stbi_load(file, &width, &height, &bpp, 0);
		if (data)
		{
			GLenum internalFormat;
			GLenum dataFormat;
			if (bpp == 1)
			{
				internalFormat = dataFormat = GL_RED;
			}
			else if (bpp == 3)
			{
				internalFormat = gammaCorrection ? GL_SRGB : GL_RGB;
				dataFormat = GL_RGB;
			}
			else if (bpp == 4)
			{
				internalFormat = gammaCorrection ? GL_SRGB_ALPHA : GL_RGBA;
				dataFormat = GL_RGBA;
			}
			texture.Internal_Format = internalFormat;
			texture.Image_Format = dataFormat;

			// Now generate texture
			texture.Generate(width, height, data);

			stbi_image_free(data);
		}
		else
		{
			std::cout << "[STB::ERROR::] Texture failed to load at path " << file << std::endl;
		}

		return texture;
	}

}