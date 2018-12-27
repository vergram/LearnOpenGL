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

	TestNormalMap::TestNormalMap() :m_Camera(), m_LightPos(0.5f, 1.0f, 2.0f)
	{
		// tell GLFW to capture our mouse
		glfwSetInputMode(Window::window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		m_IsShowMouse = false;

		m_NormalMapModelShader = std::make_unique<Shader>("res/shader/AdvanceNormalMapModel.shader");
		m_QuadShader = std::make_unique<Shader>("res/shader/AdvanceNormalMap.shader");

		m_Model = std::make_unique<Model>("res/models/cyborg/cyborg.obj");


		// about tangent space
		// ���߿ռ����������������ģ�Ϳռ���Ϊ�ο�ϵ�ģ��ڼ�������T��B��ʱ�����е����ݶ����ڸòο�ϵ�¶����ġ�
		// �����߿ռ����ҪĿ�ľ������T��B��Ϊ��������ǣ�����֪�Ĺ�ϵ�������ó�һ������T��B�ķ��̣�Ҳ������д���Ĺ�ʽ��
		// Ϊʲô��д�������Ĺ�ʽ�Ӷ���λ��ֵ������ֵ��������������ΪE0����ͨ��T��B��������ϵõ�������������ϵ�ϵ�����ÿ���ͨ����������õ���
		// ��������Ķ�������ɾ�����T��B�����Ͻ��еġ�����ʵ����ֻ��������һ��Ǳ�ڵļ��ι�ϵ���õ����̣�������Ϊλ�ú���������ͬһ�����
		// ���ֻ���ڱ����ϻ�ù�ϵ����Ҳ������T��B�����ϲ����ǵ�λ������

		m_BrickWall = std::make_unique<Texture2D>("res/image/brickwall.jpg");
		m_BrickWall_Normal = std::make_unique<Texture2D>("res/image/brickwall_normal.jpg");

		#pragma region tangent space
		
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

		glm::vec3 tangent1, bitangent1;
		glm::vec3 tangent2, bitangent2;

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

		bitangent1.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
		bitangent1.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
		bitangent1.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
		bitangent1 = glm::normalize(bitangent1);

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


		bitangent2.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
		bitangent2.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
		bitangent2.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
		bitangent2 = glm::normalize(bitangent2);

		float quadVertices[] = {
			// positions            // normal         // texcoords  // tangent                          // bitangent
			pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
			pos2.x, pos2.y, pos2.z, nm.x, nm.y, nm.z, uv2.x, uv2.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
			pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,

			pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
			pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
			pos4.x, pos4.y, pos4.z, nm.x, nm.y, nm.z, uv4.x, uv4.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z
		};

		VertexBufferLayout PNTTBLayout;
		PNTTBLayout.Push<float>(3);
		PNTTBLayout.Push<float>(3);
		PNTTBLayout.Push<float>(2);
		PNTTBLayout.Push<float>(3);
		PNTTBLayout.Push<float>(3);

		m_QuadVAO = std::make_unique<VertexArray>();
		m_QuadVBO = std::make_unique<VertexBuffer>(quadVertices, sizeof(quadVertices));
		m_QuadVAO->AddBuffer(*m_QuadVBO, PNTTBLayout);


		#pragma endregion

	}

	void TestNormalMap::OnRender()
	{
		Renderer renderer;

		glm::mat4 model;
		//model = glm::rotate(model, -90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
		//model = glm::rotate(model, (float)glfwGetTime() * -10.0f, glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glm::mat4 view = m_Camera.GetViewMatrix();
		glm::mat4 projection = m_Camera.GetProjectionMatrix();

		//// lighting
		//m_QuadShader->Bind();
		//m_QuadShader->SetUniform3f("lightPos", m_LightPos);
		//m_QuadShader->SetUniform3f("viewPos", m_Camera.GetPosition());
		//m_QuadShader->SetUniformMatrix4fv("model", model);
		//m_QuadShader->SetUniformMatrix4fv("view", view);
		//m_QuadShader->SetUniformMatrix4fv("projection", projection);

		//// texture
		//m_BrickWall->Bind(0);
		//m_BrickWall_Normal->Bind(1);
		//m_QuadShader->SetUniform1i("diffuseTexture", 0);
		//m_QuadShader->SetUniform1i("normalMap", 1);
		//m_QuadVAO->Bind();
		//glDrawArrays(GL_TRIANGLES, 0, 6);

		m_NormalMapModelShader->Bind();
		m_NormalMapModelShader->SetUniform3f("lightPos", m_LightPos);
		m_NormalMapModelShader->SetUniform3f("viewPos", m_Camera.GetPosition());
		m_NormalMapModelShader->SetUniformMatrix4fv("model", model);
		m_NormalMapModelShader->SetUniformMatrix4fv("view", view);
		m_NormalMapModelShader->SetUniformMatrix4fv("projection", projection);
		m_Model->Draw(*m_NormalMapModelShader);

	}

	void TestNormalMap::OnImGuiRender()
	{
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