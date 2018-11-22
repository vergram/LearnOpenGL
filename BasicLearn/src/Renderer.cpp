#include "Renderer.h"
#include <iostream>

void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
		std::cout << "[OPENGL ERROR] (" << error << "): " << function << "\n" << file << ": " << line << std::endl;
		return false;
	}
	return true;
}

Renderer::Renderer()
{
	GLCall(glEnable(GL_DEPTH_TEST));
}

Renderer::~Renderer()
{}

void Renderer::Clear() const
{
	GLCall(glClearColor(0.9f, 0.9f, 0.9f, 1.0f));       // 设置清空屏幕所用的颜色
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));               // 清空颜色
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader)
{
	va.Bind();
	ib.Bind();
	shader.Bind();
	GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, 0))
}

void Renderer::Draw(const VertexArray& va, const Shader& shader, int count)
{
	va.Bind();
	shader.Bind();
	GLCall(glDrawArrays(GL_TRIANGLES, 0, count));
}
