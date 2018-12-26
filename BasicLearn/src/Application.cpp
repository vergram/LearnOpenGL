#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_glfw.h"

#include "Renderer.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Renderer.h"
#include "Texture2D.h"
#include "Camera.h"
#include "Input.h"
#include "Window.h"

#include "tests/TestClearColor.h"
#include "tests/TestTexture.h"
#include "tests/TestCamera.h"
#include "tests/TestLight.h"
#include "tests/TestModel.h"
#include "tests/TestDepth.h"
#include "tests/TestFrameBuffer.h"
#include "tests/TestCubemap.h"
#include "tests/TestAdvanceGLSL.h"
#include "tests/TestGeometry.h"
#include "tests/TestInstance.h"
#include "tests/TestAdvanceLighting.h"
#include "tests/TestDepthMap.h"
#include "tests/TestPointShadow.h"
#include "tests/TestNormalMap.h"


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

const int SCR_WIDTH = 800;
const int SCR_HEIGHT = 600;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;
// camera
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// Input
float Input::MouseMovementX = 0.0f;
float Input::MouseMovementY = 0.0f;
float Input::MouseSrollOffset = 0.0f;
std::vector<int> Input::currentKeys(5, 0);

GLFWwindow* Window::window = NULL;

const char* glsl_version = "#version 130";

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_SAMPLES, 4);        // enable 4 * MSAA 

	Window::window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (Window::window == NULL)
	{
		std::cout << "Fail to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(Window::window);
	glfwSetFramebufferSizeCallback(Window::window, framebuffer_size_callback);
	glfwSetCursorPosCallback(Window::window, mouse_callback);
	glfwSetScrollCallback(Window::window, scroll_callback);

	// 在调用任何的 GL 函数前初始化 GLAD，给 GLAD 传入用来加载操作系统相关的 OpenGL 函数指针地址的函数。
	// GLFW 中获取函数指针地址的函数是 glfwGetProcAddress，所以我们就传入它，他根据我们编译的系统定义了正确的函数
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	{
		GLCall(glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT));
		glEnable(GL_MULTISAMPLE);                    // make sure we enable MSAA
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // 线框模式
		//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // 填充模式

		Renderer renderer;

		ImGui::CreateContext();
		ImGui_ImplGlfw_InitForOpenGL(Window::window, true);
		ImGui_ImplOpenGL3_Init(glsl_version);

		test::Test* currentTest = nullptr;
		test::TestMenu* testMenu = new test::TestMenu(currentTest);
		currentTest = testMenu;

		testMenu->RegisterTest<test::TestNormalMap>("NormalMap");
		testMenu->RegisterTest<test::TestPointShadow>("Point Shadow");
		testMenu->RegisterTest<test::TestDepthMap>("DepthMap");
		testMenu->RegisterTest<test::TestAdvanceLighting>("AdvanceLight");
		testMenu->RegisterTest<test::TestInstance>("Instance");
		testMenu->RegisterTest<test::TestGeometry>("Geometry");
		testMenu->RegisterTest<test::TestAdvanceGLSL>("UniformBufferObject");
		testMenu->RegisterTest<test::TestCubemap>("Skybox");
		testMenu->RegisterTest<test::TestFrameBuffer>("FrameBuffer");
		testMenu->RegisterTest<test::TestDepth>("DepthTest");
		testMenu->RegisterTest<test::TestModel>("Model");
		testMenu->RegisterTest<test::TestLight>("Light");
		testMenu->RegisterTest<test::TestCamera>("Camera");
		testMenu->RegisterTest<test::TestTexture>("Texture");
		testMenu->RegisterTest<test::TestClearColor>("Clear Color");
		
		while (!glfwWindowShouldClose(Window::window))
		{
			// per-frame time logic
			// --------------------
			float currentFrame = glfwGetTime();
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;

			processInput(Window::window);
			renderer.Clear();

			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			if (currentTest)
			{
				currentTest->OnUpdate(deltaTime);
				currentTest->OnRender();

				ImGui::Begin("Test");
				if (currentTest != testMenu && ImGui::Button("<-"))
				{
					delete currentTest;
					currentTest = testMenu;
				}
				currentTest->OnImGuiRender();
				ImGui::End();
			}

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			glfwPollEvents();
			glfwSwapBuffers(Window::window);
		}

		delete currentTest;
		if (currentTest != testMenu)
		{
			delete testMenu;
		}
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();

	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		Input::currentKeys[KeyBoard::ESC] = 1;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		Input::currentKeys[KeyBoard::W] = 1;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		Input::currentKeys[KeyBoard::S] = 1;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		Input::currentKeys[KeyBoard::A] = 1;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		Input::currentKeys[KeyBoard::D] = 1;
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	Input::MouseMovementX = xoffset;
	Input::MouseMovementY = yoffset;

}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	Input::MouseSrollOffset = yoffset;
	std::cout << "scrolling mouse and yoffset = " << yoffset << std::endl;
}
