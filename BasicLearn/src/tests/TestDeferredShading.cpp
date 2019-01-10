#include "TestDeferredShading.h"

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

	TestDeferredShading::TestDeferredShading() :m_Camera(), m_Exposure(1.0f)
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

		m_GeometryPassShader = std::make_unique<Shader>("res/shader/DeferredShadingGeometryPass.shader");
		m_QuadShader = std::make_unique<Shader>("res/shader/DeferredShadingQuad.shader");
		m_LightCubeShader = std::make_unique<Shader>("res/shader/DeferredShadingLightCube.shader");
		m_LightingPassShader = std::make_unique<Shader>("res/shader/DeferredShadingLightingPass.shader");

		m_Nanosuit = std::make_unique<Model>("res/models/nanosuit/nanosuit.obj", true);

		#pragma region position and color info
		// object positions
		m_ObjectPositions.push_back(glm::vec3(-3.0, -3.0, -3.0));
		m_ObjectPositions.push_back(glm::vec3( 0.0, -3.0, -3.0));
		m_ObjectPositions.push_back(glm::vec3( 3.0, -3.0, -3.0));
		m_ObjectPositions.push_back(glm::vec3(-3.0, -3.0,  0.0));
		m_ObjectPositions.push_back(glm::vec3( 0.0, -3.0,  0.0));
		m_ObjectPositions.push_back(glm::vec3( 3.0, -3.0,  0.0));
		m_ObjectPositions.push_back(glm::vec3(-3.0, -3.0,  3.0));
		m_ObjectPositions.push_back(glm::vec3( 0.0, -3.0,  3.0));
		m_ObjectPositions.push_back(glm::vec3( 3.0, -3.0,  3.0));

		// lighting info
		// -------------
		const unsigned int NR_LIGHTS = 32;
		srand(13);
		for (unsigned int i = 0; i < NR_LIGHTS; i++)
		{
			// calculate slightly random offsets
			float xPos = ((rand() % 100) / 100.0) * 6.0 - 3.0;
			float yPos = ((rand() % 100) / 100.0) * 6.0 - 4.0;
			float zPos = ((rand() % 100) / 100.0) * 6.0 - 3.0;
			m_LightPositions.push_back(glm::vec3(xPos, yPos, zPos));
			// also calculate random color
			float rColor = ((rand() % 100) / 200.0f) + 0.5; // between 0.5 and 1.0
			float gColor = ((rand() % 100) / 200.0f) + 0.5; // between 0.5 and 1.0
			float bColor = ((rand() % 100) / 200.0f) + 0.5; // between 0.5 and 1.0
			m_LightColors.push_back(glm::vec3(rColor, gColor, bColor));
		}

		#pragma endregion

		#pragma region G-Buffer init
		GLCall(glGenFramebuffers(1, &m_GBuffer));
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_GBuffer));

		// -position color buffer
		GLCall(glGenTextures(1, &m_GPosition));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_GPosition));
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, 800, 600, 0, GL_RGB, GL_FLOAT, NULL));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
		GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_GPosition, 0));

		// -normal color buffer
		GLCall(glGenTextures(1, &m_GNormal));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_GNormal));
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, 800, 600, 0, GL_RGB, GL_FLOAT, NULL));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
		GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_GNormal, 0));

		// -color + specular color buffer
		GLCall(glGenTextures(1, &m_GAlbedoSpec));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_GAlbedoSpec));
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 800, 600, 0, GL_RGBA, GL_FLOAT, NULL));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
		GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, m_GAlbedoSpec, 0));
		
		// - tell OpenGL which color attachments we'll use (of this framebuffer) for rendering 
		unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
		GLCall(glDrawBuffers(3, attachments));

		// add depth Render buffer object for the framebuffer
		GLCall(glGenRenderbuffers(1, &m_GDepthBuffer));
		GLCall(glBindRenderbuffer(GL_RENDERBUFFER, m_GDepthBuffer));
		GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 800, 600));
		GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_GDepthBuffer));

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			std::cout << "ERROR:: FRAMEBUFFER is NOT COMPLETE." << std::endl;
		}
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));

		#pragma endregion

	}

	void TestDeferredShading::OnRender()
	{
		Renderer renderer;
		glm::mat4 view = m_Camera.GetViewMatrix();
		glm::mat4 projection = m_Camera.GetProjectionMatrix();

		glBindFramebuffer(GL_FRAMEBUFFER, m_GBuffer);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_GeometryPassShader->Bind();
		m_GeometryPassShader->SetUniformMatrix4fv("view", view);
		m_GeometryPassShader->SetUniformMatrix4fv("projection", projection);

		for (int i = 0; i < m_ObjectPositions.size(); i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(m_ObjectPositions[i]));
			model = glm::scale(model, glm::vec3(0.25f));
			m_LightCubeShader->SetUniformMatrix4fv("model", model);
			m_Nanosuit->Draw(*m_GeometryPassShader);
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_GPosition);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, m_GNormal);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, m_GAlbedoSpec);

		m_LightingPassShader->Bind();
		m_LightingPassShader->SetUniform1i("gPosition", 0);
		m_LightingPassShader->SetUniform1i("gNormal", 1);
		m_LightingPassShader->SetUniform1i("gAlbedoSpec", 2);
		m_LightingPassShader->SetUniform3f("viewPos", m_Camera.GetPosition());
		for (int i = 0; i < m_LightColors.size(); i++)
		{
			m_LightingPassShader->SetUniform3f("lights[" + std::to_string(i) + "].Position", m_LightPositions[i]);
			m_LightingPassShader->SetUniform3f("lights[" + std::to_string(i) + "].Color", m_LightColors[i]);
			const float constant = 1.0f; // note that we don't send this to the shader, we assume it is always 1.0 (in our case)
			const float linear = 0.7f;
			const float quadratic = 1.8f;

			//  We use a light source's brightest color component, 
			// as solving the equation for a light's brightest intensity value best reflects the ideal light volume radius.
			float lightMax = std::fmaxf(m_LightColors[i].x, std::fmaxf(m_LightColors[i].y, m_LightColors[i].z));
			
			// 这里的场景中使用 5/256 作为不可见光强的阈值，表示少于它的光照强度都不在 fragment 里面计算
			// 关键公式是：
			// 5 / 256 = lightMax / Attenuation
			// 注意这里的 Attenuation 是衰减倍数，等于光的衰减方程：
			// Attenuation = 1 / (constant + linear * distance + quadratic * distance * distance)
			// 最后使用求根公式算出 radius 
			float radius = (-linear + std::sqrtf(linear * linear - 4.0f * quadratic * (constant - (256.0f / 5.0f) * lightMax)))
							/
							(2.0f * quadratic);

			m_LightingPassShader->SetUniform1f("lights[" + std::to_string(i) + "].Linear", linear);
			m_LightingPassShader->SetUniform1f("lights[" + std::to_string(i) + "].Quadratic", quadratic);
			m_LightingPassShader->SetUniform1f("lights[" + std::to_string(i) + "].Radius", radius);
		}
		m_QuadVAO->Bind();
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		glBindFramebuffer(GL_READ_FRAMEBUFFER, m_GBuffer);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glBlitFramebuffer(0, 0, 800, 600, 0, 0, 800, 600, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// forward rendering as usual
		m_LightCubeShader->Bind();
		m_LightCubeShader->SetUniformMatrix4fv("view", view);
		m_LightCubeShader->SetUniformMatrix4fv("projection", projection);
		for (int i = 0; i < m_LightPositions.size(); i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, m_LightPositions[i]);
			model = glm::scale(model, glm::vec3(0.25f));
			m_LightCubeShader->SetUniformMatrix4fv("model", model);
			m_LightCubeShader->SetUniform3f("lightColor", m_LightColors[i]);
			m_CubeVAO->Bind();
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
	}

	void TestDeferredShading::OnImGuiRender()
	{
		ImGui::DragFloat("Exposure", &m_Exposure, 0.1f, 0.0f, 1.0f);
	}
	
	TestDeferredShading::~TestDeferredShading()
	{}

	void TestDeferredShading::OnUpdate(float deltaTime)
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