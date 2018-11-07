#pragma once

#include <vector>
#include <functional>
#include "imgui/imgui.h"
#include "Camera.h"

namespace test {

	class Test
	{
	public:
		Test():m_Camera(){}
		virtual ~Test() {}

		virtual void OnUpdate(float deltaTime) {}
		virtual void OnRender() {}
		virtual void OnImGuiRender() {
			if (ImGui::Button("W"))
			{
				m_Camera.MoveCameraPosition(Camera_Movement::FORWARD, 1.0f);
			}
			if (ImGui::Button("S"))
			{
				m_Camera.MoveCameraPosition(Camera_Movement::BACKWARD, 1.0f);
			}
			if (ImGui::Button("A"))
			{
				m_Camera.MoveCameraPosition(Camera_Movement::LEFT, 1.0f);
			}
			if (ImGui::Button("D"))
			{
				m_Camera.MoveCameraPosition(Camera_Movement::RIGHT, 1.0f);
			}
			if (ImGui::SliderFloat("Yaw", &m_Camera.m_Yaw, -90.0f, 90.0f))
			{
				m_Camera.SetYaw(m_Camera.m_Yaw);
			}
			if (ImGui::SliderFloat("Pitch", &m_Camera.m_Pitch, -90.0f, 90.0f))
			{
				m_Camera.SetPitch(m_Camera.m_Pitch);
			}
			if (ImGui::SliderFloat("Zoom", &m_Camera.m_Fov, 0.0f, 90.0f))
			{
				m_Camera.SetFov(m_Camera.m_Fov);
			}

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		}
		
	protected:
		Camera m_Camera;
	};

	class TestMenu: public test::Test
	{
	public:
		TestMenu(Test*& currentTestPointer);

		void OnImGuiRender() override;

		template<typename T>
		void RegisterTest(const std::string& testName)
		{
			std::cout << "Register Tese: " << testName << std::endl;
			m_Tests.push_back(std::make_pair(testName, []() { return new T(); }));
		}
	private:
		Test * & m_CurrentTest;
		std::vector<std::pair<std::string, std::function<Test*()>>> m_Tests;
	};

}