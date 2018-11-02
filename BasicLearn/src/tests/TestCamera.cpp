#include "TestCamera.h"

#include "VertexBufferLayout.h"
#include "Renderer.h"

#include "imgui/imgui.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const float vertices[] = {
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
			 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

			-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

const glm::vec3 cubePositions[] = {
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

float yaw, pitch;

namespace test{

	TestCamera::TestCamera()
	{
		m_Camera = new Camera();
		m_VAO = std::make_unique<VertexArray>();
		m_Shader = std::make_unique<Shader>("res/shader/Basic.shader");
		m_VBO = std::make_unique<VertexBuffer>(vertices, sizeof(vertices));
		VertexBufferLayout layout;
		layout.Push<float>(3);
		layout.Push<float>(2);
		m_VAO->AddBuffer(*m_VBO, layout);

		m_Shader->Bind();

		m_Textures.push_back(std::make_unique<Texture>("res/image/wall.jpg"));
		m_Textures.push_back(std::make_unique<Texture>("res/image/awesomeface.png"));

		for (int i = 0; i < m_Textures.size(); i++)
		{
			m_Textures[i]->Bind(i);
		}

		m_Shader->SetUniform1i("u_Texture0", 0);
		m_Shader->SetUniform1i("u_Texture1", 1);


	}

	TestCamera::~TestCamera()
	{}

	void TestCamera::OnUpdate(float delaTime)
	{}

	void TestCamera::OnRender()
	{
		Renderer renderer;

		for (int i = 0; i < 10; i++)
		{
			float angle = i * 20.0f;
			glm::mat4 model(1.0f);
			model = glm::translate(model, cubePositions[i]);
			model = glm::rotate(model, angle, glm::vec3(1.0f, 0.3f, 0.5f));

			glm::mat4 mvp = m_Camera->GetViewMatrix() * model;
			renderer.Draw(*m_VAO, *m_Shader, 36);
			m_Shader->SetUniformMatrix4fv("u_mvp", mvp);
		}

	}

	void TestCamera::OnImGuiRender()
	{
		if (ImGui::Button("W"))
		{
			m_Camera->MoveCameraPosition(Camera_Movement::FORWARD, 1.0f);
		}
		if (ImGui::Button("S"))
		{
			m_Camera->MoveCameraPosition(Camera_Movement::BACKWARD, 1.0f);
		}
		if (ImGui::Button("A"))
		{
			m_Camera->MoveCameraPosition(Camera_Movement::LEFT, 1.0f);
		}
		if (ImGui::Button("D"))
		{
			m_Camera->MoveCameraPosition(Camera_Movement::RIGHT, 1.0f);
		}

		ImGui::SliderFloat("Yaw", &yaw, -90.0f, 90.0f);
		ImGui::SliderFloat("Pitch", &pitch, -90.0f, 90.0f);

		m_Camera->SetYaw(yaw);
		m_Camera->SetPitch(pitch);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

	}


}