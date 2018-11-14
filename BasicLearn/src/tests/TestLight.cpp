#include "TestLight.h"

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

	TestLight::TestLight()
	{
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

	void TestLight::OnUpdate(float delaTime)
	{}

	void TestLight::OnRender()
	{
		Renderer renderer;

		glm::mat4 light_model(1.0f);
		light_model = glm::translate(light_model, lightPos);
		light_model = glm::scale(light_model, glm::vec3(0.2f));

		glm::mat4 mvp_Light{ m_Camera.GetProjectViewMatrix() * light_model };
		glm::mat4 cube_model(1.0f);

		renderer.Draw(*m_CubeVAO, *m_CubeShader, 36);
		m_CubeShader->SetUniform3f("viewPos", m_Camera.GetPosition());
		m_CubeShader->SetUniform3f("lightPos", lightPos.x, lightPos.y, lightPos.z);
		m_CubeShader->SetUniformMatrix4fv("u_Model", cube_model);
		m_CubeShader->SetUniformMatrix4fv("u_ViewProjection", m_Camera.GetProjectViewMatrix());
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
