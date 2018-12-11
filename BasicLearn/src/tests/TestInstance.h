#pragma once

#include "Test.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture2D.h"
#include "Camera.h"
#include "Model.h"

#include <vector>

namespace test {

	class TestInstance: public Test
	{
	public:
		TestInstance();
		~TestInstance();

		void OnUpdate(float delaTime) override;
		void OnRender() override;

	private:
		Camera m_Camera;
		std::unique_ptr<Model> m_Nanosuit;
		std::unique_ptr<VertexBuffer> m_QuadVBO;
		std::unique_ptr<VertexArray> m_VAO;

		std::unique_ptr<Shader> m_ModelShader;
		std::unique_ptr<Shader> m_NormalVisualizeShader;
	};

}