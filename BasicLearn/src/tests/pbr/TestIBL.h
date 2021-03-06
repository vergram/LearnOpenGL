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

	struct IBL
	{
		unsigned int EnvironmentMap;
		unsigned int DiffuseIrradianceMap;
		unsigned int SpecularPreFilterMap;
		unsigned int SpecularBRDFMap;
	};

	class TestIBL : public Test
	{
	public:
		TestIBL();
		~TestIBL();

		void OnUpdate(float delaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		IBL* bakeIBL(const char* path);
		unsigned int loadHDREnvmap(const char* path);
		unsigned int generateHDRCubeMap(int size, bool mipmap = false);
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
	
		std::unique_ptr<Shader> m_PBRShader;
		std::unique_ptr<Shader> m_EquirectangularToCubeMapShader;
		std::unique_ptr<Shader> m_DiffuseIrradianceConvolutionShader;
		std::unique_ptr<Shader> m_SpecularPreFilterShader;
		std::unique_ptr<Shader> m_SpecularBRDFShader;
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

		std::vector<IBL*> m_IBLTextures;

		int m_DebugMode;
	};

}