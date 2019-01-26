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
		m_blinn = true;
		m_IsShowMouse = false;
		m_isSRGB = true;
		m_isGammaCorrect = true;

		float planeVertices[] = {
			// positions            // normals         // texcoords
			 10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,  10.0f,  0.0f,
			-10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
			-10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,   0.0f, 10.0f,

			 10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,  10.0f,  0.0f,
			-10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,   0.0f, 10.0f,
			 10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,  10.0f, 10.0f
		};
		m_Shader = std::make_unique<Shader>("res/shader/blinnPhong/AdvanceLighting_Blinn_Phong.shader");
		m_woodTexture = std::make_unique<Texture2D>("res/image/wood.png");
		m_woodTextureWithGammaCorrecetion = std::make_unique<Texture2D>("res/image/wood.png", true); // gamma corrected

		m_VAO = std::make_unique<VertexArray>();
		m_VBO = std::make_unique<VertexBuffer>(planeVertices, sizeof(planeVertices));
		VertexBufferLayout layout;
		layout.Push<float>(3);
		layout.Push<float>(3);
		layout.Push<float>(2);
		m_VAO->AddBuffer(*m_VBO, layout);

		// lighting info
		// -------------
		glm::vec3 lightPositions[] = {
			glm::vec3(-3.0f, 0.0f, 0.0f),
			glm::vec3(-1.0f, 0.0f, 0.0f),
			glm::vec3(1.0f, 0.0f, 0.0f),
			glm::vec3(3.0f, 0.0f, 0.0f)
		};
		glm::vec3 lightColors[] = {
			glm::vec3(0.25),
			glm::vec3(0.50),
			glm::vec3(0.75),
			glm::vec3(1.00)
		};
		m_Shader->Bind();
		glUniform3fv(glGetUniformLocation(m_Shader->GetID(), "lightPositions"), 4, &lightPositions[0][0]);
		glUniform3fv(glGetUniformLocation(m_Shader->GetID(), "lightColors"), 4, &lightColors[0][0]);

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
		m_Shader->SetUniform1f("gamma", m_isGammaCorrect);
		if (m_isSRGB)
		{
			m_woodTextureWithGammaCorrecetion->Bind(0);
		}
		else
		{
			m_woodTexture->Bind(0);
		}
		renderer.Draw(*m_VAO, *m_Shader, 6);
	}

	void TestAdvanceLighting::OnImGuiRender()
	{
		ImGui::Checkbox("blinn", &m_blinn);
		ImGui::Checkbox("SRGB", &m_isSRGB);
		ImGui::Checkbox("GammaCorrection", &m_isGammaCorrect);

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