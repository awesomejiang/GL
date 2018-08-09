#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdexcept>


class Framebuffer{
public:
	Framebuffer(int height, int width);

	void bind() const;
	void unbind() const;
	unsigned int getTex() const{return texColorBuffer;}
	
private:
	int height, width;
	unsigned int framebuffer, texColorBuffer, rbo;
};

#endif