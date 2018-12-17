#include "TestAdvanceGLSL.h"

#include "VertexBufferLayout.h"
#include "Renderer.h"
#include "Input.h"
#include "Window.h"

#include "imgui/imgui.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>



namespace test{

	TestAdvanceGLSL::TestAdvanceGLSL():m_Camera()
	{
		// tell GLFW to capture our mouse
		glfwSetInputMode(Window::window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		m_ShaderR = std::make_unique<Shader>("res/shader/UboSingleColorR.shader");
		m_ShaderG = std::make_unique<Shader>("res/shader/UboSingleColorG.shader");
		m_ShaderB = std::make_unique<Shader>("res/shader/UboSingleColorB.shader");
		m_ShaderY = std::make_unique<Shader>("res/shader/UboSingleColorY.shader");

		float vertices[] = {
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
		m_VAO = std::make_unique<VertexArray>();
		m_VBO = std::make_unique<VertexBuffer>(vertices, sizeof(vertices));
		VertexBufferLayout layout;
		layout.Push<float>(3);
		m_VAO->AddBuffer(*m_VBO, layout);
		m_VAO->Bind();

		#pragma region initilize uniform buffer object
		glGenBuffers(1, &ubo);
		glBindBuffer(GL_UNIFORM_BUFFER, ubo);
		glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
		glBindBufferRange(GL_UNIFORM_BUFFER, 0, ubo, 0, 2 * sizeof(glm::mat4));
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(m_Camera.GetViewMatrix()));
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(m_Camera.GetProjectionMatrix()));
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		unsigned int matricesIndex = glGetUniformBlockIndex(m_ShaderR->GetID(), "Matrices");
		glUniformBlockBinding(m_ShaderR->GetID(), matricesIndex, 0);
		matricesIndex = glGetUniformBlockIndex(m_ShaderG->GetID(), "Matrices");
		glUniformBlockBinding(m_ShaderG->GetID(), matricesIndex, 0);
		matricesIndex = glGetUniformBlockIndex(m_ShaderB->GetID(), "Matrices");
		glUniformBlockBinding(m_ShaderB->GetID(), matricesIndex, 0);
		matricesIndex = glGetUniformBlockIndex(m_ShaderY->GetID(), "Matrices");
		glUniformBlockBinding(m_ShaderY->GetID(), matricesIndex, 0);
		#pragma endregion
	}

	void TestAdvanceGLSL::OnRender()
	{
		Renderer renderer;
		glm::vec3 cubePositions[] = {
			glm::vec3(-0.75f,  0.75f, -5.0f),
			glm::vec3( 0.75f,  0.75f, -5.0f),
			glm::vec3(-0.75f, -0.75f, -5.0f),
			glm::vec3( 0.75f, -0.75f, -5.0f)
		};
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));       // 设置清空屏幕所用的颜色
		GLCall(glClear(GL_COLOR_BUFFER_BIT));               // 清空颜色

		glm::mat4 model(1.0f);
		model = glm::translate(model, cubePositions[0]);
		m_ShaderR->Bind();
		m_ShaderR->SetUniformMatrix4fv("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glm::mat4 model1(1.0f);
		model1 = glm::translate(model1, cubePositions[1]);
		m_ShaderG->Bind();
		m_ShaderG->SetUniformMatrix4fv("model", model1);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glm::mat4 model2(1.0f);
		model2 = glm::translate(model2, cubePositions[2]);
		m_ShaderB->Bind();
		m_ShaderB->SetUniformMatrix4fv("model", model2);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glm::mat4 model3(1.0f);
		model3 = glm::translate(model3, cubePositions[3]);
		m_ShaderY->Bind();
		m_ShaderY->SetUniformMatrix4fv("model", model3);
		glDrawArrays(GL_TRIANGLES, 0, 36);

	}
	
	TestAdvanceGLSL::~TestAdvanceGLSL()
	{}

	void TestAdvanceGLSL::OnUpdate(float deltaTime)
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