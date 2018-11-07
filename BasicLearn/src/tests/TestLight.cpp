#include "TestLight.h"

#include "Renderer.h"
#include "imgui/imgui.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "VertexBufferLayout.h"

static glm::vec3 lightPos(1.2f, 1.0f, -2.0f);

static const float vertices[] = {
			-0.5f, -0.5f, -0.5f,
			 0.5f, -0.5f, -0.5f,
			 0.5f,  0.5f, -0.5f,
			 0.5f,  0.5f, -0.5f,
			-0.5f,  0.5f, -0.5f,
			-0.5f, -0.5f, -0.5f,

			-0.5f, -0.5f,  0.5f,
			 0.5f, -0.5f,  0.5f,
			 0.5f,  0.5f,  0.5f,
			 0.5f,  0.5f,  0.5f,
			-0.5f,  0.5f,  0.5f,
			-0.5f, -0.5f,  0.5f,

			-0.5f,  0.5f,  0.5f,
			-0.5f,  0.5f, -0.5f,
			-0.5f, -0.5f, -0.5f,
			-0.5f, -0.5f, -0.5f,
			-0.5f, -0.5f,  0.5f,
			-0.5f,  0.5f,  0.5f,

		     0.5f,  0.5f,  0.5f,
		     0.5f,  0.5f, -0.5f,
		     0.5f, -0.5f, -0.5f,
		     0.5f, -0.5f, -0.5f,
		     0.5f, -0.5f,  0.5f,
		     0.5f,  0.5f,  0.5f,

			-0.5f, -0.5f, -0.5f,
			 0.5f, -0.5f, -0.5f,
			 0.5f, -0.5f,  0.5f,
			 0.5f, -0.5f,  0.5f,
			-0.5f, -0.5f,  0.5f,
			-0.5f, -0.5f, -0.5f,

			-0.5f,  0.5f, -0.5f,
			 0.5f,  0.5f, -0.5f,
			 0.5f,  0.5f,  0.5f,
			 0.5f,  0.5f,  0.5f,
			-0.5f,  0.5f,  0.5f,
			-0.5f,  0.5f, -0.5f,
		};

namespace test{

	TestLight::TestLight()
	{

		m_CubeVAO = std::make_unique<VertexArray>();
		m_CubeShader = std::make_unique<Shader>("res/shader/Cube.shader");
		m_CubeVBO = std::make_unique<VertexBuffer>(vertices, sizeof(vertices));
		VertexBufferLayout layout1;
		layout1.Push<float>(3);
		m_CubeVAO->AddBuffer(*m_CubeVBO, layout1);
		m_CubeShader->Bind();


		m_LightVAO = std::make_unique<VertexArray>();
		m_LightShader = std::make_unique<Shader>("res/shader/Light.shader");
		m_LightVBO = std::make_unique<VertexBuffer>(vertices, sizeof(vertices));
		VertexBufferLayout layout;
		layout.Push<float>(3);
		m_LightVAO->AddBuffer(*m_LightVBO, layout);
		m_LightShader->Bind();
	}

	TestLight::~TestLight()
	{}

	void TestLight::OnUpdate(float delaTime)
	{}

	void TestLight::OnRender()
	{
		Renderer renderer;

		glm::mat4 model(1.0f);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f));

		glm::mat4 mvp_Cube{ m_Camera.GetProjectViewMatrix() };
		glm::mat4 mvp_Light{ m_Camera.GetProjectViewMatrix() * model };

		renderer.Draw(*m_CubeVAO, *m_CubeShader, 36);
		m_CubeShader->SetUniformMatrix4fv("u_mvp", mvp_Cube);
		m_CubeShader->SetUniform3f("lightColor", 1.0f, 1.0f, 1.0f);
		m_CubeShader->SetUniform3f("objectColor", 1.0f, 0.5f, 0.31f);

		renderer.Draw(*m_LightVAO, *m_LightShader, 36);
		m_LightShader->SetUniformMatrix4fv("u_mvp", mvp_Light);
	}

	void TestLight::OnImGuiRender()
	{
		Test::OnImGuiRender();
	}

}
