#include "TestCamera.h"

#include "Renderer.h"
#include "imgui/imgui.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "VertexBufferLayout.h"

namespace test{

	TestCamera::TestCamera()
		:m_target(glm::vec3(0.0f, 0.0f, 0.0f)),
		m_position(glm::vec3(0.0f, 3.0f, 3.0f)),
		m_up(glm::vec3(0.0f, 1.0f, 0.0f)),
		m_projection(glm::perspective(45.0f, 1.3f, 0.1f, 100.0f)),
		m_radius(3.0f),
		m_rotation(0)
	{
		float vertices[] = {
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
		m_position.x = m_radius * glm::sin(glm::radians(m_rotation));
		m_position.z = m_radius * glm::cos(glm::radians(m_rotation));

		glm::mat4 camera = glm::lookAt(m_position, m_target, m_up);
		glm::mat4 mvp = m_projection * camera;

		renderer.Draw(*m_VAO, *m_Shader, 36);

		m_Shader->SetUniformMatrix4fv("u_mvp", mvp);
	}

	void TestCamera::OnImGuiRender()
	{
		ImGui::DragFloat("roation", &m_rotation);
		ImGui::DragFloat("radius", &m_radius);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

	}

}
