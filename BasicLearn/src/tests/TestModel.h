#pragma once

#include "Test.h"
#include "Model.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture2D.h"
#include "Camera.h"
#include <vector>

namespace test {

	class TestModel: public Test
	{
	public:
		TestModel();
		~TestModel();

		void OnUpdate(float delaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		Camera m_Camera;
		Model m_Model;
		Shader m_shader;
	};

}