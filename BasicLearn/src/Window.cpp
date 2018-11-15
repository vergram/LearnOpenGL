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
//	// �ڵ����κε� GL ����ǰ��ʼ�� GLAD���� GLAD �����������ز���ϵͳ��ص� OpenGL ����ָ���ַ�ĺ�����
//	// GLFW �л�ȡ����ָ���ַ�ĺ����� glfwGetProcAddress���������Ǿʹ����������������Ǳ����ϵͳ��������ȷ�ĺ���
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