#include "TestBloom.h"

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

	TestBloom::TestBloom() :m_Camera(), m_Exposure(1.0f)
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

		m_HDRSceneShader = std::make_unique<Shader>("res/shader/bloom/BloomScene.shader");
		m_HDRQuadShader = std::make_unique<Shader>("res/shader/bloom/BloomQuad.shader");
		m_LightCubeShader = std::make_unique<Shader>("res/shader/bloom/BloomLightCube.shader");
		m_GaussBlurShader = std::make_unique<Shader>("res/shader/bloom/BloomGaussBlur.shader");

		m_WoodTexture = std::make_unique<Texture2D>("res/image/wood.png", true); // loading texture as SRGB format
		m_ContainerTexture = std::make_unique<Texture2D>("res/image/container2.png", true); // loading texture as SRGB format


		#pragma region lighting info
		// -------------
		// positions
		m_LightPositions.push_back(glm::vec3(0.0f, 0.5f, 1.5f));
		m_LightPositions.push_back(glm::vec3(-4.0f, 0.5f, -3.0f));
		m_LightPositions.push_back(glm::vec3(3.0f, 0.5f, 1.0f));
		m_LightPositions.push_back(glm::vec3(-.8f, 2.4f, -1.0f));
		
		// colors
		m_LightColors.push_back(glm::vec3(2.0f, 2.0f, 2.0f));
		m_LightColors.push_back(glm::vec3(1.5f, 0.0f, 0.0f));
		m_LightColors.push_back(glm::vec3(0.0f, 0.0f, 1.5f));
		m_LightColors.push_back(glm::vec3(0.0f, 1.5f, 0.0f));
		#pragma endregion

		#pragma region HDR fbo
		// generate framebuffer
		GLCall(glGenFramebuffers(1, &m_HDRfbo));
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_HDRfbo));

		// add a color buffer for the framebuffer
		GLCall(glGenTextures(2, m_HDRColorBuffers));
		for (unsigned int i = 0; i < 2; i++)
		{
			GLCall(glBindTexture(GL_TEXTURE_2D, m_HDRColorBuffers[i]));
			GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, 800, 600, 0, GL_RGB, GL_FLOAT, NULL));
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

			GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, m_HDRColorBuffers[i], 0));
		}

		// add depth Render buffer object for the framebuffer
		GLCall(glGenRenderbuffers(1, &m_HDRDepthBuffer));
		GLCall(glBindRenderbuffer(GL_RENDERBUFFER, m_HDRDepthBuffer));
		GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 800, 600));
		GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_HDRDepthBuffer));

		// explictly tell OpenGL which color attchment we'll use for rendering
		unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
		glDrawBuffers(2, attachments);
		
		// check is the framebuffer complete
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			std::cout << "ERROR:: FRAMEBUFFER is NOT COMPLETE." << std::endl;
		}
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
		#pragma endregion
		
		#pragma region pingpong fbo
		GLCall(glGenFramebuffers(2, m_PingPongFbos));
		GLCall(glGenTextures(2, m_PingPongColorBuffers));
		for (int i = 0; i < 2; i++)
		{
			GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_PingPongFbos[i]));
			GLCall(glBindTexture(GL_TEXTURE_2D, m_PingPongColorBuffers[i]));
			GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, 800, 600, 0, GL_RGBA, GL_FLOAT, NULL));
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

			GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_PingPongColorBuffers[i], 0));
			// check is the framebuffer complete
			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			{
				std::cout << "ERROR:: FRAMEBUFFER is NOT COMPLETE." << std::endl;
			}
		}
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
		#pragma endregion

	}

	void TestBloom::OnRender()
	{
		Renderer renderer;
		glm::mat4 view = m_Camera.GetViewMatrix();
		glm::mat4 projection = m_Camera.GetProjectionMatrix();

		glBindFramebuffer(GL_FRAMEBUFFER, m_HDRfbo);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_HDRSceneShader->Bind();
		m_HDRSceneShader->SetUniformMatrix4fv("view", view);
		m_HDRSceneShader->SetUniformMatrix4fv("projection", projection);
		m_HDRSceneShader->SetUniform3f("viewPos", m_Camera.GetPosition());
		m_HDRSceneShader->SetUniform1i("reverse_normals", false);
		for (int i = 0; i < m_LightColors.size(); i++)
		{
			m_HDRSceneShader->SetUniform3f("lights[" + std::to_string(i) + "].Position", m_LightPositions[i]);
			m_HDRSceneShader->SetUniform3f("lights[" + std::to_string(i) + "].Color", m_LightColors[i]);
		}

		// large Cube as floor
		m_WoodTexture->Bind(0);
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0));
		model = glm::scale(model, glm::vec3(12.5f, 0.5f, 12.5f));
		m_HDRSceneShader->SetUniformMatrix4fv("model", model);
		renderer.Draw(*m_CubeVAO, *m_HDRSceneShader, 36);

		#pragma region then create multiple cubes as the scenery
		m_ContainerTexture->Bind(0);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 1.5f, 0.0));
		model = glm::scale(model, glm::vec3(0.5f));
		m_HDRSceneShader->SetUniformMatrix4fv("model", model);
		renderer.Draw(*m_CubeVAO, *m_HDRSceneShader, 36);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(2.0f, 0.0f, 1.0));
		model = glm::scale(model, glm::vec3(0.5f));
		m_HDRSceneShader->SetUniformMatrix4fv("model", model);
		renderer.Draw(*m_CubeVAO, *m_HDRSceneShader, 36);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-1.0f, -1.0f, 2.0));
		model = glm::rotate(model, 60.0f, glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
		m_HDRSceneShader->SetUniformMatrix4fv("model", model);
		renderer.Draw(*m_CubeVAO, *m_HDRSceneShader, 36);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 2.7f, 4.0));
		model = glm::rotate(model, 23.0f, glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
		model = glm::scale(model, glm::vec3(1.25));
		m_HDRSceneShader->SetUniformMatrix4fv("model", model);
		renderer.Draw(*m_CubeVAO, *m_HDRSceneShader, 36);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-2.0f, 1.0f, -3.0));
		model = glm::rotate(model, 124.0f, glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
		m_HDRSceneShader->SetUniformMatrix4fv("model", model);
		renderer.Draw(*m_CubeVAO, *m_HDRSceneShader, 36);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-3.0f, 0.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.5f));
		m_HDRSceneShader->SetUniformMatrix4fv("model", model);
		renderer.Draw(*m_CubeVAO, *m_HDRSceneShader, 36);
		#pragma endregion

		#pragma region create light cube
		m_LightCubeShader->Bind();
		m_LightCubeShader->SetUniformMatrix4fv("view", view);
		m_LightCubeShader->SetUniformMatrix4fv("projection", projection);
		for (int i = 0; i < 4; i++)
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(m_LightPositions[i]));
			model = glm::scale(model, glm::vec3(0.25f));
			m_LightCubeShader->SetUniformMatrix4fv("model", model);
			m_LightCubeShader->SetUniform3f("lightColor", m_LightColors[i]);
			renderer.Draw(*m_CubeVAO, *m_LightCubeShader, 36);
		}
		#pragma endregion

		#pragma region gauss blur
		bool horizontal = true, first_iteration = true;
		int amout = 10;
		m_GaussBlurShader->Bind();
		for (int i = 0; i < amout; i++)
		{
			GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_PingPongFbos[horizontal]));
			GLCall(glBindTexture(GL_TEXTURE_2D, first_iteration ? m_HDRColorBuffers[1] : m_PingPongColorBuffers[!horizontal]));
			m_GaussBlurShader->SetUniform1i("horizontal", horizontal);
			m_QuadVAO->Bind();
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
			horizontal = !horizontal;
			if (first_iteration) first_iteration = false;
		}
		#pragma endregion

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_HDRColorBuffers[0]);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, m_PingPongColorBuffers[1]);
		m_HDRQuadShader->Bind();
		m_HDRQuadShader->SetUniform1i("diffuseTexture", 0);
		m_HDRQuadShader->SetUniform1i("bloomBlur", 1);
		m_HDRQuadShader->SetUniform1f("exposure", m_Exposure);
		m_QuadVAO->Bind();
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	}

	void TestBloom::OnImGuiRender()
	{
		ImGui::DragFloat("Exposure", &m_Exposure, 0.1f, 0.0f, 1.0f);
	}
	
	TestBloom::~TestBloom()
	{}

	void TestBloom::OnUpdate(float deltaTime)
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