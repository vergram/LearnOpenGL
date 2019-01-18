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

	class TestSSAO : public Test
	{
	public:
		TestSSAO();
		~TestSSAO();

		void OnUpdate(float delaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		float calculateLightRadius(glm::vec3& lightColor);
		inline float lerp(float a, float b, float factor) { return a + factor * (b - a); }

		void GeometryPass();

		void SSAOPass();

		void LightingPass();

		Camera m_Camera;

		std::unique_ptr<VertexBuffer> m_PlaneVBO;
		std::unique_ptr<VertexArray> m_PlaneVAO;
		std::unique_ptr<VertexBuffer> m_CubeVBO;
		std::unique_ptr<VertexArray> m_CubeVAO;
		std::unique_ptr<VertexBuffer> m_QuadVBO;
		std::unique_ptr<VertexArray> m_QuadVAO;

		std::unique_ptr<Sphere> m_Sphere;

		std::unique_ptr<Model> m_Nanosuit;
	
		std::unique_ptr<Shader> m_QuadShader;
		std::unique_ptr<Shader> m_GeometryPassShader;
		std::unique_ptr<Shader> m_LightingPassShader;
		std::unique_ptr<Shader> m_SSAOBlurShader;
		std::unique_ptr<Shader> m_SSAOPassShader;

		glm::vec3 m_LightPosition;
		glm::vec3 m_LightColor;

		std::vector<glm::vec3> m_SSAO_Kernel;
		std::vector<glm::vec3> m_SSAO_Noise;
		unsigned int m_NoiseTexture;

		bool m_IsShowMouse;
		float m_Exposure;

		enum GBufferTextureType
		{
			POSITION  = 0,
			NORMAL    = 1,
			ALBEDO    = 2
		};

		enum SSAOTextureType
		{
			SSAO = 0,
			BLUR = 1
		};

		unsigned int m_GBufferTextures[4];
		unsigned int m_GBufferFBO;
		unsigned int m_GDepthBuffer;

		unsigned int m_SSAO_fbo;
		unsigned int m_SSAOTextures[2];


		int m_DebugMode;
	};

}