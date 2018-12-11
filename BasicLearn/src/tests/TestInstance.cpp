#include "TestInstance.h"

#include "VertexBufferLayout.h"
#include "Renderer.h"
#include "Input.h"
#include "Window.h"

#include "imgui/imgui.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <sstream>

namespace test{

	TestInstance::TestInstance():m_Camera()
	{
		// tell GLFW to capture our mouse
		glfwSetInputMode(Window::window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		
		float quadVertices[] = {
			// 位置          // 颜色
			-0.05f,  0.05f,  1.0f, 0.0f, 0.0f,
			 0.05f, -0.05f,  0.0f, 1.0f, 0.0f,
			-0.05f, -0.05f,  0.0f, 0.0f, 1.0f,

			-0.05f,  0.05f,  1.0f, 0.0f, 0.0f,
			 0.05f, -0.05f,  0.0f, 1.0f, 0.0f,
			 0.05f,  0.05f,  0.0f, 1.0f, 1.0f
		};

		m_QuadVBO = std::make_unique<VertexBuffer>(quadVertices, sizeof(quadVertices));
		m_VAO = std::make_unique<VertexArray>();
		VertexBufferLayout layout;
		layout.Push<float>(2);
		layout.Push<float>(3);
		m_VAO->AddBuffer(*m_QuadVBO, layout);

		m_ModelShader = std::make_unique<Shader>("res/shader/Instance.shader");
		
		glm::vec2 offsets[100];
		int index = 0;
		float offset = 0.1f;
		for (int y = -10; y < 10; y += 2)
		{
			for (int x = -10; x < 10; x += 2)
			{
				glm::vec2 transaltion;
				transaltion.x = (float)x / 10.0f + offset;
				transaltion.y = (float)y / 10.0f + offset;
				offsets[index++] = transaltion;
			}
		}

		unsigned int offsetBuffer;
		glGenBuffers(1, &offsetBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, offsetBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(offsets), offsets, GL_STATIC_DRAW);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);
		glVertexAttribDivisor(2, 1);        // 顶点属性在索引为 2 的位置，每 1 个实例更新一次该索引的顶点属性



		// uniform ways
		//m_ModelShader->Bind();
		//for (int i = 0; i < 100; i++)
		//{
		//	std::stringstream ss;
		//	std::string index;
		//	ss << i;
		//	index = ss.str();
		//	m_ModelShader->SetUniform2f(("offsets[" + index + "]"), offsets[i].x, offsets[i].y);
		//}
	}

	void TestInstance::OnRender()
	{
		m_ModelShader->Bind();
		m_VAO->Bind();
		GLCall(glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 100));
	}

	TestInstance::~TestInstance()
	{}

	void TestInstance::OnUpdate(float deltaTime)
	{
		if (Input::currentKeys[KeyBoard::ESC])
		{
			glfwSetInputMode(Window::window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			Input::currentKeys[KeyBoard::ESC] = 0;
		}
		if (Input::currentKeys[KeyBoard::W])
		{
			m_Camera.MoveCameraPosition(Camera_Movement::FORWARD, deltaTime);
			Input::currentKeys[KeyBoard::W] = 0;
		}
		if (Input::currentKeys[KeyBoard::A])
		{
			m_Camera.MoveCameraPosition(Camera_Movement::LEFT, deltaTime);
			Input::currentKeys[KeyBoard::A] = 0;
		}
		if (Input::currentKeys[KeyBoard::S])
		{
			m_Camera.MoveCameraPosition(Camera_Movement::BACKWARD, deltaTime);
			Input::currentKeys[KeyBoard::S] = 0;
		}
		if (Input::currentKeys[KeyBoard::D])
		{
			m_Camera.MoveCameraPosition(Camera_Movement::RIGHT, deltaTime);
			Input::currentKeys[KeyBoard::D] = 0;
		}
		m_Camera.MoveCameraDirection(Input::MouseMovementX, Input::MouseMovementY);
		Input::MouseMovementX = 0;
		Input::MouseMovementY = 0;
		m_Camera.ZoomCameraView(Input::MouseSrollOffset);
		Input::MouseSrollOffset = 0;
	}

}