#include "SpriteRenderer.h"
#include <glad/glad.h>

#include "glm/ext.hpp"

namespace BreakOut {



	SpriteRenderer::SpriteRenderer(Shader & shader)
		:m_Shader(shader)
	{
		this->initRenderData();
	}

	SpriteRenderer::~SpriteRenderer()
	{}

	void SpriteRenderer::DrawSprite(Texture2D & texture, glm::vec2 position, glm::vec2 size, float rotate, glm::vec3 color)
	{
		this->m_Shader.Bind();
		glm::mat4 model(1.0f);

		model = glm::translate(model, glm::vec3(position, 0.0f));
		
		// sice we are sprite defined the origin on top left, we want move it back to origin(0,0) for rotation.
		model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
		model = glm::rotate(model, rotate, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));
		
		model = glm::scale(model, glm::vec3(size, 0.0f));

		this->m_Shader.SetUniformMatrix4fv("model", model);
		this->m_Shader.SetUniform3f("spriteColor", color);
		texture.Bind();

		glBindVertexArray(m_VAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);
	}

	void SpriteRenderer::initRenderData()
	{
		unsigned int VBO;
		float vertices[] = {
			// Pos      // Tex
			0.0f, 1.0f, 0.0f, 1.0f,
			1.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 0.0f,

			0.0f, 1.0f, 0.0f, 1.0f,
			1.0f, 1.0f, 1.0f, 1.0f,
			1.0f, 0.0f, 1.0f, 0.0f
		};
		
		glGenVertexArrays(1, &this->m_VAO);
		glBindVertexArray(this->m_VAO);

		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)0);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

}