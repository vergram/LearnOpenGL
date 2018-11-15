#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Window
{
public:
	static GLFWwindow* window;
};

//class Window
//{
//public:
//	Window(int width, int height, const std::string& title);
//	virtual ~Window();
//
//	inline bool IsCloseRequested()          const { return glfwWindowShouldClose(m_window); }
//	inline int GetWidth()                   const { return m_width; }
//	inline int GetHeight()                  const { return m_height; }
//	inline float GetAspect()                const { return (float)m_width / (float)m_height; }
//	inline const std::string& GetTitle()    const { return m_title; }
//	inline glm::vec2 GetCenter()            const { return glm::vec2((float)m_width / 2.0f, (float)m_height / 2.0f); }
//	inline GLFWwindow* GetGLFWwindow()		{ return m_window; }
//	inline const Input& GetInput()          const { return m_input; }
//
//	void processInput();
//	void SwapBuffers();
//
//private:
//	int           m_width;
//	int           m_height;
//	std::string   m_title;
//	GLFWwindow*   m_window;
//	Input         m_input;
//	bool          m_isCloseRequested;
//
//	static void framebufferSizeCallback(GLFWwindow* window, int width, int height)
//	{
//		glViewport(0, 0, width, height);
//	}
//};

