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
#include "Texture.h"

#include "tests/TestClearColor.h"
#include "tests/TestTexture.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

const int SCR_HEIGHT = 600;
const int SCR_WIDTH = 800;

const char* glsl_version = "#version 130";

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Fail to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// 在调用任何的 GL 函数前初始化 GLAD，给 GLAD 传入用来加载操作系统相关的 OpenGL 函数指针地址的函数。
	// GLFW 中获取函数指针地址的函数是 glfwGetProcAddress，所以我们就传入它，他根据我们编译的系统定义了正确的函数
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	{
		GLCall(glViewport(0, 0, 800, 600));

		float vertices[] = {
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
			 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

			-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

			0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
		};

		glm::vec3 cubePositions[] = {
			glm::vec3(0.0f,  0.0f,  0.0f),
			glm::vec3(2.0f,  5.0f, -15.0f),
			glm::vec3(-1.5f, -2.2f, -2.5f),
			glm::vec3(-3.8f, -2.0f, -12.3f),
			glm::vec3(2.4f, -0.4f, -3.5f),
			glm::vec3(-1.7f,  3.0f, -7.5f),
			glm::vec3(1.3f, -2.0f, -2.5f),
			glm::vec3(1.5f,  2.0f, -2.5f),
			glm::vec3(1.5f,  0.2f, -1.5f),
			glm::vec3(-1.3f,  1.0f, -1.5f)
		};

		//float vertices[] = {
		//	// position        textcoordine
		//	-0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
		//	-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
		//	 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		//	 0.5f,  0.5f, 0.0f, 1.0f, 1.0f
		//};

		unsigned int indices[] = {
			0, 1, 2,
			0, 2, 3
		};


		// 1. 绑定顶点数组对象
		// 2. 把我们的顶点数组复制到一个顶点缓冲中，供 OpenGL 使用
		// 3. 复制我们的索引数组到一个索引缓冲中，供 OpenGL 使用
		// 4. 设定顶点属性指针

		IndexBuffer ib(indices, 6);
		VertexBuffer vb(vertices, sizeof(vertices));
		VertexArray va;
		VertexBufferLayout layout;
		layout.Push<float>(3);
		layout.Push<float>(2);
		va.AddBuffer(vb, layout);
		
		Shader shader("res/shader/Basic.shader");
		shader.Bind();

		Texture texture0("res/image/wall.jpg");
		Texture texture1("res/image/awesomeface.png");
		texture0.Bind();
		texture1.Bind(1);

		shader.SetUniform1i("u_Texture0", 0);
		shader.SetUniform1i("u_Texture1", 1);

		vb.Unbind();
		va.Unbind();
		ib.Unbind();
		shader.Unbind();

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // 线框模式
		//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // 填充模式

		Renderer renderer;

		ImGui::CreateContext();
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init(glsl_version);

		test::Test* currentTest = nullptr;
		test::TestMenu* testMenu = new test::TestMenu(currentTest);
		currentTest = testMenu;

		testMenu->RegisterTest<test::TestClearColor>("Clear Color");
		testMenu->RegisterTest<test::TestTexture>("Texture");


		while (!glfwWindowShouldClose(window))
		{
			#pragma region mvp matrices
			//bool show_demo_window = true;
			//bool show_another_window = false;
			//ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

			//static float fovy = 45.0f;
			//static float aspect = (float)SCR_WIDTH / (float)SCR_HEIGHT;
			//static float zNear = 0.1f;
			//static float zFar = 100.0f;

			//ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

			//ImGui::SliderFloat("fovy", &fovy, 0.0f, 360.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
			//ImGui::SliderFloat("aspect", &aspect, -10.0f, 10.0f);
			//ImGui::SliderFloat("zNear", &zNear, -10.0f, 100.0f);
			//ImGui::SliderFloat("zFar", &zFar, -10.0f, 1000.0f);

			//ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			//ImGui::End();

			//glm::mat4 view(1.0f);
			//glm::mat4 projection(1.0f);
			//view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
			//projection = glm::perspective(fovy, aspect, zNear, zFar);
			//for (int i = 0; i < 10; i++)
			//{
			//	renderer.Draw(va, shader, 36);
			//	glm::mat4 model(1.0f);
			//	model = glm::translate(model, cubePositions[i]);
			//	model = glm::rotate(model, 20.0f * (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));
			//	glm::mat4 mvp = projection * view * model;

			//	shader.SetUniformMatrix4fv("u_mvp", mvp);
			//}
#pragma endregion
			processInput(window);
			renderer.Clear();

			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			if (currentTest)
			{
				currentTest->OnUpdate(0.0f);
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
			glfwSwapBuffers(window);
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
	{
		glfwSetWindowShouldClose(window, true);
	}
}
