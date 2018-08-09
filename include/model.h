#ifndef MODEL_H
#define MODEL_H

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "shader.h"
#include "mesh.h"

#include <string>
#include <vector>
#include <unordered_map>
#include <stdexcept>

class Model{
public:
	Model(std::string path);

	void draw(Shader const &shader) const;
private:
	void processNode(aiNode *node);
	Mesh processMesh(aiMesh *mesh);
	std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
	unsigned int textureFromFile(std::string const &path, std::string const &directory);

	aiScene const *scene;
	std::string const directory;
	std::vector<Mesh> meshes;
	std::unordered_map<std::string, Texture> loadedTexture;

};

#endif