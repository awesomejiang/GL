#include "shader.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <type_traits>

Shader::Shader(std::string const &vshader, std::string const &fshader){
	 std::string vcode, fcode;

	//read files
	std::ifstream vfs(vshader), ffs(fshader);
	if(vfs.is_open() && ffs.is_open()){
		std::stringstream vss, fss;
		vss << vfs.rdbuf();
		fss << ffs.rdbuf();

		vcode = vss.str();
		fcode = fss.str();
	}
	else
		throw std::runtime_error("Cannot open shader files.");

	//compile shaders
	int success;
	char infoLog[512];

	//vertex shader
	char const *vptr = vcode.c_str();
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vptr, nullptr);
	glCompileShader(vertex);

	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if(!success){
		glGetShaderInfoLog(vertex, 512, nullptr, infoLog);
		throw std::runtime_error(infoLog);
	}

	//fragment shader
	char const *fptr = fcode.c_str();
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fptr, nullptr);
	glCompileShader(fragment);

	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if(!success){
		glGetShaderInfoLog(fragment, 512, nullptr, infoLog);
		throw std::runtime_error(infoLog);
	}

	//link shaders
	ID = glCreateProgram();
	glAttachShader(ID, vertex);(glIsShader(vertex)==GL_TRUE);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);

	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if(!success){
		glGetProgramInfoLog(ID, 512, nullptr, infoLog);
		throw std::runtime_error(infoLog);
	}
	
	//TODO: potential memory link if thrown exception?
	deleteShader();
}


void Shader::use() const{
	glUseProgram(ID);
}


void Shader::deleteShader() const{
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}


void Shader::deleteProgram() const{
	glDeleteProgram(ID);
}


//setUniform overrides
template<>
void Shader::setUniform(std::string const &name,
float const &v1) const{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), v1);
}

template<>
void Shader::setUniform(std::string const &name,
float const &v1, float const &v2) const{
	glUniform2f(glGetUniformLocation(ID, name.c_str()), v1, v2);
}

template<>
void Shader::setUniform(std::string const &name,
float const &v1, float const &v2, float const &v3) const{
	glUniform3f(glGetUniformLocation(ID, name.c_str()), v1, v2, v3);
}

template<>
void Shader::setUniform(std::string const &name,
float const &v1, float const &v2, float const &v3, float const &v4) const{
	glUniform4f(glGetUniformLocation(ID, name.c_str()), v1, v2, v3, v4);
}

template<>
void Shader::setUniform(std::string const &name,
int const &v1) const{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), v1);
}

template<>
void Shader::setUniform(std::string const &name,
int const &v1, int const &v2) const{
	glUniform2i(glGetUniformLocation(ID, name.c_str()), v1, v2);
}

template<>
void Shader::setUniform(std::string const &name,
int const &v1, int const &v2, int const &v3) const{
	glUniform3i(glGetUniformLocation(ID, name.c_str()), v1, v2, v3);
}

template<>
void Shader::setUniform(std::string const &name,
int const &v1, int const &v2, int const &v3, int const &v4) const{
	glUniform4i(glGetUniformLocation(ID, name.c_str()), v1, v2, v3, v4);
}

template<>
void Shader::setUniform(std::string const &name,
glm::vec3 const &vec) const{
	glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(vec));
}

template<>
void Shader::setUniform(std::string const &name,
glm::vec4 const &vec) const{
	glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(vec));
}

template<>
void Shader::setUniform(std::string const &name,
glm::mat4 const &mat) const{
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
}
