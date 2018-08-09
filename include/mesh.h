#ifndef MASH_H
#define MASH_H

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"

#include <string>
#include <vector>

struct Vertex{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;
};

struct Texture{
	unsigned int id;
	std::string type;
};

class Mesh{
public:
	Mesh(std::vector<Vertex> const &vertices,
		 std::vector<unsigned int> const &indices,
		 std::vector<Texture> const &textures);

	void draw(Shader const &shader) const;
	void draw(unsigned int const &textureID) const;
private:
	unsigned int const pts;
	std::vector<Texture> const textures;

	unsigned int VAO, VBO, EBO;
};

#endif