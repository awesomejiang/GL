#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>
#include <stdexcept>
#include <vector>

class Texture{
public:
	Texture(std::string const &, int const &);

	void bind();
	unsigned int texture;

private:
	int const index;
	int width, height, nrChannels;
	unsigned char *data;

};

#endif