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

		m_Nanosuit = std::make_unique<Model>("res/models/nanosuit/nanosuit.obj");
		m_ModelShader = std::make_unique<Shader>("res/shader/Model.shader");
		m_NormalVisualizeShader = std::make_unique<Shader>("res/shader/geometryShader/GeoNormalVisualize.shader");
	}

	void TestGeometry::OnRender()
	{
		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		glm::mat4 view = m_Camera.GetViewMatrix();
		glm::mat4 projection = m_Camera.GetProjectionMatrix();

		#pragma region model 
		m_ModelShader->Bind();
		m_ModelShader->SetUniformMatrix4fv("model", model);
		m_ModelShader->SetUniformMatrix4fv("view", view);
		m_ModelShader->SetUniformMatrix4fv("projection", projection);

		m_Nanosuit->Draw(*m_ModelShader);
		#pragma endregion 

		m_NormalVisualizeShader->Bind();
		m_NormalVisualizeShader->SetUniformMatrix4fv("model", model);
		m_NormalVisualizeShader->SetUniformMatrix4fv("view", view);
		m_NormalVisualizeShader->SetUniformMatrix4fv("projection", projection);

		m_Nanosuit->Draw(*m_NormalVisualizeShader);
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