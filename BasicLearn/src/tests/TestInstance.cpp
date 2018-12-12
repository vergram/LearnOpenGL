#include "TestInstance.h"

#include "VertexBufferLayout.h"
#include "Renderer.h"
#include "Input.h"
#include "Window.h"

#include "imgui/imgui.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <sstream>

namespace test{

	TestInstance::TestInstance():m_Camera()
	{
		// tell GLFW to capture our mouse
		glfwSetInputMode(Window::window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		
		m_Planet = std::make_unique<Model>("res/models/planet/planet.obj");
		m_Rock = std::make_unique<Model>("res/models/rock/rock.obj");

		m_PlanetShader = std::make_unique<Shader>("res/shader/Model.shader");
		m_InstanceRockShader = std::make_unique<Shader>("res/shader/InstanceModel.shader");

	
		#pragma region init model matrices
		m_ModelMatrixAmount = 10000;
		m_ModelMatrices = new glm::mat4[m_ModelMatrixAmount];
		srand(glfwGetTime());
		float radius = 50.0f;
		float offset = 5.0f;
		for (unsigned int i = 0; i < m_ModelMatrixAmount; i++)
		{
			glm::mat4 model;
			float angle = (float)i / (float)m_ModelMatrixAmount * 360.0f;

			// transaltion displacement = [-offset, offset]
			float displacement = (rand() % (int)(2 * offset * 100) / 100.0f - offset);
			float x = glm::sin(angle) * radius + displacement;
			displacement = (rand() % (int)(2 * offset * 100) / 100.0f - offset);
			float y = displacement * 0.4f;
			displacement = (rand() % (int)(2 * offset * 100) / 100.0f - offset);
			float z = glm::cos(angle) * radius + displacement;
			model = glm::translate(model, glm::vec3(x, y, z));

			// maybe scale first
			float rotAngle = (rand() % 360);
			model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

			// scale = [0.05f, 0.2f]
			float scale = (rand() % 20) / 100.0f + 0.05f;
			model = glm::scale(model, glm::vec3(scale));

			m_ModelMatrices[i] = model;
		}

		unsigned int MatriceBuffer;
		GLCall(glGenBuffers(1, &MatriceBuffer));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, MatriceBuffer));
		GLCall(glBufferData(GL_ARRAY_BUFFER, m_ModelMatrixAmount * sizeof(glm::mat4), &m_ModelMatrices[0], GL_STATIC_DRAW));
		int vec4Size = sizeof(glm::vec4);

		for (int i = 0; i < m_Rock->m_Meshs.size(); i++)
		{
			unsigned int vao = m_Rock->m_Meshs[i].GetVAO();
			GLCall(glBindVertexArray(vao));

			GLCall(glEnableVertexAttribArray(3));
			GLCall(glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)0));
			GLCall(glEnableVertexAttribArray(4));
			GLCall(glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)vec4Size));
			GLCall(glEnableVertexAttribArray(5));
			GLCall(glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(2 * vec4Size)));
			GLCall(glEnableVertexAttribArray(6));
			GLCall(glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(3 * vec4Size)));

			GLCall(glVertexAttribDivisor(3, 1));        // 顶点属性在索引为 3 的位置，每 1 个实例更新一次该索引的顶点属性
			GLCall(glVertexAttribDivisor(4, 1));
			GLCall(glVertexAttribDivisor(5, 1));
			GLCall(glVertexAttribDivisor(6, 1));

			GLCall(glBindVertexArray(0));
		}
		#pragma endregion
	}

	void TestInstance::OnRender()
	{
		glm::mat4 view = m_Camera.GetViewMatrix();
		glm::mat4 projection = m_Camera.GetProjectionMatrix();

		m_PlanetShader->Bind();
		m_PlanetShader->SetUniformMatrix4fv("view", view);
		m_PlanetShader->SetUniformMatrix4fv("projection", projection);

		glm::mat4 model;
		model = glm::translate(model, glm::vec3(0.0f, -3.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		m_PlanetShader->SetUniformMatrix4fv("model", model);
		m_Planet->Draw(*m_PlanetShader);

		m_InstanceRockShader->Bind();
		m_InstanceRockShader->SetUniformMatrix4fv("view", view);
		m_InstanceRockShader->SetUniformMatrix4fv("projection", projection);
		m_InstanceRockShader->SetUniform1i("texture_diffuse1", 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_Rock->m_Textures_loaded[0].id);
		for (unsigned int i = 0; i < m_Rock->m_Meshs.size(); i++)
		{
			glBindVertexArray(m_Rock->m_Meshs[i].GetVAO());
			glDrawElementsInstanced(GL_TRIANGLES, m_Rock->m_Meshs[i].indices.size(), GL_UNSIGNED_INT, 0, m_ModelMatrixAmount);
			glBindVertexArray(0);
		}

	}

	void TestInstance::OnImGuiRender()
	{
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}

	TestInstance::~TestInstance()
	{}

	void TestInstance::OnUpdate(float deltaTime)
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