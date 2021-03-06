#pragma once

#include <vector>
#include <functional>
#include "imgui/imgui.h"

namespace test {

	class Test
	{
	public:
		Test(){}
		virtual ~Test() {}

		virtual void OnUpdate(float deltaTime) {}
		virtual void OnRender() {}
		virtual void OnImGuiRender() {}
		
	};

	class TestMenu: public test::Test
	{
	public:
		TestMenu(Test*& currentTestPointer);

		void OnImGuiRender() override;
		void OnUpdate(float deltaTime) override;

		template<typename T>
		void RegisterTest(const std::string& testName)
		{
			std::cout << "Register Test: " << testName << std::endl;
			m_Tests.push_back(std::make_pair(testName, []() { return new T(); }));
		}
	private:
		Test * & m_CurrentTest;
		std::vector<std::pair<std::string, std::function<Test*()>>> m_Tests;
	};

}