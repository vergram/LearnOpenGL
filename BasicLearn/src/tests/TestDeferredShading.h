#pragma once

#include "Test.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture2D.h"
#include "Camera.h"
#include "Model.h"
#include "Sphere.h"

#include <vector>

namespace test {

	class TestDeferredShading : public Test
	{
	public:
		TestDeferredShading();
		~TestDeferredShading();

		void OnUpdate(float delaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		float calculateLightRadius(glm::vec3& lightColor);

		Camera m_Camera;

		std::unique_ptr<VertexBuffer> m_PlaneVBO;
		std::unique_ptr<VertexArray> m_PlaneVAO;
		std::unique_ptr<VertexBuffer> m_CubeVBO;
		std::unique_ptr<VertexArray> m_CubeVAO;
		std::unique_ptr<VertexBuffer> m_QuadVBO;
		std::unique_ptr<VertexArray> m_QuadVAO;

		std::unique_ptr<Shader> m_QuadShader;
		std::unique_ptr<Shader> m_LightCubeShader;
		std::unique_ptr<Shader> m_GeometryPassShader;
		std::unique_ptr<Shader> m_LightingPassShader;
		std::unique_ptr<Shader> m_PointLightPassShader;
		std::unique_ptr<Shader> m_StencilPassShader;
		std::unique_ptr<Shader> m_DebugShader;

		std::unique_ptr<Sphere> m_Sphere;

		std::unique_ptr<Model> m_Nanosuit;

		std::vector<glm::vec3> m_LightPositions;
		std::vector<glm::vec3> m_LightColors;
		std::vector<glm::vec3> m_ObjectPositions;

		bool m_IsShowMouse;
		float m_Exposure;

		enum GBufferTextureType
		{
			POSITION                   = 0,
			NORMAL                     = 1,
			DIFFUSE_SPECULAR_INTENSITY = 2,
			FINAL_TEXTURE              = 3
		};

		unsigned int m_GBufferTextures[4];
		unsigned int m_GBuffer;
		unsigned int m_GDepthBuffer;
		int m_DebugMode;
	};

}