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

	class TestInstance: public Test
	{
	public:
		TestInstance();
		~TestInstance();

		void OnUpdate(float delaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		Camera m_Camera;
		std::unique_ptr<Model> m_Planet;
		std::unique_ptr<Model> m_Rock;

		std::unique_ptr<Shader> m_PlanetShader;
		std::unique_ptr<Shader> m_InstanceRockShader;

		glm::mat4 * m_ModelMatrices;
		unsigned int m_ModelMatrixAmount;

	};

}