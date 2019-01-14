#pragma once

#include "glm/glm.hpp"

#include <vector>

class VertexBufferLayout;

// source: http://www.songho.ca/opengl/gl_sphere.html#sphere
class Sphere
{
private:
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> texCoords;

	std::vector<int> indices;

	unsigned int m_VAO;
	unsigned int m_VBO;
	unsigned int m_EBO;

	float radius;
public:
	Sphere(int sectors = 36, int stacks = 18, float radius = 1.0f);
	~Sphere();

	inline unsigned int GetVAO() const{ return m_VAO; }
	inline unsigned int GetEBO() const{ return m_EBO; }
	inline unsigned int GetIndicesCount() const { return indices.size(); }

	void Bind() const;
	void UnBind() const;
};