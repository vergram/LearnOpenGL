#pragma once

#include <string>
#include <unordered_map>

#include "glm/glm.hpp"

namespace BreakOut {

	struct ShaderProgramSource
	{
		std::string VertexSource;
		std::string FragmentSource;
		std::string GeometrySource;
	};

	class Shader
	{
	private:
		unsigned int m_RendererID;
		std::string m_filepath;
		std::unordered_map<std::string, int> m_UniformLocationCache;
		// caching for uniforms
	public:
		Shader();
		Shader(const std::string& filepath);
		~Shader();

		void Bind() const;
		void Unbind();
		unsigned int GetID() const;

		// set Uniforms
		void SetUniform1i(const std::string& name, int value);
		void SetUniform1f(const std::string& name, float value);
		void SetUniform2f(const std::string& name, float v0, float v1);
		void SetUniform3f(const std::string& name, float v0, float v1, float v2);
		void SetUniform3f(const std::string& name, glm::vec3 vec3);
		void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
		void SetUniformMatrix4fv(const std::string& name, const glm::mat4& trans);

	private:
		ShaderProgramSource ParseShader(const std::string& filepath);
		unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader, const std::string& geometryShader);
		unsigned int CompileShader(unsigned int type, const std::string& source);
		int GetUniformLocation(const std::string& name);
	};

}
