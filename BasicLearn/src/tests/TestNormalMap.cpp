#include "TestNormalMap.h"

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

	TestNormalMap::TestNormalMap() :m_Camera(), m_LightPos(0.0f, 0.0f, 0.0f)
	{
		// tell GLFW to capture our mouse
		glfwSetInputMode(Window::window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		m_IsShowMouse = false;
		m_IsSelfShadowing = false;

		m_NormapMapShader = std::make_unique<Shader>("res/shader/normalMap/AdvanceNormalMap.shader");
		m_BrickWall = std::make_unique<Texture2D>("res/image/brickwall.jpg");
		m_BrickWall_Normal = std::make_unique<Texture2D>("res/image/brickwall_normal.jpg");

		m_NormalMapModelShader = std::make_unique<Shader>("res/shader/normalMap/AdvanceNormalMapModel.shader");
		m_Model = std::make_unique<Model>("res/models/cyborg/cyborg.obj");

		m_ParallaxMapShader = std::make_unique<Shader>("res/shader/parallaxMap/AdvanceParallaxMap.shader");
		m_Bricks2 = std::make_unique<Texture2D>("res/image/bricks2.jpg");
		m_Bricks2_Normal = std::make_unique<Texture2D>("res/image/bricks2_normal.jpg");
		m_Bricks2_Depth = std::make_unique<Texture2D>("res/image/bricks2_disp.jpg");

		m_Toy_Box_Diffuse = std::make_unique<Texture2D>("res/image/toy_box_diffuse.png");
		m_Toy_Box_Normal = std::make_unique<Texture2D>("res/image/toy_box_normal.png");
		m_Toy_Box_Depth = std::make_unique<Texture2D>("res/image/toy_box_disp.png");


		// about tangent space
		// 切线空间的坐标轴向量是以模型空间作为参考系的，在计算向量T和B的时候所有的数据都是在该参考系下度量的。
		// 求切线空间的主要目的就是求出T和B，为了求出它们，从已知的关系出发，得出一个关于T和B的方程，也就是你写出的公式。
		// 为什么会写出这样的公式从而把位置值和纹理值关连起来？是因为E0可以通过T和B的线性组合得到，而该线性组合的系数正好可以通过纹理坐标得到，
		// 纹理坐标的定义或生成就是在T和B方向上进行的。这里实际上只是利用了一种潜在的几何关系来得到方程，这是因为位置和纹理不是同一个概念，
		// 因此只能在比例上获得关系，这也导致了T和B基本上不会是单位向量。
		#pragma region tangent space for quad
		
		// positions
		glm::vec3 pos1(-1.0f, 1.0f, 0.0f);
		glm::vec3 pos2(-1.0f, -1.0f, 0.0f);
		glm::vec3 pos3(1.0f, -1.0f, 0.0f);
		glm::vec3 pos4(1.0f, 1.0f, 0.0f);
		// texture coordinates
		glm::vec2 uv1(0.0f, 1.0f);
		glm::vec2 uv2(0.0f, 0.0f);
		glm::vec2 uv3(1.0f, 0.0f);
		glm::vec2 uv4(1.0f, 1.0f);
		// normal vector
		glm::vec3 nm(0.0f, 0.0f, 1.0f);

		glm::vec3 tangent1;
		glm::vec3 tangent2;

		// triangle 1
		// ----------
		glm::vec3 edge1 = pos2 - pos1;
		glm::vec3 edge2 = pos3 - pos1;
		glm::vec2 deltaUV1 = uv2 - uv1;
		glm::vec2 deltaUV2 = uv3 - uv1;

		float f = 1 / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);

		tangent1.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
		tangent1.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
		tangent1.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
		tangent1 = glm::normalize(tangent1);

		// triangle 2
		// ----------
		edge1 = pos3 - pos1;
		edge2 = pos4 - pos1;
		deltaUV1 = uv3 - uv1;
		deltaUV2 = uv4 - uv1;

		f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

		tangent2.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
		tangent2.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
		tangent2.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
		tangent2 = glm::normalize(tangent2);

		float quadVertices[] = {
			// positions            // normal         // texcoords  // tangent                         
			pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent1.x, tangent1.y, tangent1.z,
			pos2.x, pos2.y, pos2.z, nm.x, nm.y, nm.z, uv2.x, uv2.y, tangent1.x, tangent1.y, tangent1.z,
			pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent1.x, tangent1.y, tangent1.z,

			pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent2.x, tangent2.y, tangent2.z,
			pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent2.x, tangent2.y, tangent2.z,
			pos4.x, pos4.y, pos4.z, nm.x, nm.y, nm.z, uv4.x, uv4.y, tangent2.x, tangent2.y, tangent2.z
		};

		VertexBufferLayout PNTTLayout;
		PNTTLayout.Push<float>(3);
		PNTTLayout.Push<float>(3);
		PNTTLayout.Push<float>(2);
		PNTTLayout.Push<float>(3);

		m_QuadVAO = std::make_unique<VertexArray>();
		m_QuadVBO = std::make_unique<VertexBuffer>(quadVertices, sizeof(quadVertices));
		m_QuadVAO->AddBuffer(*m_QuadVBO, PNTTLayout);


		#pragma endregion

	}

	void TestNormalMap::OnRender()
	{
		glm::mat4 model;
		model = glm::rotate(model, -90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
		//model = glm::rotate(model, (float)glfwGetTime() * -10.0f, glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
		//model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glm::mat4 view = m_Camera.GetViewMatrix();
		glm::mat4 projection = m_Camera.GetProjectionMatrix();

		//// normal map brick wall
		//m_NormalMapShader->Bind();
		//m_NormalMapShader->SetUniform3f("lightPos", m_LightPos);
		//m_NormalMapShader->SetUniform3f("viewPos", m_Camera.GetPosition());
		//m_NormalMapShader->SetUniformMatrix4fv("model", model);
		//m_NormalMapShader->SetUniformMatrix4fv("view", view);
		//m_NormalMapShader->SetUniformMatrix4fv("projection", projection);
		//m_NormalMap->SetUniform1i("diffuseTexture", 0);
		//m_NormalMap->SetUniform1i("normalMap", 1);
		//m_BrickWall->Bind(0);
		//m_BrickWall_Normal->Bind(1);
		//m_QuadVAO->Bind();
		//glDrawArrays(GL_TRIANGLES, 0, 6);

		//// normal map model
		//m_NormalMapModelShader->Bind();
		//m_NormalMapModelShader->SetUniform3f("lightPos", m_LightPos);
		//m_NormalMapModelShader->SetUniform3f("viewPos", m_Camera.GetPosition());
		//m_NormalMapModelShader->SetUniformMatrix4fv("model", model);
		//m_NormalMapModelShader->SetUniformMatrix4fv("view", view);
		//m_NormalMapModelShader->SetUniformMatrix4fv("projection", projection);
		//m_Model->Draw(*m_NormalMapModelShader);


		float radius = 2.0f;
		float currentTime = 20.0f * glfwGetTime();
		glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
		glm::mat4 lightModel(1.0f);
		lightModel = glm::translate(lightModel, glm::vec3(radius * glm::sin(glm::radians(currentTime)), 
														  1.0f, 
														  radius * glm::cos(glm::radians(currentTime))));
		lightPos = glm::vec3(lightModel * glm::vec4(lightPos, 1.0f));

		// Parallax Map bricks
		m_ParallaxMapShader->Bind();
		m_ParallaxMapShader->SetUniform3f("lightPos", lightPos);
		m_ParallaxMapShader->SetUniform3f("viewPos", m_Camera.GetPosition());
		m_ParallaxMapShader->SetUniformMatrix4fv("model", model);
		m_ParallaxMapShader->SetUniformMatrix4fv("view", view);
		m_ParallaxMapShader->SetUniformMatrix4fv("projection", projection);
		m_ParallaxMapShader->SetUniform1f("parallax_scale", 0.1f);
		m_ParallaxMapShader->SetUniform1i("texture_diffuse1", 0);
		m_ParallaxMapShader->SetUniform1i("texture_normal1", 1);
		m_ParallaxMapShader->SetUniform1i("texture_depth1", 2);
		m_ParallaxMapShader->SetUniform1i("self_shadowing", m_IsSelfShadowing);
		//m_Bricks2->Bind(0);
		//m_Bricks2_Normal->Bind(1);
		//m_Bricks2_Depth->Bind(2);
		m_Toy_Box_Diffuse->Bind(0);
		m_Toy_Box_Normal->Bind(1);
		m_Toy_Box_Depth->Bind(2);
		m_QuadVAO->Bind();
		glDrawArrays(GL_TRIANGLES, 0, 6);

	}

	void TestNormalMap::OnImGuiRender()
	{
		ImGui::Checkbox("SoftShadow", &m_IsSelfShadowing);
	}
	
	TestNormalMap::~TestNormalMap()
	{}

	void TestNormalMap::OnUpdate(float deltaTime)
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