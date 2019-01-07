#include "TestHDR.h"

#include "VertexBufferLayout.h"
#include "Renderer.h"
#include "Input.h"
#include "Window.h"

#include "imgui/imgui.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

namespace test{

	TestHDR::TestHDR() :m_Camera()
	{
		// tell GLFW to capture our mouse
		glfwSetInputMode(Window::window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		m_IsShowMouse = false;

		VertexBufferLayout PNTLayout;
		PNTLayout.Push<float>(3);
		PNTLayout.Push<float>(3);
		PNTLayout.Push<float>(2);

		VertexBufferLayout PTLayout;
		PTLayout.Push<float>(3);
		PTLayout.Push<float>(2);

		float CubeVertices[] = {
			// back face
			-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
			 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
			 1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
			 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
			-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
			-1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
			// front face
			-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
			 1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
			 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
			 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
			-1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
			-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
			// left face
			-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
			-1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
			-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
			-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
			-1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
			-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
			// right face
			 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
			 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
			 1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
			 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
			 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
			 1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
			// bottom face
			-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
			 1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
			 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
			 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
			-1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
			-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
			// top face
			-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
			 1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
			 1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
			 1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
			-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
			-1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left        
		};
		m_CubeVAO = std::make_unique<VertexArray>();
		m_CubeVBO = std::make_unique<VertexBuffer>(CubeVertices, sizeof(CubeVertices));
		m_CubeVAO->AddBuffer(*m_CubeVBO, PNTLayout);

		float planeVertices[] = {
			// positions            // normals         // texcoords
			 25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
			-25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
			-25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,

			 25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
			-25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,
			 25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,  25.0f, 25.0f
		};
		m_PlaneVAO = std::make_unique<VertexArray>();
		m_PlaneVBO = std::make_unique<VertexBuffer>(planeVertices, sizeof(planeVertices));
		m_PlaneVAO->AddBuffer(*m_PlaneVBO, PNTLayout);

		float quadVertices[] = {
			// positions        // texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		m_QuadVAO = std::make_unique<VertexArray>();
		m_QuadVBO = std::make_unique<VertexBuffer>(quadVertices, sizeof(quadVertices));
		m_QuadVAO->AddBuffer(*m_QuadVBO, PTLayout);

		m_HDRSceneShader = std::make_unique<Shader>("res/shader/HDRScene.shader");
		m_HDRQuadShader = std::make_unique<Shader>("res/shader/HDRQuad.shader");
		m_WoodTexture = std::make_unique<Texture2D>("res/image/wood.png");


		#pragma region lighting info
		// -------------
		// positions
		m_LightPositions.push_back(glm::vec3(0.0f, 0.0f, 49.5f)); // back light
		m_LightPositions.push_back(glm::vec3(-1.4f, -1.9f, 9.0f));
		m_LightPositions.push_back(glm::vec3(0.0f, -1.8f, 4.0f));
		m_LightPositions.push_back(glm::vec3(0.8f, -1.7f, 6.0f));
		// colors
		m_LightColors.push_back(glm::vec3(200.0f, 200.0f, 200.0f));
		m_LightColors.push_back(glm::vec3(0.1f, 0.0f, 0.0f));
		m_LightColors.push_back(glm::vec3(0.0f, 0.0f, 0.2f));
		m_LightColors.push_back(glm::vec3(0.0f, 0.1f, 0.0f));
		#pragma endregion

		#pragma region HDR fbo
		// generate framebuffer
		GLCall(glGenFramebuffers(1, &m_HDRfbo));

		// add a color buffer for the framebuffer
		GLCall(glGenTextures(1, &m_HDRColorBuffer));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_HDRColorBuffer));
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, 800, 600, 0, GL_RGBA, GL_FLOAT, NULL));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// add depth Render buffer object for the framebuffer
		GLCall(glGenRenderbuffers(1, &m_HDRDepthBuffer));
		GLCall(glBindRenderbuffer(GL_RENDERBUFFER, m_HDRDepthBuffer));
		GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 800, 600));

		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_HDRfbo));
		GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_HDRColorBuffer, 0));
		GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_HDRDepthBuffer));
		
		// check is the framebuffer complete
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			std::cout << "ERROR:: FRAMEBUFFER is NOT COMPLETE." << std::endl;
		}
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
		#pragma endregion
		

	}

	void TestHDR::OnRender()
	{
		glm::mat4 view = m_Camera.GetViewMatrix();
		glm::mat4 projection = m_Camera.GetProjectionMatrix();

		glBindFramebuffer(GL_FRAMEBUFFER, m_HDRfbo);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 25.0));
		model = glm::scale(model, glm::vec3(2.5f, 2.5f, 27.5f));

		m_WoodTexture->Bind(0);
		m_HDRSceneShader->Bind();
		m_HDRSceneShader->SetUniformMatrix4fv("model", model);
		m_HDRSceneShader->SetUniformMatrix4fv("view", view);
		m_HDRSceneShader->SetUniformMatrix4fv("projection", projection);
		m_HDRSceneShader->SetUniform1i("reverse_normals", true);
		for (int i = 0; i < m_LightColors.size(); i++)
		{
			m_HDRSceneShader->SetUniform3f("lights[" + std::to_string(i) + "].Position", m_LightPositions[i]);
			m_HDRSceneShader->SetUniform3f("lights[" + std::to_string(i) + "].Color", m_LightColors[i]);
		}
		m_HDRSceneShader->SetUniform3f("viewPos", m_Camera.GetPosition());
		m_CubeVAO->Bind();
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_HDRColorBuffer);
		m_HDRQuadShader->Bind();
		m_QuadVAO->Bind();
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	}

	void TestHDR::OnImGuiRender()
	{
	}
	
	TestHDR::~TestHDR()
	{}

	void TestHDR::OnUpdate(float deltaTime)
	{
		if (Input::currentKeys[KeyBoard::ESC])
		{
			if (m_IsShowMouse)
				glfwSetInputMode(Window::window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			else
				glfwSetInputMode(Window::window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			
			m_IsShowMouse = !m_IsShowMouse;
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