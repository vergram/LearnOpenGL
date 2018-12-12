#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <vector>
#include <string>

#include "Shader.h"
#include "Mesh.h"

class Model
{
public:
	Model(const char* path);
	void Draw(Shader& shader);
	
	std::vector<Mesh> m_Meshs;
	std::vector<Texture> m_Textures_loaded;

private:
	std::string m_Directory;

	void loadModel(std::string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};

