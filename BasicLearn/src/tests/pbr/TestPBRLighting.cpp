#include "TestPBRLighting.h"

#include "VertexBufferLayout.h"
#include "Renderer.h"
#include "Input.h"
#include "Window.h"

#include "imgui/imgui.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <random>

namespace test{

	TestPBRLighting::TestPBRLighting() :m_Camera(), m_Exposure(1.0f), m_DebugMode(0)
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

		m_Sphere = std::make_unique<Sphere>();

		m_GeometryPassShader = std::make_unique<Shader>("res/shader/SSAOGeometryPass.shader");
		m_QuadShader = std::make_unique<Shader>("res/shader/SSAOQuad.shader");
		m_LightingPassShader = std::make_unique<Shader>("res/shader/SSAOLightPass.shader");
		m_SSAOBlurShader = std::make_unique<Shader>("res/shader/SSAOBlur.shader");
		m_SSAOPassShader = std::make_unique<Shader>("res/shader/SSAOSsaoPass.shader");
		m_ReconstructPosShader = std::make_unique<Shader>("res/shader/SSAOReconstructPos.shader");

		m_Nanosuit = std::make_unique<Model>("res/models/nanosuit/nanosuit.obj", true);

		// lighting info
		// -------------
		m_LightPosition = glm::vec3(2.0, 4.0, -2.0);
		m_LightColor = glm::vec3(0.2, 0.2, 0.7);


		// build a 64 sample kernal in tangent sapce with a weight that close to the kernal
		std::uniform_real_distribution<float> randomFloats(0.0f, 1.0f);  // random float in [0.0, 1.0]
		std::default_random_engine generator;
		for (unsigned int i = 0; i < 64; i++)
		{
			glm::vec3 sampler{
				randomFloats(generator) * 2.0f - 1.0f,
				randomFloats(generator) * 2.0f - 1.0f,
				randomFloats(generator)
			};
			sampler = glm::normalize(sampler);
			float scale = float(i) / 64;
			scale = lerp(0.1f, 1.0f, scale * scale);
			sampler *= scale;
			m_SSAO_Kernel.push_back(sampler);
		}

