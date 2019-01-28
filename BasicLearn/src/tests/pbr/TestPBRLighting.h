#pragma once

#include "tests/Test.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture2D.h"
#include "Camera.h"
#include "Model.h"
#include "Sphere.h"

#include <vector>

namespace test {

	class TestPBRLighting : public Test
	{
	public:
		TestPBRLighting();
		~TestPBRLighting();

		void OnUpdate(float delaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		inline float lerp(float a, float b, float factor) { return a + factor * (b - a); }

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
		std::unique_ptr<Shader> m_PBRLightingShader;

		std::vector<glm::vec3> m_LightPositions;
		std::vector<glm::vec3> m_LightColors;

		bool m_IsShowMouse;
		float m_Exposure;

		int m_DebugMode;
	};

}