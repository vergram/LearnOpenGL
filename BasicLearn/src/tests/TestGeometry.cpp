#include "TestGeometry.h"

#include "VertexBufferLayout.h"
#include "Renderer.h"
#include "Input.h"
#include "Window.h"

#include "imgui/imgui.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>



namespace test{

	TestGeometry::TestGeometry():m_Camera()
	{
		// tell GLFW to capture our mouse
		glfwSetInputMode(Window::window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		float points[] = {
			-0.5f,  0.5f, 1.0f, 0.0f, 0.0f, // 左上
			 0.5f,  0.5f, 0.0f, 1.0f, 0.0f, // 右上
			 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // 右下
			-0.5f, -0.5f, 1.0f, 1.0f, 0.0f  // 左下
		};
		m_VAO = std::make_unique<VertexArray>();
		m_VBO = std::make_unique<VertexBuffer>(points, sizeof(points));
		VertexBufferLayout layout;
		layout.Push<float>(2);
		layout.Push<float>(3);
		m_VAO->AddBuffer(*m_VBO, layout);
		m_VAO->Bind();

		m_Shader = std::make_unique<Shader>("res/shader/GeoSingleColor.shader");
	}

	void TestGeometry::OnRender()
	{
		m_VAO->Bind();
		m_Shader->Bind();
		GLCall(glDrawArrays(GL_POINTS, 0, 4));
	}
	
	TestGeometry::~TestGeometry()
	{}

	void TestGeometry::OnUpdate(float deltaTime)
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