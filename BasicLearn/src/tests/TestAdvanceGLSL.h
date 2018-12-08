#pragma once

#include "Test.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture2D.h"
#include "Camera.h"

#include <vector>

namespace test {

	class TestAdvanceGLSL: public Test
	{
	public:
		TestAdvanceGLSL();
		~TestAdvanceGLSL();

		void OnUpdate(float delaTime) override;
		void OnRender() override;

	private:
		Camera m_Camera;
		std::unique_ptr<VertexBuffer> m_VBO;
		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr<Shader> m_ShaderR;
		std::unique_ptr<Shader> m_ShaderG;
		std::unique_ptr<Shader> m_ShaderB;
		std::unique_ptr<Shader> m_ShaderY;

		unsigned int ubo;
	};

}