#pragma once

#include "Test.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture2D.h"
#include "Camera.h"

#include <vector>

namespace test {

	class TestFrameBuffer: public Test
	{
	public:
		TestFrameBuffer();
		~TestFrameBuffer();

		void OnUpdate(float delaTime) override;
		void OnRender() override;

	private:
		Camera m_Camera;
		std::unique_ptr<VertexBuffer> m_CubeVBO;
		std::unique_ptr<VertexArray> m_CubeVAO;
		std::unique_ptr<VertexBuffer> m_PlaneVBO;
		std::unique_ptr<VertexArray> m_PlaneVAO;
		std::unique_ptr<VertexBuffer> m_GrassVBO;
		std::unique_ptr<VertexArray> m_GrassVAO;
		std::unique_ptr<VertexBuffer> m_QuadVBO;
		std::unique_ptr<VertexArray> m_QuadVAO;

		std::unique_ptr<Shader> m_SceneShader;
		std::unique_ptr<Shader> m_TransparentShader;
		std::unique_ptr<Shader> m_SingleColorShader;
		std::unique_ptr<Shader> m_QuadShader;
		
		Texture2D m_CubeTexture;
		Texture2D m_PlaneTexture;
		Texture2D m_TransparentTexture;

		unsigned int m_MSAAfbo;
		unsigned int m_MSAATexture;
		unsigned int m_rbo;
		unsigned int m_Intermediatefbo;
		unsigned int m_ScreenTexture;

	};

}