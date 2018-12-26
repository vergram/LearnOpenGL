#pragma once

#include "Test.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture2D.h"
#include "Camera.h"

#include <vector>

namespace test {

	class TestNormalMap : public Test
	{
	public:
		TestNormalMap();
		~TestNormalMap();

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
		std::unique_ptr<Texture2D> m_BrickWall;
		std::unique_ptr<Texture2D> m_BrickWall_Normal;

		glm::mat4 m_ShadowProjection;
		glm::vec3 m_LightPos;

		bool m_IsShowMouse;

		unsigned int m_DepthCubeMapFbo;
		unsigned int m_DepthCubeMap;

	};

}