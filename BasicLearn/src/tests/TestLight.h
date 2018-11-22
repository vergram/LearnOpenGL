#pragma once

#include "Test.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture2D.h"
#include "Camera.h"
#include <vector>

namespace test {

	class TestLight: public Test
	{
	public:
		TestLight();
		~TestLight();

		void OnUpdate(float delaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		Camera m_Camera;
		Texture2D m_WoodTexture;
		Texture2D m_SteelTexture;
		Texture2D m_EmissionTexture;
		std::unique_ptr<VertexBuffer> m_VBO;

		std::unique_ptr<VertexArray> m_CubeVAO;
		std::unique_ptr<Shader> m_CubeShader;

		std::unique_ptr<VertexArray> m_LampVAO;
		std::unique_ptr<Shader> m_LampShader;
	};

}