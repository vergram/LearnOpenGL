#pragma once

#include <glfw/glfw3.h>
#include <glm/glm.hpp>
#include "Input.h"

#include <string>

class Window
{
public:
	Window(int width, int height, const std::string& title);
	virtual ~Window();

	inline bool IsCloseRequested()          const { return m_isCloseRequested; }
	inline int GetWidth()                   const { return m_width; }
	inline int GetHeight()                  const { return m_height; }
	inline float GetAspect()                const { return (float)m_width / (float)m_height; }
	inline const std::string& GetTitle()    const { return m_title; }
	inline glm::vec2 GetCenter()             const { return glm::vec2((float)m_width / 2.0f, (float)m_height / 2.0f); }
	inline GLFWwindow* GetGLFWwindow()		{ return m_window; }
	inline const Input& GetInput()          const { return m_input; }

private:
	int           m_width;
	int           m_height;
	std::string   m_title;
	GLFWwindow*   m_window;
	Input         m_input;
	bool          m_isCloseRequested;
};

