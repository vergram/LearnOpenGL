#pragma once

#include "Test.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"
#include "Camera.h"

#include <vector>

namespace test {

	class TestCamera: public Test
	{
	public:
		TestCamera();
		~TestCamera();

		void OnUpdate(float delaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		std::unique_ptr<VertexBuffer> m_VBO;
		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr<Shader> m_Shader;
		std::vector<std::unique_ptr<Texture>> m_Textures;

		Camera* m_Camera;

	};

}