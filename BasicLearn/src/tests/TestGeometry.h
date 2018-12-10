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

	class TestGeometry: public Test
	{
	public:
		TestGeometry();
		~TestGeometry();

		void OnUpdate(float delaTime) override;
		void OnRender() override;

	private:
		Camera m_Camera;
		std::unique_ptr<Model> m_Nanosuit;
		std::unique_ptr<Shader> m_ModelShader;
		std::unique_ptr<Shader> m_NormalVisualizeShader;
	};

}