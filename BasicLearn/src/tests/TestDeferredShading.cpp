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

		m_Sphere = std::make_unique<Sphere>();

		m_GeometryPassShader = std::make_unique<Shader>("res/shader/DeferredShadingGeometryPass.shader");
		m_QuadShader = std::make_unique<Shader>("res/shader/DeferredShadingQuad.shader");
		m_LightCubeShader = std::make_unique<Shader>("res/shader/DeferredShadingLightCube.shader");
		m_LightingPassShader = std::make_unique<Shader>("res/shader/DeferredShadingLightingPass.shader");
		m_PointLightPassShader = std::make_unique<Shader>("res/shader/DeferredPointLightPass.shader");
		m_StencilPassShader = std::make_unique<Shader>("res/shader/DeferredStencilPass.shader");
		m_DebugShader = std::make_unique<Shader>("res/shader/DeferredDebug.shader");

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
		GLCall(glGenTextures(1, &m_GTangentPosition));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_GTangentPosition));
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, 800, 600, 0, GL_RGB, GL_FLOAT, NULL));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
		GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_GTangentPosition, 0));

		// -normal color buffer
		GLCall(glGenTextures(1, &m_GTangentNormal));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_GTangentNormal));
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, 800, 600, 0, GL_RGB, GL_FLOAT, NULL));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
		GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_GTangentNormal, 0));

		// -color + specular color buffer
		GLCall(glGenTextures(1, &m_GAlbedoSpec));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_GAlbedoSpec));
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 800, 600, 0, GL_RGBA, GL_FLOAT, NULL));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
		GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, m_GAlbedoSpec, 0));

		// final texture for bilt
		GLCall(glGenTextures(1, &m_GFinalTexture));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_GFinalTexture));
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, 800, 600, 0, GL_RGB, GL_FLOAT, NULL));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
		GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, m_GFinalTexture, 0));

		// add depth Render buffer object for the framebuffer
		GLCall(glGenRenderbuffers(1, &m_GDepthBuffer));
		GLCall(glBindRenderbuffer(GL_RENDERBUFFER, m_GDepthBuffer));
		GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH32F_STENCIL8, 800, 600));
		GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_GDepthBuffer));


		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			std::cout << "ERROR:: FRAMEBUFFER is NOT COMPLETE." << std::endl;
		}
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));

		#pragma endregion

	}

	//------------------ Deferred shading (with light volume) pipeline ---------------------------------
	// 1. DSGeometryPass(); setup GBuffer
	//
	// We need stencil to be enabled in the stencil pass to get the stencil buffer
	// updated and we also need it in the light pass because we render the light
	// only if the stencil passes.
	// 
	// ------- 2. glEnable(GL_STENCIL_TEST); ---------
	//
	//			foreach(pointLight in lights){
    //			    StencilPass(light);
	//			    PointLightPass(light);
    //			}
	// --------  glDisable(GL_STENCIL_TEST); ---------
	// 
	// 3. DSDirectionalLightPass(); 
	//
	// 4. FinalPass();
	// -------------------------------------------------------------------------------------------------
	void TestDeferredShading::OnRender()
	{
		Renderer renderer;
		glm::mat4 view = m_Camera.GetViewMatrix();
		glm::mat4 projection = m_Camera.GetProjectionMatrix();

		// clear the final texture
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_GBuffer);

		glDrawBuffer(GL_COLOR_ATTACHMENT3);
		glClear(GL_COLOR_BUFFER_BIT);


		#pragma region Geometry Pass		
		glEnable(GL_DEPTH_TEST);
		// only geometry pass can updates the depth buffer
		glDepthMask(GL_TRUE);

		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_GBuffer);

		// - tell OpenGL which color attachments we'll use (of this framebuffer) for rendering 
		unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
		GLCall(glDrawBuffers(3, attachments));
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_GeometryPassShader->Bind();
		m_GeometryPassShader->SetUniformMatrix4fv("view", view);
		m_GeometryPassShader->SetUniformMatrix4fv("projection", projection);

		for (int i = 0; i < m_ObjectPositions.size(); i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(m_ObjectPositions[i]));
			model = glm::scale(model, glm::vec3(0.25f));
			m_GeometryPassShader->SetUniformMatrix4fv("model", model);
			m_Nanosuit->Draw(*m_GeometryPassShader);
		}
		
		// When we get here the depth buffer is already populated and the stencil pass
		// depends on it, but it does not write to it.
		glDepthMask(GL_FALSE);
		#pragma endregion
		
		glEnable(GL_STENCIL_TEST);

		#pragma region point light pass
		// setup things that unchange uniform in this pass
		m_PointLightPassShader->Bind();
		m_PointLightPassShader->SetUniform1i("gPosition", 0);
		m_PointLightPassShader->SetUniform1i("gNormal", 1);
		m_PointLightPassShader->SetUniform1i("gAlbedoSpec", 2);
		m_PointLightPassShader->SetUniform3f("viewPos", m_Camera.GetPosition());
		m_PointLightPassShader->SetUniformMatrix4fv("view", view);
		m_PointLightPassShader->SetUniformMatrix4fv("projection", projection);

		m_StencilPassShader->Bind();
		m_StencilPassShader->SetUniformMatrix4fv("view", view);
		m_StencilPassShader->SetUniformMatrix4fv("projection", projection);

		for (int i = 0; i < m_LightPositions.size(); i++)
		{
			// stencil pass
			//{
				// must disable the draw buffers since we only want to update stencil buffer in this pass
				glDrawBuffer(GL_NONE);

				glEnable(GL_DEPTH_TEST);

				// here we disable FACE_CULL because we gonna use the technique which make use of both FRONT and BACK face
				glDisable(GL_CULL_FACE);

				glClear(GL_STENCIL_BUFFER_BIT);

				// we need stencil test work, but we want it always pass
				// we only care about the value in stencil buffer
				glStencilFunc(GL_ALWAYS, 0, 0);

				// void glStencilOpSeparate(GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass);
				// BACK face only INCREMENT when depth test FAIL, else keep it value
				glStencilOpSeparate(GL_BACK, GL_KEEP, GL_INCR_WRAP, GL_KEEP);
				// FRONT face only DRCREMENT when depth test FAIL, else keep it value
				glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_DECR_WRAP, GL_KEEP);

				float radius = calculateLightRadius(m_LightColors[i]);

				glm::mat4 model;
				model = glm::translate(model, m_LightPositions[i]);
				model = glm::scale(model, glm::vec3(radius));
				m_StencilPassShader->Bind();
				m_StencilPassShader->SetUniformMatrix4fv("model", model);
				m_Sphere->Bind();
				glDrawElements(GL_TRIANGLES, m_Sphere->GetIndicesCount(), GL_UNSIGNED_INT, 0);
			//}
			// point light pass
			//{
				// draw the pixels to final texture
				glDrawBuffer(GL_COLOR_ATTACHMENT3);

				// after the stencil pass, now the stencil buffer should have NOT_ZERO value for this particular light
				glStencilFunc(GL_NOTEQUAL, 0, 0xFF);

				// now we only draw the pixels which pass the stencil test for this light
				glDisable(GL_DEPTH_TEST);

				// set up texture sampler for calculatelight
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, m_GTangentPosition);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, m_GTangentNormal);
				glActiveTexture(GL_TEXTURE2);
				glBindTexture(GL_TEXTURE_2D, m_GAlbedoSpec);
				
				glEnable(GL_BLEND);
				glBlendEquation(GL_FUNC_ADD);         // we use additive blend for light volume
				glBlendFunc(GL_ONE, GL_ONE);

				glEnable(GL_CULL_FACE);               // enable face culling because each light should only take one acount for blend
				glCullFace(GL_FRONT);                 // culling the FRONT face because when camera was in that light volume, we can only see the BACK
				
				m_PointLightPassShader->Bind();
				m_PointLightPassShader->SetUniform3f("pointLight.Position", m_LightPositions[i]);
				m_PointLightPassShader->SetUniform3f("pointLight.Color", m_LightColors[i]);
				m_PointLightPassShader->SetUniform1f("pointLight.Linear", 0.7f);
				m_PointLightPassShader->SetUniform1f("pointLight.Quadratic", 1.8f);
				m_PointLightPassShader->SetUniformMatrix4fv("model", model);
				//m_DebugShader->Bind();
				//m_DebugShader->SetUniformMatrix4fv("model", model);
				//m_DebugShader->SetUniformMatrix4fv("view", view);
				//m_DebugShader->SetUniformMatrix4fv("projection", projection);

				m_Sphere->Bind();
				glDrawElements(GL_TRIANGLES, m_Sphere->GetIndicesCount(), GL_UNSIGNED_INT, 0);
				glCullFace(GL_BACK);
				glDisable(GL_BLEND);
			//}
		}
		#pragma endregion

		glDisable(GL_STENCIL_TEST);
		
		#pragma region final pass
		//glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		//glBindFramebuffer(GL_READ_FRAMEBUFFER, m_GBuffer);
		//glReadBuffer(GL_COLOR_ATTACHMENT3);
		//glBlitFramebuffer(0, 0, 800, 600,
		//				  0, 0, 800, 600, GL_COLOR_BUFFER_BIT, GL_NEAREST);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_GFinalTexture);
		m_QuadShader->Bind();
		m_QuadShader->SetUniform1i("diffuseTexture", 0);
		m_QuadShader->SetUniform1f("exposure", m_Exposure);
		m_QuadVAO->Bind();
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		#pragma endregion


		// forward rendering as usual
		#pragma region forward rendering for light sphere
		//glBindFramebuffer(GL_READ_FRAMEBUFFER, m_GBuffer);
		//glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		//glBlitFramebuffer(0, 0, 800, 600, 0, 0, 800, 600, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
		//glBindFramebuffer(GL_FRAMEBUFFER, 0);

		//m_LightCubeShader->Bind();
		//m_LightCubeShader->SetUniformMatrix4fv("view", view);
		//m_LightCubeShader->SetUniformMatrix4fv("projection", projection);
		//for (int i = 0; i < m_LightPositions.size(); i++)
		//{
		//	glm::mat4 model = glm::mat4(1.0f);
		//	model = glm::translate(model, m_LightPositions[i]);
		//	model = glm::scale(model, glm::vec3(0.25f));
		//	m_LightCubeShader->SetUniformMatrix4fv("model", model);
		//	m_LightCubeShader->SetUniform3f("lightColor", m_LightColors[i]);
		//	//m_CubeVAO->Bind();
		//	//glDrawArrays(GL_TRIANGLES, 0, 36);
		//	m_Sphere->Bind();
		//	glDrawElements(GL_TRIANGLES, m_Sphere->GetIndicesCount(), GL_UNSIGNED_INT, 0);
		//}
		#pragma endregion
	}

	void TestDeferredShading::OnImGuiRender()
	{
		ImGui::DragFloat("Exposure", &m_Exposure, 0.1f, 0.0f, 1.0f);
	}

	float TestDeferredShading::calculateLightRadius(glm::vec3 & lightColor)
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