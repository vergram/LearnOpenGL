#pragma once

#include "Test.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"
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
		Texture m_WoodTexture;
		Texture m_SteelTexture;
		Texture m_EmissionTexture;
		std::unique_ptr<VertexBuffer> m_VBO;

		std::unique_ptr<VertexArray> m_CubeVAO;
		std::unique_ptr<Shader> m_CubeShader;

		std::unique_ptr<VertexArray> m_LightVAO;
		std::unique_ptr<Shader> m_LightShader;
	};

}