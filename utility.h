#ifndef UTILITY_H
#define UTILITY_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "mesh.h"
#include "shader.h"
#include "camera.h"

extern int screenWidth, screenHeight;

void debugQuad(unsigned int textureID){
  std::vector<Vertex> vertices = {
    // positions          // normals           // texture coords
    {{-1.0f, -1.0f,  0.0f},  {0.0f,  0.0f, 1.0f},   {0.0f, 0.0f}},
    {{ 1.0f, -1.0f,  0.0f},  {0.0f,  0.0f, 1.0f},   {1.0f, 0.0f}},
    {{ 1.0f,  1.0f,  0.0f},  {0.0f,  0.0f, 1.0f},   {1.0f, 1.0f}},
    {{ 1.0f,  1.0f,  0.0f},  {0.0f,  0.0f, 1.0f},   {1.0f, 1.0f}},
    {{-1.0f,  1.0f,  0.0f},  {0.0f,  0.0f, 1.0f},   {0.0f, 1.0f}},
    {{-1.0f, -1.0f,  0.0f},  {0.0f,  0.0f, 1.0f},   {0.0f, 0.0f}}
  };

  Mesh mesh(vertices);
  Shader debugShader("shaders/debug/screen.vs", "shaders/debug/screen.fs");
  debugShader.use();
  debugShader.setUniform("screenTexture", 0);

  glViewport(0, 0, screenWidth, screenHeight);
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);

  debugShader.use();
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, textureID);
  mesh.draw(debugShader);
}

template <typename T>
void showNormal(T const &m, Camera const &camera, glm::mat4 const &model){
  Shader shader("shaders/debug/normal.vs", "shaders/debug/normal.fs", "shaders/debug/normal.gs");
  shader.use();
  shader.setUniform("projection", camera.getProjection());
  shader.setUniform("view", camera.getView());
  shader.setUniform("model", model);
  m.draw(shader);

  shader.deleteProgram();
}

#endif