		// build a random noise rotations for kernel
		for (unsigned int i = 0; i < 16; i++)
		{
			glm::vec3 noise{
				randomFloats(generator) * 2.0f - 1.0f,
				randomFloats(generator) * 2.0f - 1.0f,
				0.0f
			};
			m_SSAO_Noise.push_back(noise);
		}
		glGenTextures(1, &m_NoiseTexture);
		GLCall(glBindTexture(GL_TEXTURE_2D, m_NoiseTexture));
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, 4, 4, 0, GL_RGB, GL_FLOAT, m_SSAO_Noise.data()));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));

		// init ssao fbo
		GLCall(glGenFramebuffers(1, &m_SSAO_fbo));
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_SSAO_fbo));
		GLCall(glGenTextures(2, m_SSAOTextures));
		for (unsigned int i = 0; i < 2; i++)
		{
			GLCall(glBindTexture(GL_TEXTURE_2D, m_SSAOTextures[i]));
			GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, 800, 600, 0, GL_RGB, GL_FLOAT, NULL));
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
			GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, m_SSAOTextures[i], 0));
		}

		// debug depth
		GLCall(glGenTextures(1, &m_ReconstructPosTexture));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_ReconstructPosTexture));
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, 800, 600, 0, GL_RGB, GL_FLOAT, NULL));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
		GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, m_ReconstructPosTexture, 0));
		
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			std::cout << "ERROR:: FRAMEBUFFER is NOT COMPLETE." << std::endl;
		}
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));


		#pragma region G-Buffer init
		GLCall(glGenFramebuffers(1, &m_GBufferFBO));
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_GBufferFBO));

		glGenTextures(4, m_GBufferTextures);
		// for POSITION, NORMAL
		for (unsigned int i = 0; i < 2; i++)
		{
			GLCall(glBindTexture(GL_TEXTURE_2D, m_GBufferTextures[i]));
			GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, 800, 600, 0, GL_RGB, GL_FLOAT, NULL));
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
			GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, m_GBufferTextures[i], 0));
		}
		// ALBEDO
		GLCall(glBindTexture(GL_TEXTURE_2D, m_GBufferTextures[GBufferTextureType::ALBEDO]));
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
		GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, m_GBufferTextures[GBufferTextureType::ALBEDO], 0));

		// DEPTH
		GLCall(glBindTexture(GL_TEXTURE_2D, m_GBufferTextures[GBufferTextureType::DEPTH]));
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, 800, 600, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
		GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_GBufferTextures[GBufferTextureType::DEPTH], 0));

		// add depth Render buffer object for the framebuffer
		//GLCall(glGenRenderbuffers(1, &m_GDepthBuffer));
		//GLCall(glBindRenderbuffer(GL_RENDERBUFFER, m_GDepthBuffer));
		//GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600));            // don't use GL_DEPTH32F_STENCIL8 because the 0 framebuffer can't accpet it well
		//GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_GDepthBuffer));

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			std::cout << "ERROR:: FRAMEBUFFER is NOT COMPLETE." << std::endl;
		}
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));

		#pragma endregion

	}

	void TestPBRLighting::OnRender()
	{

		GeometryPass();

		SSAOPass();

		//LightingPass();
		
		#pragma region debug porpuse
		//glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		//glBindFramebuffer(GL_READ_FRAMEBUFFER, m_GBufferFBO);
		//glReadBuffer(GL_COLOR_ATTACHMENT0 + m_DebugMode);
		//glBlitFramebuffer(0, 0, 800, 600,
		//				  0, 0, 800, 600, GL_COLOR_BUFFER_BIT, GL_NEAREST);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_SSAOTextures[m_DebugMode]);
		m_QuadShader->Bind();
		m_QuadShader->SetUniform1i("diffuseTexture", 0);
		m_QuadShader->SetUniform1f("exposure", m_Exposure);
		m_QuadVAO->Bind();
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		#pragma endregion
	}

	void TestPBRLighting::GeometryPass()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_GBufferFBO);

		glEnable(GL_DEPTH_TEST);

		// - tell OpenGL which color attachments we'll use (of this framebuffer) for rendering 
		unsigned int attachments[3];
		for (unsigned int i = 0; i < 3; i++)
		{
			attachments[i] = GL_COLOR_ATTACHMENT0 + i;
		}
		GLCall(glDrawBuffers(3, attachments));
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_GeometryPassShader->Bind();
		m_GeometryPassShader->SetUniformMatrix4fv("view", m_Camera.GetViewMatrix());
		m_GeometryPassShader->SetUniformMatrix4fv("projection", m_Camera.GetProjectionMatrix());

		glm::mat4 model = glm::mat4(1.0f);
		// room cube
		model = glm::translate(model, glm::vec3(0.0, 7.0f, 0.0f));
		model = glm::scale(model, glm::vec3(7.5f, 7.5f, 7.5f));
		m_GeometryPassShader->SetUniformMatrix4fv("model", model);
		m_GeometryPassShader->SetUniform1i("inverseNormal", true);
		m_CubeVAO->Bind();
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// nanosuit model on the floor
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 5.0));
		model = glm::rotate(model, -90.0f, glm::vec3(1.0, 0.0, 0.0));
		model = glm::scale(model, glm::vec3(0.5f));
		m_GeometryPassShader->Bind();
		m_GeometryPassShader->SetUniformMatrix4fv("model", model);
		m_GeometryPassShader->SetUniform1i("inverseNormal", false);
		m_Nanosuit->Draw(*m_GeometryPassShader);


	}

	void TestPBRLighting::SSAOPass()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_SSAO_fbo);
		glDrawBuffer(GL_COLOR_ATTACHMENT0 + SSAOTextureType::SSAO);
		glDisable(GL_DEPTH_TEST);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_GBufferTextures[GBufferTextureType::POSITION]);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, m_GBufferTextures[GBufferTextureType::NORMAL]);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, m_NoiseTexture);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, m_GBufferTextures[GBufferTextureType::DEPTH]);

		m_SSAOPassShader->Bind();
		m_SSAOPassShader->SetUniform1i("gPosition", 0);
		m_SSAOPassShader->SetUniform1i("gNormal", 1);
		m_SSAOPassShader->SetUniform1i("texNoise", 2);
		m_SSAOPassShader->SetUniformMatrix4fv("projection", m_Camera.GetProjectionMatrix());
		
		m_SSAOPassShader->SetUniform1i("gDepthMap", 3);
		m_SSAOPassShader->SetUniform1f("gProjectionTangentHalfFov", std::tanf(glm::radians(45.0f / 2.0f)));
		m_SSAOPassShader->SetUniform1f("gProjectionAspect", 1.3f);

		for (int i = 0; i < m_SSAO_Kernel.size(); i++)
		{
			m_SSAOPassShader->SetUniform3f("samples[" + std::to_string(i) + "]", m_SSAO_Kernel[i]);
		}
		m_QuadVAO->Bind();
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		glBindFramebuffer(GL_FRAMEBUFFER, m_SSAO_fbo);
		glDrawBuffer(GL_COLOR_ATTACHMENT0 + SSAOTextureType::BLUR);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_SSAOTextures[SSAOTextureType::SSAO]);
		m_SSAOBlurShader->Bind();
		m_SSAOBlurShader->SetUniform1i("ssaoInput", 0);
		m_QuadVAO->Bind();
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	}

	void TestPBRLighting::LightingPass()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_GBufferTextures[GBufferTextureType::POSITION]);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, m_GBufferTextures[GBufferTextureType::NORMAL]);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, m_GBufferTextures[GBufferTextureType::ALBEDO]);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, m_SSAOTextures[SSAOTextureType::BLUR]);
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, m_GBufferTextures[GBufferTextureType::DEPTH]);

		m_LightingPassShader->Bind();
		m_LightingPassShader->SetUniform1i("gPosition", 0);
		m_LightingPassShader->SetUniform1i("gNormal", 1);
		m_LightingPassShader->SetUniform1i("gAlbedoSpec", 2);
		m_LightingPassShader->SetUniform1i("ssao", 3);
		m_LightingPassShader->SetUniform1i("gDepthMap", 4);

		glm::vec3 lightPos = glm::vec3(m_Camera.GetViewMatrix() * glm::vec4(m_LightPosition, 1.0f));
		m_LightingPassShader->SetUniform3f("pointLight.Position", lightPos);
		m_LightingPassShader->SetUniform3f("pointLight.Color", m_LightColor);
		m_LightingPassShader->SetUniform1f("pointLight.Linear", 0.09f);
		m_LightingPassShader->SetUniform1f("pointLight.Quadratic", 0.032);
		m_LightingPassShader->SetUniform1f("gProjectionTangentHalfFov", std::tanf(glm::radians(45.0f / 2.0f)));
		m_LightingPassShader->SetUniform1f("gProjectionAspect", 1.3f);
		m_LightingPassShader->SetUniformMatrix4fv("projection", m_Camera.GetProjectionMatrix());

		m_QuadVAO->Bind();
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}

	void TestPBRLighting::OnImGuiRender()
	{
		ImGui::DragFloat("Exposure", &m_Exposure, 0.1f, 0.0f, 1.0f);
		ImGui::DragInt("DebugGBuffer", &m_DebugMode, 1.0f, 0, 3);
	}

	float TestPBRLighting::calculateLightRadius(glm::vec3 & lightColor)
	{
		const float constant = 1.0f; // note that we don't send this to the shader, we assume it is always 1.0 (in our case)
		const float linear = 0.7f;
		const float quadratic = 1.8f;

		// use light volume
		// We use a light source's brightest color component, 
		// as solving the equation for a light's brightest intensity value best reflects the ideal light volume radius.
		float lightMax = std::fmaxf(lightColor.x, std::fmaxf(lightColor.y, lightColor.z));

		// 这里的场景中使用 5/256 作为不可见光强的阈值，表示少于它的光照强度都不在 fragment 里面计算
		// 关键公式是：
		// 5 / 256 = lightMax / Attenuation
		// 注意这里的 Attenuation 是衰减倍数，等于光的衰减方程：
		// Attenuation = 1 / (constant + linear * distance + quadratic * distance * distance)
		// 最后使用求根公式算出 radius 
		float radius = (-linear + std::sqrtf(linear * linear - 4.0f * quadratic * (constant - (256.0f / 5.0f) * lightMax)))
						/
						(2.0f * quadratic);
		return radius;
	}

	TestPBRLighting::~TestPBRLighting()
	{}

	void TestPBRLighting::OnUpdate(float deltaTime)
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