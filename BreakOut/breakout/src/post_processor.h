#pragma once

#include <glm/glm.hpp>

#include "shader.h"
#include "texture.h"

namespace BreakOut {

	class PostProcessor
	{
	public:
		Shader& m_Shader;
		Texture2D Texture;
		unsigned int Width, Height;
		bool Chaos, Confuse, Shake;

		PostProcessor(Shader& shader, unsigned int width, unsigned int height);

		void BeginRender();

		void EndRender();

		void Render(float time);
	private:
		unsigned int MSFBO, FBO;             // mutil sample fbo for anti-aliasing
		unsigned int RBO;
		unsigned int VAO;

		void initRenderData();
	};

}