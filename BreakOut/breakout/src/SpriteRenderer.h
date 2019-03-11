#pragma once

#include "shader.h"
#include "texture.h"

#include <glm/glm.hpp>

namespace BreakOut {

	class SpriteRenderer
	{
	public:
		SpriteRenderer(Shader& shader);
		virtual ~SpriteRenderer();

		void DrawSprite(Texture2D& texture, glm::vec2 position,
			glm::vec2 size = glm::vec2(10.0f, 10.0f), float rotate = 0.0f,
			glm::vec3 color = glm::vec3(1.0f));

	private:
		void initRenderData();

	private:
		Shader& m_Shader;
		unsigned int m_VAO;
	};

}