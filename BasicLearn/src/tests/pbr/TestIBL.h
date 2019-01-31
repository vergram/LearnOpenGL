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

	class TestIBL : public Test
	{
	public:
		TestIBL();
		~TestIBL();

		void OnUpdate(float delaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		void buildCubeMap(const char* path);
		unsigned int loadHDREnvmap(const char* path);
		unsigned int generateHDRCubeMap(int size);
		void renderSkybox(unsigned int cubemap);

		Camera m_Camera;

		std::unique_ptr<VertexBuffer> m_PlaneVBO;
		std::unique_ptr<VertexArray> m_PlaneVAO;
		std::unique_ptr<VertexBuffer> m_CubeVBO;
		std::unique_ptr<VertexArray> m_CubeVAO;
		std::unique_ptr<VertexBuffer> m_QuadVBO;
		std::unique_ptr<VertexArray> m_QuadVAO;

		std::unique_ptr<Sphere> m_Sphere;
		std::unique_ptr<Model> m_Nanosuit;
	
		std::unique_ptr<Shader> m_IBLShader;
		std::unique_ptr<Shader> m_EquirectangularToCubeMapShader;
		std::unique_ptr<Shader> m_ConvolutionShader;
		std::unique_ptr<Shader> m_SkyboxShader;
		std::unique_ptr<Shader> m_DebugShader;

		std::unique_ptr<Texture2D> m_Albedo;
		std::unique_ptr<Texture2D> m_Metallic;
		std::unique_ptr<Texture2D> m_Roughness;
		std::unique_ptr<Texture2D> m_Ao;
		std::unique_ptr<Texture2D> m_Normal;

		std::vector<glm::vec3> m_LightPositions;
		std::vector<glm::vec3> m_LightColors;

		bool m_IsShowMouse;
		float m_Exposure;

		std::vector<unsigned int> m_EnvCubeMaps;
		std::vector<unsigned int> m_IrradianceMaps;
		int m_DebugMode;
	};

}