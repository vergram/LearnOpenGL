#pragma once

#include "Test.h"

namespace Test {

	class TestClearColor: public Test
	{
	public:
		TestClearColor();
		~TestClearColor();

		void OnUpdate(float delaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		float m_ClearColor[4];
	};

}