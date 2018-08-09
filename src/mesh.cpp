#include "mesh.h"

Mesh::Mesh(std::vector<Vertex> const &vertices,
std::vector<unsigned int> const &indices,
std::vector<Texture> const &textures)
:pts(indices.size()), textures(textures){
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	//set VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

	//set EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    //set vertex postions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    //set normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

    //set texCoords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

    //unbind VAO
    glBindVertexArray(0);
}

void Mesh::draw(Shader const &shader) const{
	unsigned int diffIdx = 1, specIdx = 1, normalIdx = 1, heightIdx = 1;
	for(auto i=0; i<textures.size(); ++i){
		glActiveTexture(GL_TEXTURE0 + i);

		std::string name = textures[i].type;
		if(name == "textureDiffuse")
			name += std::to_string(diffIdx++);
		else if(name == "textureSpecular")
			name += std::to_string(specIdx++);
		else if(name == "textureNormal")
			name += std::to_string(normalIdx++);
		else if(name == "textureHeight")
			name += std::to_string(heightIdx++);

		shader.setUniform("material." + name, i);
		glBindTexture(GL_TEXTURE_2D, textures[i].id);
	}

	//draw
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, pts, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	//set everything to default
	glActiveTexture(GL_TEXTURE0);
}


void Mesh::draw(GLenum texType, unsigned int const &textureID) const{
	glBindTexture(texType, textureID);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, pts, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}