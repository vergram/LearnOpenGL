#pragma once

#include "Test.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture2D.h"
#include "Camera.h"

#include <vector>

namespace test {

	class TestAdvanceLighting: public Test
	{
	public:
		TestAdvanceLighting();
		~TestAdvanceLighting();

		void OnUpdate(float delaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		Camera m_Camera;
		std::unique_ptr<VertexBuffer> m_VBO;
		std::unique_ptr<VertexArray> m_VAO;

		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<Texture2D> m_woodTexture;
		std::unique_ptr<Texture2D> m_woodTextureWithGammaCorrecetion;

		bool m_blinn;
		bool m_IsShowMouse;
		bool m_isSRGB;
		bool m_isGammaCorrect;

	};

}