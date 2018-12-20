#pragma once

#include "Test.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture2D.h"
#include "Camera.h"

#include <vector>

namespace test {

	class TestDepthMap : public Test
	{
	public:
		TestDepthMap();
		~TestDepthMap();

		void OnUpdate(float delaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		Camera m_Camera;
		std::unique_ptr<VertexBuffer> m_PlaneVBO;
		std::unique_ptr<VertexArray> m_PlaneVAO;
		std::unique_ptr<VertexBuffer> m_CubeVBO;
		std::unique_ptr<VertexArray> m_CubeVAO;
		std::unique_ptr<VertexBuffer> m_QuadVBO;
		std::unique_ptr<VertexArray> m_QuadVAO;


		std::unique_ptr<Shader> m_SimpleDepthMapShader;
		std::unique_ptr<Shader> m_ShadowMapShader;
		std::unique_ptr<Shader> m_QuadShader;
		std::unique_ptr<Texture2D> m_woodTexture;
		std::unique_ptr<Texture2D> m_woodTextureWithGammaCorrecetion;

		bool m_blinn;
		bool m_IsShowMouse;
		bool m_isSRGB;
		bool m_isGammaCorrect;

		unsigned int m_DepthMapFbo;
		unsigned int m_DepthMap;
	};

}