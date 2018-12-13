#include "TestLight.h"

#include "Window.h"
#include "Renderer.h"
#include "Input.h"
#include "imgui/imgui.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "VertexBufferLayout.h"


static const glm::vec3 cubePositions[] = {
	glm::vec3( 0.0f,  0.0f,   0.0f),
	glm::vec3( 2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f,  -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3( 2.4f, -0.4f,  -3.5f),
	glm::vec3(-1.7f,  3.0f,  -7.5f),
	glm::vec3( 1.3f, -2.0f,  -2.5f),
	glm::vec3( 1.5f,  2.0f,  -2.5f),
	glm::vec3( 1.5f,  0.2f,  -1.5f),
	glm::vec3(-1.3f,  1.0f,  -1.5f)
};

// positions of the point lights
static const glm::vec3 pointLightPositions[] = {
	glm::vec3( 0.7f,  0.2f,   2.0f),
	glm::vec3( 2.3f, -3.3f,  -4.0f),
	glm::vec3(-4.0f,  2.0f, -12.0f),
	glm::vec3( 0.0f,  0.0f,  -3.0f)
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

		float vertices[] = {
	// positions          // normals           // texture coords
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
};
		m_VBO = std::make_unique<VertexBuffer>(vertices, sizeof(vertices));
		VertexBufferLayout layout;
		layout.Push<float>(3);
		layout.Push<float>(3);
		layout.Push<float>(2);
		m_LampVAO = std::make_unique<VertexArray>();
		m_LampShader = std::make_unique<Shader>("res/shader/Lamp.shader");

		m_LampVAO->AddBuffer(*m_VBO, layout);
		m_LampShader->Bind();

		m_CubeVAO = std::make_unique<VertexArray>();
		m_CubeShader = std::make_unique<Shader>("res/shader/Lighting.shader");
		
		m_CubeVAO->AddBuffer(*m_VBO, layout);
		m_CubeShader->Bind();
		m_WoodTexture.Bind(0);
		m_SteelTexture.Bind(1);
		m_EmissionTexture.Bind(2);

		#pragma region unchange uniforms
		// directional light
		m_CubeShader->SetUniform3f("dirLight.direction", -0.2f, -1.0f, -0.3f);
		m_CubeShader->SetUniform3f("dirLight.ambient", 0.3f, 0.3f, 0.3f);
		m_CubeShader->SetUniform3f("dirLight.diffuse", 0.6f, 0.6f, 0.6f);
		m_CubeShader->SetUniform3f("dirLight.specular", 0.5f, 0.5f, 0.5f);
		// point light 1
		m_CubeShader->SetUniform3f("pointLights[0].position", pointLightPositions[0]);
		m_CubeShader->SetUniform3f("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
		m_CubeShader->SetUniform3f("pointLights[0].diffuse", 0.5f, 0.5f, 0.5f);
		m_CubeShader->SetUniform3f("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
		m_CubeShader->SetUniform1f("pointLights[0].constant", 1.0f);
		m_CubeShader->SetUniform1f("pointLights[0].linear", 0.09);
		m_CubeShader->SetUniform1f("pointLights[0].quadratic", 0.032);
		// point light 2
		m_CubeShader->SetUniform3f("pointLights[1].position", pointLightPositions[1]);
		m_CubeShader->SetUniform3f("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
		m_CubeShader->SetUniform3f("pointLights[1].diffuse", 0.5f, 0.5f, 0.5f);
		m_CubeShader->SetUniform3f("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
		m_CubeShader->SetUniform1f("pointLights[1].constant", 1.0f);
		m_CubeShader->SetUniform1f("pointLights[1].linear", 0.09);
		m_CubeShader->SetUniform1f("pointLights[1].quadratic", 0.032);
		// point light 3
		m_CubeShader->SetUniform3f("pointLights[2].position", pointLightPositions[2]);
		m_CubeShader->SetUniform3f("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
		m_CubeShader->SetUniform3f("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
		m_CubeShader->SetUniform3f("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
		m_CubeShader->SetUniform1f("pointLights[2].constant", 1.0f);
		m_CubeShader->SetUniform1f("pointLights[2].linear", 0.09);
		m_CubeShader->SetUniform1f("pointLights[2].quadratic", 0.032);
		// point light 4
		m_CubeShader->SetUniform3f("pointLights[3].position", pointLightPositions[3]);
		m_CubeShader->SetUniform3f("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
		m_CubeShader->SetUniform3f("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
		m_CubeShader->SetUniform3f("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
		m_CubeShader->SetUniform1f("pointLights[3].constant", 1.0f);
		m_CubeShader->SetUniform1f("pointLights[3].linear", 0.09);
		m_CubeShader->SetUniform1f("pointLights[3].quadratic", 0.032);

		m_CubeShader->SetUniform1i("material.diffuse", 0);										 // Âþ·´ÉäÌùÍ¼
		m_CubeShader->SetUniform1i("material.specular", 1);										 // ¾µÃæ·´ÉäÌùÍ¼
		//m_CubeShader->SetUniform1i("emission", 2);											   // ·¢¹âÌùÍ¼
		m_CubeShader->SetUniform1f("material.shininess", 32.0f);
		#pragma endregion
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
			renderer.Draw(*m_CubeVAO, *m_CubeShader, 36);

			glm::mat4 cube_model(1.0f);
			cube_model = glm::translate(cube_model, cubePositions[i]);
			float angle = 20.0f * i;
			cube_model = glm::rotate(cube_model, angle, glm::vec3(1.0f, 0.3f, 0.5f));

			m_CubeShader->SetUniformMatrix4fv("u_Model", cube_model);
			m_CubeShader->SetUniform3f("viewPos", m_Camera.GetPosition());
		}
		m_CubeShader->SetUniformMatrix4fv("u_ViewProjection", m_Camera.GetProjectViewMatrix());

		// spotLight
		m_CubeShader->SetUniform3f("spotLight.position", m_Camera.GetPosition());
		m_CubeShader->SetUniform3f("spotLight.direction", m_Camera.GetForward());
		m_CubeShader->SetUniform3f("spotLight.ambient", 0.0f, 0.0f, 0.0f);
		m_CubeShader->SetUniform3f("spotLight.diffuse", 0.0f, 1.0f, 0.0f);
		m_CubeShader->SetUniform3f("spotLight.specular", 0.0f, 1.0f, 0.0f);
		m_CubeShader->SetUniform1f("spotLight.constant", 1.0f);
		m_CubeShader->SetUniform1f("spotLight.linear", 0.09);
		m_CubeShader->SetUniform1f("spotLight.quadratic", 0.032);
		m_CubeShader->SetUniform1f("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
		m_CubeShader->SetUniform1f("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

		// Lamp Object
		for (int i = 0; i < 4; i++)
		{
			renderer.Draw(*m_LampVAO, *m_LampShader, 36);

			glm::mat4 light_model(1.0f);
			light_model = glm::translate(light_model, pointLightPositions[i]);
			light_model = glm::scale(light_model, glm::vec3(0.2f));

			m_LampShader->SetUniformMatrix4fv("u_Model", light_model);
		}
		m_LampShader->SetUniformMatrix4fv("u_ViewProjection", m_Camera.GetProjectViewMatrix());
	}

	void TestLight::OnImGuiRender()
	{
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
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

}
