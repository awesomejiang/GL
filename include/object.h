#ifndef OBJECT_H
#define OBJECT_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>

class Object{
public:
	//TODO: Add a constructor which can copy VAO and VBO from other Objects
	Object();
	~Object();

	void setVBO(std::vector<float> const &vertices) const;
	void setEBO(std::vector<int> const &indices) const;
	void setVAO(unsigned int loc, long int size, size_t stride, long int offset) const;
	void clearBuffers();
	void draw(int n) const;

private:
	unsigned VAO, VBO, EBO;
};

#endif