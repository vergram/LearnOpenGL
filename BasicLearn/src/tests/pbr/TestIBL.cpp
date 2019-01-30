#include "TestIBL.h"

#include "VertexBufferLayout.h"
#include "Renderer.h"
#include "Input.h"
#include "Window.h"

#include "imgui/imgui.h"

#include "stb_image/stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <random>

namespace test{

	TestIBL::TestIBL() :m_Camera(), m_Exposure(1.0f), m_DebugMode(0)
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

		m_PBRLightingShader = std::make_unique<Shader>("res/shader/pbr/pbrLighting.shader");
		m_EquirectangularToCubeMapShader = std::make_unique<Shader>("res/shader/ibl/equirectangular_to_cubemap.shader");
		m_SkyboxShader = std::make_unique<Shader>("res/shader/environmentMap/Skybox.shader");
		m_DebugShader = std::make_unique<Shader>("res/shader/debug/cubemap.shader");

		//m_Nanosuit = std::make_unique<Model>("res/models/nanosuit/nanosuit.obj", true);

		m_Albedo    = std::make_unique<Texture2D>("res/pbr/rustediron2/albedo.png", true);
		m_Metallic  = std::make_unique<Texture2D>("res/pbr/rustediron2/metallic.png");
		m_Roughness = std::make_unique<Texture2D>("res/pbr/rustediron2/roughness.png");
		m_Ao        = std::make_unique<Texture2D>("res/pbr/rustediron2/ao.png");
		m_Normal    = std::make_unique<Texture2D>("res/pbr/rustediron2/normal.png");

		// lighting info
		// -------------
		m_LightPositions = {
			glm::vec3(0.0f, 0.0f, 10.0f), 
		};
		m_LightColors = {
			glm::vec3(150.0f, 150.0f, 150.0f),
		};

		m_PBRLightingShader->Bind();
		m_PBRLightingShader->SetUniformMatrix4fv("projection", m_Camera.GetProjectionMatrix());
		m_PBRLightingShader->SetUniform1i("reverse_normals", false);
		m_PBRLightingShader->SetUniform1i("albedoMap", 0);
		m_PBRLightingShader->SetUniform1i("metallicMap", 1);
		m_PBRLightingShader->SetUniform1i("roughnessMap", 2);
		m_PBRLightingShader->SetUniform1i("aoMap", 3);
		m_PBRLightingShader->SetUniform1i("normalMap", 4);

