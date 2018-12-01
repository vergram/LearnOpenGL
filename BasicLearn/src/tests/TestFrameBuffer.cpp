#include "TestFrameBuffer.h"

#include "VertexBufferLayout.h"
#include "Renderer.h"
#include "Input.h"
#include "Window.h"

#include "imgui/imgui.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <map>
#include <iostream>

namespace test {

	TestFrameBuffer::TestFrameBuffer()
		:m_Camera(),
		m_CubeTexture("res/image/marble.jpg"),
		m_PlaneTexture("res/image/metal.png"),
		m_TransparentTexture("res/image/window.png")
	{
		// tell GLFW to capture our mouse
		glfwSetInputMode(Window::window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
		GLCall(glDepthFunc(GL_LESS));
		
		float cubeVertices[] = {
			// Back face
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // Bottom-left
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
			 0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // bottom-right         
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // bottom-left
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
			// Front face
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
			 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
			 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
			-0.5f,  0.5f,  0.5f,  0.0f, 1.0f, // top-left
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
			// Left face
			-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
			-0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-left
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
			-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
			// Right face
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
			 0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right         
			 0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
			 0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left     
			// Bottom face
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
			 0.5f, -0.5f, -0.5f,  1.0f, 1.0f, // top-left
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
			// Top face
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right     
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
			-0.5f,  0.5f,  0.5f,  0.0f, 0.0f  // bottom-left        
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
		float grassVertices[] = {
			// positions         // texture Coords (swapped y coordinates because texture is flipped upside down)
			0.0f,  0.5f,  0.0f,  1.0f,  1.0f,
			0.0f, -0.5f,  0.0f,  1.0f,  0.0f,
			1.0f, -0.5f,  0.0f,  0.0f,  0.0f,

			0.0f,  0.5f,  0.0f,  1.0f,  1.0f,
			1.0f, -0.5f,  0.0f,  0.0f,  0.0f,
			1.0f,  0.5f,  0.0f,  0.0f,  1.0f
		};
		float quadVertices[] = {
			// positions   // texCoords
			-1.0f,  1.0f,  0.0f, 1.0f,
			-1.0f, -1.0f,  0.0f, 0.0f,
			 1.0f, -1.0f,  1.0f, 0.0f,

			-1.0f,  1.0f,  0.0f, 1.0f,
			 1.0f, -1.0f,  1.0f, 0.0f,
			 1.0f,  1.0f,  1.0f, 1.0f
		};

		#pragma region framebuffer init
		// generate framebuffer
		GLCall(glGenFramebuffers(1, &m_fbo));
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_fbo));

		// add a color buffer for the framebuffer
		GLCall(glGenTextures(1, &m_TexColorBuffer));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_TexColorBuffer));
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GLCall(glBindTexture(GL_TEXTURE_2D, 0));
		GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_TexColorBuffer, 0));

		// add depth and stencil Render buffer object for the framebuffer
		GLCall(glGenRenderbuffers(1, &m_rbo));
		GLCall(glBindRenderbuffer(GL_RENDERBUFFER, m_rbo));
		GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600));
		GLCall(glBindRenderbuffer(GL_RENDERBUFFER, 0));
		GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rbo));

		// check is the framebuffer complete
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			std::cout << "ERROR:: FRAMEBUFFER is NOT COMPLETE." << std::endl;
		}
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
		#pragma endregion

		m_QuadVAO = std::make_unique<VertexArray>();
		m_QuadVBO = std::make_unique<VertexBuffer>(quadVertices, sizeof(quadVertices));

		VertexBufferLayout texLayout;
		texLayout.Push<float>(2);
		texLayout.Push<float>(2);
		m_QuadVAO->AddBuffer(*m_QuadVBO, texLayout);

		m_SingleColorShader = std::make_unique<Shader>("res/shader/SingleColor.shader");
		m_SceneShader = std::make_unique<Shader>("res/shader/DepthTest.shader");
		m_TransparentShader = std::make_unique<Shader>("res/shader/Blend.shader");
		m_QuadShader = std::make_unique<Shader>("res/shader/Post_Processing.shader");

		m_CubeVAO = std::make_unique<VertexArray>();
		m_CubeVBO = std::make_unique<VertexBuffer>(cubeVertices, sizeof(cubeVertices));

		m_PlaneVAO = std::make_unique<VertexArray>();
		m_PlaneVBO = std::make_unique<VertexBuffer>(planeVertices, sizeof(planeVertices));
		
		m_GrassVAO = std::make_unique<VertexArray>();
		m_GrassVBO = std::make_unique<VertexBuffer>(grassVertices, sizeof(grassVertices));

		VertexBufferLayout layout;
		layout.Push<float>(3);
		layout.Push<float>(2);
		m_CubeVAO->AddBuffer(*m_CubeVBO, layout);
		m_PlaneVAO->AddBuffer(*m_PlaneVBO, layout);
		m_GrassVAO->AddBuffer(*m_GrassVBO, layout);

	}

	void TestFrameBuffer::OnRender()
	{
		Renderer renderer;

		glm::mat4 view = m_Camera.GetViewMatrix();
		glm::mat4 projection = m_Camera.GetProjectionMatrix();

		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_fbo));
		GLCall(glClearColor(0.1f, 0.1f, 0.1f, 0.1f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
		GLCall(glEnable(GL_DEPTH_TEST));

		#pragma region draw scene
		m_SceneShader->Bind();
		m_SceneShader->SetUniformMatrix4fv("view", view);
		m_SceneShader->SetUniformMatrix4fv("projection", projection);

		// draw plane
		m_PlaneTexture.Bind();
		m_SceneShader->SetUniformMatrix4fv("model", glm::mat4(1.0f));
		renderer.Draw(*m_PlaneVAO, *m_SceneShader, 6);

		// draw cube
		m_CubeTexture.Bind();
		glm::mat4 cubeModel1(1.0f);
		cubeModel1 = glm::translate(cubeModel1, glm::vec3(-1.0f, 0.01f, -1.0f));
		m_SceneShader->SetUniformMatrix4fv("model", cubeModel1);
		renderer.Draw(*m_CubeVAO, *m_SceneShader, 36);

		glm::mat4 cubeModel2(1.0f);
		cubeModel2 = glm::translate(cubeModel2, glm::vec3(2.0f, 0.01f, 0.0f));
		m_SceneShader->SetUniformMatrix4fv("model", cubeModel2);
		renderer.Draw(*m_CubeVAO, *m_SceneShader, 36);

		// draw transparent texture
		std::vector<glm::vec3> windows{
		glm::vec3(-1.5f, 0.0f, -0.48f),
		glm::vec3( 1.5f, 0.0f,  0.51f),
		glm::vec3( 0.0f, 0.0f,   0.7f),
		glm::vec3(-0.3f, 0.0f,  -2.3f),
		glm::vec3( 0.5f, 0.0f,  -0.6f)
		};
		std::map<float, glm::vec3> sorted;
		for (auto& pos : windows)
		{
			float distance = glm::length(pos - m_Camera.GetPosition());
			sorted[distance] = pos;
		}

		m_TransparentShader->Bind();
		m_TransparentTexture.Bind();
		m_TransparentShader->SetUniformMatrix4fv("view", view);
		m_TransparentShader->SetUniformMatrix4fv("projection", projection);

		for (auto it = sorted.rbegin(); it != sorted.rend(); it++)
		{
			m_TransparentShader->Bind();
			glm::mat4 model;
			model = glm::translate(model, it->second);
			m_TransparentShader->SetUniformMatrix4fv("model", model);
			renderer.Draw(*m_GrassVAO, *m_TransparentShader, 6);
		}
		#pragma endregion 

		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
		GLCall(glDisable(GL_DEPTH_TEST));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_TexColorBuffer));

		renderer.Draw(*m_QuadVAO, *m_QuadShader, 6);


	}

	TestFrameBuffer::~TestFrameBuffer()
	{}

	void TestFrameBuffer::OnUpdate(float deltaTime)
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