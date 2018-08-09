#ifndef SCENE_H
#define SCENE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdexcept>
#include <iostream>
#include <vector>
#include <string>
#include <cmath>

class Scene{
public:
	Scene();

	void processInput() const;
	void setVBO(std::vector<float> const &vertices) const;
	void setEBO(std::vector<int> const &indices) const;
	void setVAO(unsigned int loc, long int size, size_t stride, long int offset) const;
	void clearBuffers();
	void draw(int n) const;

	GLFWwindow *window;

private:
    unsigned int EBO, VBO, VAO;
};


#endif