		m_EnvCubeMap = buildCubeMap("res/hdr/newport_loft/newport_loft.hdr");
	}

	void TestIBL::OnRender()
	{
		m_PBRLightingShader->Bind();
		m_PBRLightingShader->SetUniformMatrix4fv("view", m_Camera.GetViewMatrix());
		m_PBRLightingShader->SetUniform3f("camPos", m_Camera.GetPosition());

		m_Albedo->Bind(0);
		m_Metallic->Bind(1);
		m_Roughness->Bind(2);
		m_Ao->Bind(3);
		m_Normal->Bind(4);

		int nrRows = 7;
		int nrColumns = 7;
		float spacing = 2.5f;

		glm::mat4 model(1.0f);
		for (int row = 0; row < nrRows; row++)
		{
			//m_PBRLightingShader->SetUniform1f("metallic", (float)row / (float)nrRows);
			for (int col = 0; col < nrColumns; col++)
			{
				//m_PBRLightingShader->SetUniform1f("roughness", glm::clamp((float)col / (float)nrColumns, 0.05f, 1.0f));
				
				model = glm::mat4(1.0f);
				model = glm::translate(model, glm::vec3((row - (nrRows / 2)) * spacing,
														(col - (nrColumns / 2)) * spacing,
														0.0f));
				m_PBRLightingShader->SetUniformMatrix4fv("model", model);
				m_Sphere->Bind();
				glDrawElements(GL_TRIANGLES, m_Sphere->GetIndicesCount(), GL_UNSIGNED_INT, 0);
			}
		}

		for (int i = 0; i < m_LightPositions.size(); i++)
		{
			//glm::vec3 newPos = m_LightPositions[i] + glm::vec3(sinf(glfwGetTime() * 5.0f) * 5.0f, 0.0f, 0.0f);
			//newPos = m_LightPositions[i];
			//m_PBRLightingShader->SetUniform3f("lightPos[" + std::to_string(i) + "]", newPos);
			m_PBRLightingShader->SetUniform3f("lightPos[" + std::to_string(i) + "]", m_LightPositions[i]);
			m_PBRLightingShader->SetUniform3f("lightColor[" + std::to_string(i) + "]", m_LightColors[i]);

			model = glm::mat4(1.0f);
			//model = glm::translate(model, newPos);
			model = glm::translate(model, m_LightPositions[i]);
			model = glm::scale(model, glm::vec3(0.5f));
			m_PBRLightingShader->SetUniformMatrix4fv("model", model);
			m_Sphere->Bind();
			glDrawElements(GL_TRIANGLES, m_Sphere->GetIndicesCount(), GL_UNSIGNED_INT, 0);
		}

		// render skybox
		m_SkyboxShader->Bind();
		m_SkyboxShader->SetUniformMatrix4fv("view", m_Camera.GetViewMatrix());
		m_SkyboxShader->SetUniformMatrix4fv("projection", m_Camera.GetProjectionMatrix());
		m_SkyboxShader->SetUniform1i("skybox", 0);
		//m_DebugShader->Bind();
		//m_DebugShader->SetUniformMatrix4fv("model", glm::mat4(1.0f));
		//m_DebugShader->SetUniformMatrix4fv("view", m_Camera.GetViewMatrix());
		//m_DebugShader->SetUniformMatrix4fv("projection", m_Camera.GetProjectionMatrix());
		//m_DebugShader->SetUniform1i("cubemap", 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_EnvCubeMap);
		m_CubeVAO->Bind();
		glDepthFunc(GL_LEQUAL);               // we need it when we are render a skybox with trick it depth always be the max depth 1.0
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

	unsigned int TestIBL::buildCubeMap(const char* path)
	{
		unsigned int hdrEnvTexture = loadHDREnvmap(path);

		unsigned int captureFBO, captureRBO;
		glViewport(0, 0, 512, 512);
		glGenFramebuffers(1, &captureFBO);
		glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);

		glGenRenderbuffers(1, &captureRBO);
		glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_COMPONENT, GL_RENDERBUFFER, captureRBO);

		unsigned int envCubeMap;
		glGenTextures(1, &envCubeMap);
		glBindTexture(GL_TEXTURE_CUBE_MAP, envCubeMap);
		for (unsigned int i = 0; i < 6; i++)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 512, 512, 0, GL_RGB, GL_FLOAT, nullptr);
		}
		GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
		GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));

		// since envCubeMap is a unit cube, we do not need a huge range for perspective projection
		float near_plane = 0.1f, far_plane = 10.0f;
		glm::mat4 capturePorjection = glm::perspective(90.0f, 1.0f, near_plane, far_plane);

		std::vector<glm::mat4> captureViews(6);
		// for why these up vector is negative see the link https://www.nvidia.com/object/cube_map_ogl_tutorial.html "Mapping Texture Coordinates to Cube Map Faces"
		captureViews[0] = glm::lookAt(glm::vec3(0.0f,0.0f,0.0f), glm::vec3( 1.0,  0.0,  0.0), glm::vec3(0.0, -1.0,  0.0));
		captureViews[1] = glm::lookAt(glm::vec3(0.0f,0.0f,0.0f), glm::vec3(-1.0,  0.0,  0.0), glm::vec3(0.0, -1.0,  0.0));
		captureViews[2] = glm::lookAt(glm::vec3(0.0f,0.0f,0.0f), glm::vec3( 0.0,  1.0,  0.0), glm::vec3(0.0,  0.0,  1.0));
		captureViews[3] = glm::lookAt(glm::vec3(0.0f,0.0f,0.0f), glm::vec3( 0.0, -1.0,  0.0), glm::vec3(0.0,  0.0, -1.0));
		captureViews[4] = glm::lookAt(glm::vec3(0.0f,0.0f,0.0f), glm::vec3( 0.0,  0.0,  1.0), glm::vec3(0.0, -1.0,  0.0));
		captureViews[5] = glm::lookAt(glm::vec3(0.0f,0.0f,0.0f), glm::vec3( 0.0,  0.0, -1.0), glm::vec3(0.0, -1.0,  0.0));

		m_EquirectangularToCubeMapShader->Bind();
		m_EquirectangularToCubeMapShader->SetUniformMatrix4fv("projection", capturePorjection);
		m_EquirectangularToCubeMapShader->SetUniform1i("equirectangularMap", 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, hdrEnvTexture);

		for (unsigned int i = 0; i < 6; i++)
		{
			m_EquirectangularToCubeMapShader->SetUniformMatrix4fv("view", captureViews[i]);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, envCubeMap, 0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			m_CubeVAO->Bind();
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, 800, 600);
		return envCubeMap;
	}

	unsigned int TestIBL::loadHDREnvmap(const char* path)
	{
		stbi_set_flip_vertically_on_load(true);
		unsigned int hdrTexture;
		int width, height, nrComponents;
		float* data = stbi_loadf(path, &width, &height, &nrComponents, 0);
		if (data)
		{
			glGenTextures(1, &hdrTexture);
			glBindTexture(GL_TEXTURE_2D, hdrTexture);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, data);
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Fail to load HDR textrue" << std::endl;
		}
		return hdrTexture;
	}

	void TestIBL::OnImGuiRender()
	{
		ImGui::DragFloat("Exposure", &m_Exposure, 0.1f, 0.0f, 1.0f);
		ImGui::DragInt("DebugGBuffer", &m_DebugMode, 1.0f, 0, 3);
	}

	TestIBL::~TestIBL()
	{}

	void TestIBL::OnUpdate(float deltaTime)
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