#include "TestCamera.h"

#include "Renderer.h"
#include "imgui/imgui.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "VertexBufferLayout.h"

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

namespace test{

	TestCamera::TestCamera()
		:m_forward(glm::vec3(0.0f, 0.0f, -1.0f)),
		m_position(glm::vec3(0.0f, 0.0f, 1.0f)),
		m_up(glm::vec3(0.0f, 1.0f, 0.0f)),
		m_projection(glm::perspective(45.0f, 1.3f, 0.1f, 100.0f)),
		m_Yaw(0),
		m_Pitch(0)
	{

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

		m_forward.y = glm::sin(glm::radians(m_Pitch));
		m_forward.z = -(glm::cos(glm::radians(m_Pitch)) * glm::cos(glm::radians(m_Yaw)));
		m_forward.x = glm::cos(glm::radians(m_Pitch)) * glm::sin(glm::radians(m_Yaw));

		glm::mat4 view = glm::lookAt(m_position, m_position + m_forward, m_up);

		for (int i = 0; i < 10; i++)
		{
			float angle = i * 20.0f;
			glm::mat4 model(1.0f);
			model = glm::translate(model, cubePositions[i]);
			model = glm::rotate(model, angle, glm::vec3(1.0f, 0.3f, 0.5f));

			glm::mat4 mvp = m_projection * view * model;
			renderer.Draw(*m_VAO, *m_Shader, 36);
			m_Shader->SetUniformMatrix4fv("u_mvp", mvp);
		}

	}

	void TestCamera::OnImGuiRender()
	{
		ImGui::SliderFloat("horizontial", &m_position.x, -10.0f, 10.0f);
		ImGui::SliderFloat("vertical", &m_position.y, -10.0f, 10.0f);
		ImGui::SliderFloat("forward", &m_position.z, -10.0f, 10.0f);
		ImGui::SliderFloat("Yaw", &m_Yaw, -90.0f, 90.0f);
		ImGui::SliderFloat("Pitch", &m_Pitch, -90.0f, 90.0f);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

	}

}
