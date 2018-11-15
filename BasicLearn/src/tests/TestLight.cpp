#include "TestLight.h"

#include "Window.h"
#include "Renderer.h"
#include "imgui/imgui.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "VertexBufferLayout.h"

static glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

static const float vertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	-0.5f, -0.5f,  0.5f,   0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,   0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,   0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,   0.0f,  0.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,   0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,   0.0f,  0.0f, 1.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};

namespace test{

	TestLight::TestLight():m_Camera()
	{
		// tell GLFW to capture our mouse
		glfwSetInputMode(Window::window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		m_VBO = std::make_unique<VertexBuffer>(vertices, sizeof(vertices));
		VertexBufferLayout layout;
		layout.Push<float>(3);
		layout.Push<float>(3);
		m_LightVAO = std::make_unique<VertexArray>();
		m_LightShader = std::make_unique<Shader>("res/shader/Light.shader");

		m_LightVAO->AddBuffer(*m_VBO, layout);
		m_LightShader->Bind();

		m_CubeVAO = std::make_unique<VertexArray>();
		m_CubeShader = std::make_unique<Shader>("res/shader/Cube.shader");
		
		m_CubeVAO->AddBuffer(*m_VBO, layout);
		m_CubeShader->Bind();

	}

	TestLight::~TestLight()
	{}

	void TestLight::OnUpdate(float deltaTime)
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

	void TestLight::OnRender()
	{
		Renderer renderer;

		glm::mat4 light_model(1.0f);
		light_model = glm::translate(light_model, lightPos);
		light_model = glm::scale(light_model, glm::vec3(0.2f));

		glm::mat4 mvp_Light{ m_Camera.GetProjectViewMatrix() * light_model };
		glm::mat4 cube_model(1.0f);

		glm::vec3 lightColor;
		lightColor.x = glm::sin(glfwGetTime() * 2.0f);
		lightColor.y = glm::sin(glfwGetTime() * 0.7f);
		lightColor.z = glm::sin(glfwGetTime() * 1.3f);

		glm::vec3 ambientColor = 0.2f * lightColor;
		glm::vec3 diffuseColor = 0.5f * lightColor;

		renderer.Draw(*m_CubeVAO, *m_CubeShader, 36);
		m_CubeShader->SetUniformMatrix4fv("u_Model", cube_model);
		m_CubeShader->SetUniformMatrix4fv("u_ViewProjection", m_Camera.GetProjectViewMatrix());
		m_CubeShader->SetUniform3f("viewPos", m_Camera.GetPosition());
		m_CubeShader->SetUniform3f("light.position", lightPos.x, lightPos.y, lightPos.z);
		m_CubeShader->SetUniform3f("light.ambient", 0.1f, 0.1f, 0.1f);
		m_CubeShader->SetUniform3f("light.diffuse", 0.5f, 0.5f, 0.5f);
		m_CubeShader->SetUniform3f("light.specular", 1.0f, 1.0f, 1.0f);
		m_CubeShader->SetUniform3f("material.ambient", ambientColor);
		m_CubeShader->SetUniform3f("material.diffuse", diffuseColor);
		m_CubeShader->SetUniform3f("material.specular", 0.5f, 0.5f, 0.5f);
		m_CubeShader->SetUniform1f("material.shininess", 32.0f);

		renderer.Draw(*m_LightVAO, *m_LightShader, 36);
		m_LightShader->SetUniformMatrix4fv("u_mvp", mvp_Light);
	}

	void TestLight::OnImGuiRender()
	{

	}

}
