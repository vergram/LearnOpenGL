#include "Test.h"
#include "imgui/imgui.h"
#include "Input.h"
#include "Window.h"


namespace test {


	TestMenu::TestMenu(Test *& currentTestPointer)
		:m_CurrentTest(currentTestPointer)
	{}

	void TestMenu::OnImGuiRender()
	{
		for(auto& test : m_Tests)
		{
			if (ImGui::Button(test.first.c_str()))
			{
				m_CurrentTest = test.second(); 
			}
		}
	}

	void TestMenu::OnUpdate(float deltaTime)
	{
		if (Input::currentKeys[KeyBoard::ESC])
		{
			glfwSetWindowShouldClose(Window::window, true);
			Input::currentKeys[KeyBoard::ESC] = 0;
		}
	}

}

