#pragma once

#include "Test.h"
#include "Model.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Camera.h"
#include "TextureCube.h"

#include <vector>

namespace test {

	class TestCubemap: public Test
	{
	public:
		TestCubemap();
		~TestCubemap();

		void OnUpdate(float delaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		Camera m_Camera;
		std::unique_ptr<Model> m_Model;
		std::unique_ptr<TextureCube> m_SkyboxTexture;

		std::unique_ptr<Shader> m_NanosuitShader;
		std::unique_ptr<Shader> m_SkyboxShader;
		std::unique_ptr<Shader> m_CubeShader;

		std::unique_ptr<VertexArray> m_SkyboxVAO;
		std::unique_ptr<VertexBuffer> m_SkyboxVBO;
		std::unique_ptr<VertexArray> m_CubeVAO;
		std::unique_ptr<VertexBuffer> m_CubeVBO;

	};

}