#include "TestAdvanceLighting.h"

#include "VertexBufferLayout.h"
#include "Renderer.h"
#include "Input.h"
#include "Window.h"

#include "imgui/imgui.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


namespace test{

	TestAdvanceLighting::TestAdvanceLighting():m_Camera()
	{
		// tell GLFW to capture our mouse
		glfwSetInputMode(Window::window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		m_IsShowMouse = false;
		
		float planeVertices[] = {
			// positions            // normals         // texcoords
			 10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,  10.0f,  0.0f,
			-10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
			-10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,   0.0f, 10.0f,

			 10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,  10.0f,  0.0f,
			-10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,   0.0f, 10.0f,
			 10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,  10.0f, 10.0f
		};
		m_Shader = std::make_unique<Shader>("res/shader/AdvanceLighting_Blinn_Phong.shader");
		m_woodTexture = std::make_unique<Texture2D>("res/image/wood.png");
		m_VAO = std::make_unique<VertexArray>();
		m_VBO = std::make_unique<VertexBuffer>(planeVertices, sizeof(planeVertices));
		VertexBufferLayout layout;
		layout.Push<float>(3);
		layout.Push<float>(3);
		layout.Push<float>(2);
		m_VAO->AddBuffer(*m_VBO, layout);

	}

	void TestAdvanceLighting::OnRender()
	{
		Renderer renderer;

		m_Shader->Bind();
		m_Shader->SetUniformMatrix4fv("model", glm::mat4(1.0f));
		m_Shader->SetUniformMatrix4fv("view", m_Camera.GetViewMatrix());
		m_Shader->SetUniformMatrix4fv("projection", m_Camera.GetProjectionMatrix());
		m_Shader->SetUniform3f("viewPos", m_Camera.GetPosition());
		m_Shader->SetUniform3f("lightPos", glm::vec3(0.0f));
		m_Shader->SetUniform1i("blinn", m_blinn);
		m_woodTexture->Bind(0);
		renderer.Draw(*m_VAO, *m_Shader, 6);
	}

	void TestAdvanceLighting::OnImGuiRender()
	{
		ImGui::Checkbox("blinn", &m_blinn);
	}
	
	TestAdvanceLighting::~TestAdvanceLighting()
	{}

	void TestAdvanceLighting::OnUpdate(float deltaTime)
	{
		if (Input::currentKeys[KeyBoard::ESC])
		{
			if (m_IsShowMouse)
				glfwSetInputMode(Window::window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			else
				glfwSetInputMode(Window::window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			
			m_IsShowMouse = !m_IsShowMouse;
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