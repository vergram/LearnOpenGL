#include "TestLight.h"

#include "Window.h"
#include "Renderer.h"
#include "Input.h"
#include "imgui/imgui.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "VertexBufferLayout.h"

static const glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

static const float vertices[] = {
	// positions          // normals           // texture coords
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,   0.0f,  0.0f, 1.0f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,   0.0f,  0.0f, 1.0f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,   0.0f,  0.0f, 1.0f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,   0.0f,  0.0f, 1.0f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,   0.0f,  0.0f, 1.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,   0.0f,  0.0f, 1.0f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
};

static const glm::vec3 cubePositions[] = {
			glm::vec3(0.0f,  0.0f,  0.0f),
			glm::vec3(2.0f,  5.0f, -15.0f),
			glm::vec3(-1.5f, -2.2f, -2.5f),
			glm::vec3(-3.8f, -2.0f, -12.3f),
			glm::vec3(2.4f, -0.4f, -3.5f),
			glm::vec3(-1.7f,  3.0f, -7.5f),
			glm::vec3(1.3f, -2.0f, -2.5f),
			glm::vec3(1.5f,  2.0f, -2.5f),
			glm::vec3(1.5f,  0.2f, -1.5f),
			glm::vec3(-1.3f,  1.0f, -1.5f)
};

namespace test{

	TestLight::TestLight()
		:m_Camera(),
		m_WoodTexture("res/image/container2.png"),
		m_SteelTexture("res/image/container2_specular.png"), 
		m_EmissionTexture("res/image/matrix.jpg")
	{
		// tell GLFW to capture our mouse
		glfwSetInputMode(Window::window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		m_VBO = std::make_unique<VertexBuffer>(vertices, sizeof(vertices));
		VertexBufferLayout layout;
		layout.Push<float>(3);
		layout.Push<float>(3);
		layout.Push<float>(2);
		m_LightVAO = std::make_unique<VertexArray>();
		m_LightShader = std::make_unique<Shader>("res/shader/Light.shader");

		m_LightVAO->AddBuffer(*m_VBO, layout);
		m_LightShader->Bind();

		m_CubeVAO = std::make_unique<VertexArray>();
		m_CubeShader = std::make_unique<Shader>("res/shader/Cube.shader");
		
		m_CubeVAO->AddBuffer(*m_VBO, layout);
		m_CubeShader->Bind();
		m_WoodTexture.Bind(0);
		m_SteelTexture.Bind(1);
		m_EmissionTexture.Bind(2);

		// set up unchanger uniforms
		m_CubeShader->SetUniform3f("light.ambient", 0.2f, 0.2f, 0.2f);
		m_CubeShader->SetUniform3f("light.diffuse", 0.5f, 0.5f, 0.5f);
		m_CubeShader->SetUniform3f("light.specular", 1.0f, 1.0f, 1.0f);
		//m_CubeShader->SetUniform3f("light.direction", -0.2f, -1.0f, -0.3f);                      // 方向光
		m_CubeShader->SetUniform3f("light.position", lightPos.x, lightPos.y, lightPos.z);        // 点光源
		m_CubeShader->SetUniform1f("light.constant", 1.0f);
		m_CubeShader->SetUniform1f("light.liner", 0.09f);
		m_CubeShader->SetUniform1f("light.quadratic", 0.032f);

		m_CubeShader->SetUniform1i("material.diffuse", 0);										 // 漫反射贴图
		m_CubeShader->SetUniform1i("material.specular", 1);										 // 镜面反射贴图
		//m_CubeShader->SetUniform1i("emission", 2);											   // 发光贴图
		m_CubeShader->SetUniform1f("material.shininess", 32.0f);
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

		// Exercise for Light
		//glm::vec3 ambientLight(1.0f, 1.0f, 1.0f);
		//glm::vec3 diffuseLight(1.0f, 1.0f, 1.0f);
		//glm::vec3 specularLight(1.0f, 1.0f, 1.0f);
		//ambientLight *= glm::sin(glfwGetTime() * 0.1f);
		//diffuseLight *= glm::sin(glfwGetTime() * 0.3f);
		//specularLight *= glm::sin(glfwGetTime() * 0.5f);

		for(int i = 0; i < 10; i++)
		{
			glm::mat4 cube_model(1.0f);
			cube_model = glm::translate(cube_model, cubePositions[i]);
			float angle = 20.0f * i;
			cube_model = glm::rotate(cube_model, angle, glm::vec3(1.0f, 0.3f, 0.5f));

			renderer.Draw(*m_CubeVAO, *m_CubeShader, 36);

			m_CubeShader->SetUniformMatrix4fv("u_ViewProjection", m_Camera.GetProjectViewMatrix());
			m_CubeShader->SetUniform3f("viewPos", m_Camera.GetPosition());
			m_CubeShader->SetUniformMatrix4fv("u_Model", cube_model);
		}

		// Lamp Object
		glm::mat4 light_model(1.0f);
		light_model = glm::translate(light_model, lightPos);
		light_model = glm::scale(light_model, glm::vec3(0.2f));
		glm::mat4 mvp_Light{ m_Camera.GetProjectViewMatrix() * light_model };

		renderer.Draw(*m_LightVAO, *m_LightShader, 36);
		m_LightShader->SetUniformMatrix4fv("u_mvp", mvp_Light);
	}

	void TestLight::OnImGuiRender()
	{
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}

}
