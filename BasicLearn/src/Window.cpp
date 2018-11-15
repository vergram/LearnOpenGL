//#include "Window.h"
//#include <iostream>
//
//
//
//Window::Window(int width, int height, const std::string& title)
//	:m_width(width), m_height(height), m_title(title)
//{
//	glfwInit();
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
//
//	GLFWwindow* m_window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
//	if (m_window == NULL)
//	{
//		std::cout << "Fail to create GLFW window" << std::endl;
//		glfwTerminate();
//		
//	}
//	glfwMakeContextCurrent(m_window);
//
//	glfwSetFramebufferSizeCallback(m_window, Window::framebufferSizeCallback);
//
//	// 在调用任何的 GL 函数前初始化 GLAD，给 GLAD 传入用来加载操作系统相关的 OpenGL 函数指针地址的函数。
//	// GLFW 中获取函数指针地址的函数是 glfwGetProcAddress，所以我们就传入它，他根据我们编译的系统定义了正确的函数
//	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
//	{
//		std::cout << "Failed to initialize GLAD" << std::endl;
//	}
//}
//
//Window::~Window()
//{
//	glfwTerminate();
//}
//
//void Window::processInput()
//{}
//
//void Window::SwapBuffers()
//{
//	glfwSwapBuffers(m_window);
//}
//
////void framebufferSizeCallback(GLFWwindow* window, int width, int height)
////{
////	glViewport(0, 0, width, height);
////}