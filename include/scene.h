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
	Scene(int, int);

	void processInput() const;

	GLFWwindow *window;
};


#endif
