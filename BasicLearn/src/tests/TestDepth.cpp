#include "TestDepth.h"

#include "VertexBufferLayout.h"
#include "Renderer.h"
#include "Input.h"
#include "Window.h"

#include "imgui/imgui.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace test{

	TestDepth::TestDepth()
		:m_Camera(),
		m_CubeTexture("res/image/marble.jpg"),
		m_PlaneTexture("res/image/metal.png")
	{
		// tell GLFW to capture our mouse
		glfwSetInputMode(Window::window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		GLCall(glDepthFunc(GL_LESS));

		float cubeVertices[] = {
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
		float planeVertices[] = {
			// positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
			 5.0f, -0.5f,  5.0f,  1.0f, 0.0f,
			-5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
			-5.0f, -0.5f, -5.0f,  0.0f, 1.0f,
					   
			 5.0f, -0.5f,  5.0f,  1.0f, 0.0f,
			-5.0f, -0.5f, -5.0f,  0.0f, 1.0f,
			 5.0f, -0.5f, -5.0f,  1.0f, 1.0f
		};

		m_SingleColorShader = std::make_unique<Shader>("res/shader/StencilTest.shader");
		m_Shader = std::make_unique<Shader>("res/shader/DepthTest.shader");

		m_CubeVAO = std::make_unique<VertexArray>();
		m_CubeVBO = std::make_unique<VertexBuffer>(cubeVertices, sizeof(cubeVertices));

		m_PlaneVAO = std::make_unique<VertexArray>();
		m_PlaneVBO = std::make_unique<VertexBuffer>(planeVertices, sizeof(planeVertices));

		VertexBufferLayout layout;
		layout.Push<float>(3);
		layout.Push<float>(2);
		m_CubeVAO->AddBuffer(*m_CubeVBO, layout);
		m_PlaneVAO->AddBuffer(*m_PlaneVBO, layout);

		m_Shader->Bind();
		m_Shader->SetUniform1i("texture0", 0);
	}

	void TestDepth::OnRender()
	{
		Renderer renderer;

		glm::mat4 view = m_Camera.GetViewMatrix();
		glm::mat4 projection = m_Camera.GetProjectionMatrix();

		m_Shader->Bind();
		m_Shader->SetUniformMatrix4fv("view", view);
		m_Shader->SetUniformMatrix4fv("projection", projection);
		
		GLCall(glEnable(GL_STENCIL_TEST));                           // enable stencil test
		GLCall(glStencilFunc(GL_ALWAYS, 1, 0xff));                   // set always pass stencil test
		GLCall(glStencilOp(GL_KEEP, GL_REPLACE, GL_REPLACE));        // if pass stencil test set stencil buffer to ref value
		GLCall(glStencilMask(0x00));                                 // do not allow writing the stencil buffer because we dont want the plane influence our cube outlining

		// draw plane
		m_PlaneTexture.Bind();
		m_Shader->SetUniformMatrix4fv("model", glm::mat4(1.0f));
		renderer.Draw(*m_PlaneVAO, *m_Shader, 6);
		 
		GLCall(glStencilFunc(GL_ALWAYS, 1, 0xff));                   // set every pixels in cube pass the stencil test
		GLCall(glStencilMask(0xff));                                 // enable writing to the stencil buffer

		m_CubeTexture.Bind();
		glm::mat4 cubeModel1(1.0f);
		cubeModel1 = glm::translate(cubeModel1, glm::vec3(-1.0f, 0.01f, -1.0f));
		m_Shader->SetUniformMatrix4fv("model", cubeModel1);
		renderer.Draw(*m_CubeVAO, *m_Shader, 36);

		glm::mat4 cubeModel2(1.0f);
		cubeModel2 = glm::translate(cubeModel2, glm::vec3(2.0f, 0.01f, 0.0f));
		m_Shader->SetUniformMatrix4fv("model", cubeModel2);
		renderer.Draw(*m_CubeVAO, *m_Shader, 36);

		GLCall(glStencilFunc(GL_NOTEQUAL, 1, 0xff));                 // all the pixels outside the original cube will pass the test 
		GLCall(glStencilMask(0x00));                                 // disable writing the stencil buffer
		GLCall(glDisable(GL_DEPTH_TEST));                            // we want the outlining wont cover by depth test

		m_SingleColorShader->Bind();
		m_SingleColorShader->SetUniformMatrix4fv("view", view);
		m_SingleColorShader->SetUniformMatrix4fv("projection", projection);
		
		m_CubeTexture.Bind();
		glm::mat4 cubeModel3(1.0f);
		cubeModel3 = glm::translate(cubeModel3, glm::vec3(-1.0f, 0.01f, -1.0f));
		cubeModel3 = glm::scale(cubeModel3, glm::vec3(1.1f, 1.1f, 1.1f));
		m_Shader->SetUniformMatrix4fv("model", cubeModel3);
		renderer.Draw(*m_CubeVAO, *m_SingleColorShader, 36);

		glm::mat4 cubeModel4(1.0f);
		cubeModel4 = glm::translate(cubeModel4, glm::vec3(2.0f, 0.01f, 0.0f));
		cubeModel4 = glm::scale(cubeModel4, glm::vec3(1.1f, 1.1f, 1.1f));
		m_Shader->SetUniformMatrix4fv("model", cubeModel4);
		renderer.Draw(*m_CubeVAO, *m_SingleColorShader, 36);

		GLCall(glEnable(GL_DEPTH_TEST));                             // reenable depth test
		GLCall(glStencilMask(0xff));                                 // for we need to clear stencil buffer every frame
		GLCall(glDisable(GL_STENCIL_TEST));                          // disable stencil test or imgui wont work fine
	}

	TestDepth::~TestDepth()
	{
	}

	void TestDepth::OnUpdate(float deltaTime)
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