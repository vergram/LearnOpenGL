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

	class TestBloom : public Test
	{
	public:
		TestBloom();
		~TestBloom();

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

		std::unique_ptr<Shader> m_HDRQuadShader;
		std::unique_ptr<Shader> m_LightCubeShader;
		std::unique_ptr<Shader> m_HDRSceneShader;
		std::unique_ptr<Shader> m_GaussBlurShader;

		std::unique_ptr<Texture2D> m_WoodTexture;
		std::unique_ptr<Texture2D> m_ContainerTexture;

		std::vector<glm::vec3> m_LightPositions;
		std::vector<glm::vec3> m_LightColors;

		bool m_IsShowMouse;
		float m_Exposure;

		unsigned int m_HDRfbo;
		unsigned int m_ColorBuffers[2];
		unsigned int m_HDRDepthBuffer;

		unsigned int m_PingPongFbos[2];
		unsigned int m_PingPongColorBuffers[2];


	};

}