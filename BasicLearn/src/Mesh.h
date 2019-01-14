#pragma once

#include <assimp/types.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>

#include "Shader.h"

struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	glm::vec3 Tangent;
};

struct Texture
{
	unsigned int id;
	std::string type;
	aiString path;
};

class Mesh
{
public:
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	void Draw(Shader& shader);
	inline unsigned int GetVAO() const { return VAO; }
	inline unsigned int GetEBO() const { return EBO; }


protected:
	unsigned int VAO, VBO, EBO;
	void setupMesh();
};

