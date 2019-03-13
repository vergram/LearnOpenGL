#include "post_processor.h"

#include <glad/glad.h>

#include <iostream>

#include "Renderer.h"

namespace BreakOut {

	unsigned int textureMultisample;

	PostProcessor::PostProcessor(Shader & shader, unsigned int width, unsigned int height)
		:m_Shader(shader), Texture(), Width(width), Height(height), Chaos(false), Confuse(false), Shake(false)
	{
		glGenFramebuffers(1, &MSFBO);
		glGenFramebuffers(1, &FBO);
		glGenRenderbuffers(1, &RBO);

		// set up Multisample framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, MSFBO);
		glBindRenderbuffer(GL_RENDERBUFFER, RBO);
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_RGB, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, RBO);
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "ERROR::POSTPROCESSOR: Failed to initialize MSFBO" << std::endl;

		// set up framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, FBO);
		Texture.Generate(width, height, NULL);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, Texture.ID, 0);
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "ERROR::POSTPROCESSOR: Failed to initialize FBO" << std::endl;
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		initRenderData();
		m_Shader.Bind();
		m_Shader.SetUniform1i("scene", 0);
		float offset = 1.0f / 300.0f;
		float offsets[9][2] = {
			{ -offset,  offset  },  // top-left
			{  0.0f,    offset  },  // top-center
			{  offset,  offset  },  // top-right
			{ -offset,  0.0f    },  // center-left
			{  0.0f,    0.0f    },  // center-center
			{  offset,  0.0f    },  // center - right
			{ -offset, -offset  },  // bottom-left
			{  0.0f,   -offset  },  // bottom-center
			{  offset, -offset  }   // bottom-right    
		};
		glUniform2fv(glGetUniformLocation(m_Shader.GetID(), "offsets"), 9, &offsets[0][0]);
		
		int edge_kernel[9] = {
			-1, -1, -1,
			-1,  8, -1,
			-1, -1, -1
		};
		glUniform1iv(glGetUniformLocation(m_Shader.GetID(), "edge_kernel"), 9, edge_kernel);

		float blur_kernel[9] = {
		1.0 / 16, 2.0 / 16, 1.0 / 16,
		2.0 / 16, 4.0 / 16, 2.0 / 16,
		1.0 / 16, 2.0 / 16, 1.0 / 16
		};
		glUniform1fv(glGetUniformLocation(m_Shader.GetID(), "blur_kernel"), 9, blur_kernel);
	}

	void PostProcessor::BeginRender()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, MSFBO);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void PostProcessor::EndRender()
	{
		glBindFramebuffer(GL_READ_FRAMEBUFFER, MSFBO);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FBO);
		glBlitFramebuffer(0, 0, Width, Height, 0, 0, Width, Height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void PostProcessor::Render(float time)
	{
		m_Shader.Bind();
		m_Shader.SetUniform1f("time", time);
		m_Shader.SetUniform1i("chaos", Chaos);
		m_Shader.SetUniform1i("confuse", Confuse);
		m_Shader.SetUniform1i("shake", Shake);

		Texture.Bind(0);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);


	}

	void PostProcessor::initRenderData()
	{
		// Configure VAO/VBO
		unsigned int VBO;
		float vertices[] = {
			// Pos        // Tex
			-1.0f, -1.0f, 0.0f, 0.0f,
			 1.0f,  1.0f, 1.0f, 1.0f,
			-1.0f,  1.0f, 0.0f, 1.0f,

			-1.0f, -1.0f, 0.0f, 0.0f,
			 1.0f, -1.0f, 1.0f, 0.0f,
			 1.0f,  1.0f, 1.0f, 1.0f
		};
		glGenVertexArrays(1, &this->VAO);
		glGenBuffers(1, &VBO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glBindVertexArray(this->VAO);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GL_FLOAT), (GLvoid*)0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

}