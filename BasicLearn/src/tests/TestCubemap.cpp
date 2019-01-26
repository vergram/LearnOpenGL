#include "TestCubemap.h"

#include "Window.h"
#include "Renderer.h"
#include "Input.h"
#include "imgui/imgui.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "VertexBufferLayout.h"

namespace test{

	TestCubemap::TestCubemap()
		:m_Camera()
	{
		// tell GLFW to capture our mouse
		glfwSetInputMode(Window::window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		float cubeVertices[] = {
			// positions          // normals          
			-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  
			 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  
			 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  
			 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  
			-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  
			-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  

			-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  
			 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  
			 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  
			 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  
			-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  
			-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  

			-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  
			-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  
			-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  
			-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  
			-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  
			-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  

			 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  
			 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  
			 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  
			 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  
			 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  
			 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  

			-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  
			 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  
			 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  
			 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  
			-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  
			-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  

			-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  
			 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  
			 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  
			 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  
			-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  
			-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  
		};
		float skyboxVertices[] = {
			// positions          
			-1.0f,  1.0f, -1.0f,
			-1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			 1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,

			-1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,

			 1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,

			-1.0f, -1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,

			-1.0f,  1.0f, -1.0f,
			 1.0f,  1.0f, -1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f, -1.0f,

			-1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			 1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			 1.0f, -1.0f,  1.0f
		};
		std::vector<std::string> skyboxFacePaths{
			"res/skybox/sky/right.jpg",
			"res/skybox/sky/left.jpg",
			"res/skybox/sky/top.jpg",
			"res/skybox/sky/bottom.jpg",
			"res/skybox/sky/front.jpg",
			"res/skybox/sky/back.jpg"
		};
		
		m_SkyboxVBO = std::make_unique<VertexBuffer>(skyboxVertices, sizeof(skyboxVertices));
		m_SkyboxVAO = std::make_unique<VertexArray>();
		VertexBufferLayout layout;
		layout.Push<float>(3);
		m_SkyboxVAO->AddBuffer(*m_SkyboxVBO, layout);

		m_CubeVAO = std::make_unique<VertexArray>();
		m_CubeVBO = std::make_unique<VertexBuffer>(cubeVertices, sizeof(cubeVertices));
		layout.Push<float>(3);
		m_CubeVAO->AddBuffer(*m_CubeVBO, layout);

		m_Model = std::make_unique<Model>("res/models/nanosuit_reflection/nanosuit.obj");
		m_SkyboxTexture = std::make_unique<TextureCube>(skyboxFacePaths);

		m_SkyboxShader = std::make_unique<Shader>("res/shader/environmentMap/Skybox.shader");
		m_NanosuitShader = std::make_unique<Shader>("res/shader/environmentMap/EnvmapModel.shader");
		m_CubeShader = std::make_unique<Shader>("res/shader/environmentMap/Envmap.shader");
		
	}

	void TestCubemap::OnRender()
	{
		Renderer renderer;
		glm::mat4 view = m_Camera.GetViewMatrix();
		glm::mat4 projection = m_Camera.GetProjectionMatrix();

		GLCall(glDepthFunc(GL_LESS));
		m_NanosuitShader->Bind();
		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
		model = glm::rotate(model, 180.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		m_NanosuitShader->SetUniformMatrix4fv("model", model);
		m_NanosuitShader->SetUniformMatrix4fv("view", view);
		m_NanosuitShader->SetUniformMatrix4fv("projection", projection);
		m_NanosuitShader->SetUniform3f("cameraPos", m_Camera.GetPosition());
		m_NanosuitShader->SetUniform1i("envmap", 3);
		m_SkyboxTexture->Bind(3);            // the nanosuit model we loaded have 3 ACTIVE texture unit, so we set the skybox as the 4th texture unit
		m_Model->Draw(*m_NanosuitShader);

		m_CubeShader->Bind();
		glm::mat4 modelCube(1.0f);
		modelCube = glm::translate(modelCube, glm::vec3(0.0f, 0.0f, 4.0f));
		m_CubeShader->SetUniformMatrix4fv("model", modelCube);
		m_CubeShader->SetUniformMatrix4fv("view", view);
		m_CubeShader->SetUniformMatrix4fv("projection", projection);
		m_CubeShader->SetUniform3f("cameraPos", m_Camera.GetPosition());
		m_SkyboxTexture->Bind();
		renderer.Draw(*m_CubeVAO, *m_CubeShader, 36);

		GLCall(glDepthFunc(GL_LEQUAL));
		m_SkyboxShader->Bind();
		m_SkyboxShader->SetUniformMatrix4fv("view", glm::mat4(glm::mat3(view)));
		m_SkyboxShader->SetUniformMatrix4fv("projection", projection);
		m_SkyboxTexture->Bind();
		renderer.Draw(*m_SkyboxVAO, *m_SkyboxShader, 36);

	}

	void TestCubemap::OnImGuiRender()
	{
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}

	void TestCubemap::OnUpdate(float deltaTime)
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

	TestCubemap::~TestCubemap()
	{}
}
