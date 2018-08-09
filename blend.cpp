#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stdexcept>
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <numeric>

#include "shader.h"
#include "scene.h"
#include "camera.h"
#include "mesh.h"
#include "model.h"

int main(){
    //init scene
    Scene scene(800, 600);

    //create lamps by hand
    std::vector<Vertex> vertices = {
      // positions          // normals           // texture coords
      {{-0.5f, -0.5f, -0.5f},  {0.0f,  0.0f, -1.0f},  {0.0f, 0.0f}},
      {{ 0.5f, -0.5f, -0.5f},  {0.0f,  0.0f, -1.0f},  {1.0f, 0.0f}},
      {{ 0.5f,  0.5f, -0.5f},  {0.0f,  0.0f, -1.0f},  {1.0f, 1.0f}},
      {{ 0.5f,  0.5f, -0.5f},  {0.0f,  0.0f, -1.0f},  {1.0f, 1.0f}},
      {{-0.5f,  0.5f, -0.5f},  {0.0f,  0.0f, -1.0f},  {0.0f, 1.0f}},
      {{-0.5f, -0.5f, -0.5f},  {0.0f,  0.0f, -1.0f},  {0.0f, 0.0f}},

      {{-0.5f, -0.5f,  0.5f},  {0.0f,  0.0f, 1.0f},   {0.0f, 0.0f}},
      {{ 0.5f, -0.5f,  0.5f},  {0.0f,  0.0f, 1.0f},   {1.0f, 0.0f}},
      {{ 0.5f,  0.5f,  0.5f},  {0.0f,  0.0f, 1.0f},   {1.0f, 1.0f}},
      {{ 0.5f,  0.5f,  0.5f},  {0.0f,  0.0f, 1.0f},   {1.0f, 1.0f}},
      {{-0.5f,  0.5f,  0.5f},  {0.0f,  0.0f, 1.0f},   {0.0f, 1.0f}},
      {{-0.5f, -0.5f,  0.5f},  {0.0f,  0.0f, 1.0f},   {0.0f, 0.0f}},

      {{-0.5f,  0.5f,  0.5f}, {-1.0f,  0.0f,  0.0f},  {1.0f, 0.0f}},
      {{-0.5f,  0.5f, -0.5f}, {-1.0f,  0.0f,  0.0f},  {1.0f, 1.0f}},
      {{-0.5f, -0.5f, -0.5f}, {-1.0f,  0.0f,  0.0f},  {0.0f, 1.0f}},
      {{-0.5f, -0.5f, -0.5f}, {-1.0f,  0.0f,  0.0f},  {0.0f, 1.0f}},
      {{-0.5f, -0.5f,  0.5f}, {-1.0f,  0.0f,  0.0f},  {0.0f, 0.0f}},
      {{-0.5f,  0.5f,  0.5f}, {-1.0f,  0.0f,  0.0f},  {1.0f, 0.0f}},

      {{ 0.5f,  0.5f,  0.5f},  {1.0f,  0.0f,  0.0f},  {1.0f, 0.0f}},
      {{ 0.5f,  0.5f, -0.5f},  {1.0f,  0.0f,  0.0f},  {1.0f, 1.0f}},
      {{ 0.5f, -0.5f, -0.5f},  {1.0f,  0.0f,  0.0f},  {0.0f, 1.0f}},
      {{ 0.5f, -0.5f, -0.5f},  {1.0f,  0.0f,  0.0f},  {0.0f, 1.0f}},
      {{ 0.5f, -0.5f,  0.5f},  {1.0f,  0.0f,  0.0f},  {0.0f, 0.0f}},
      {{ 0.5f,  0.5f,  0.5f},  {1.0f,  0.0f,  0.0f},  {1.0f, 0.0f}},

      {{-0.5f, -0.5f, -0.5f},  {0.0f, -1.0f,  0.0f},  {0.0f, 1.0f}},
      {{ 0.5f, -0.5f, -0.5f},  {0.0f, -1.0f,  0.0f},  {1.0f, 1.0f}},
      {{ 0.5f, -0.5f,  0.5f},  {0.0f, -1.0f,  0.0f},  {1.0f, 0.0f}},
      {{ 0.5f, -0.5f,  0.5f},  {0.0f, -1.0f,  0.0f},  {1.0f, 0.0f}},
      {{-0.5f, -0.5f,  0.5f},  {0.0f, -1.0f,  0.0f},  {0.0f, 0.0f}},
      {{-0.5f, -0.5f, -0.5f},  {0.0f, -1.0f,  0.0f},  {0.0f, 1.0f}},

      {{-0.5f,  0.5f, -0.5f},  {0.0f,  1.0f,  0.0f},  {0.0f, 1.0f}},
      {{ 0.5f,  0.5f, -0.5f},  {0.0f,  1.0f,  0.0f},  {1.0f, 1.0f}},
      {{ 0.5f,  0.5f,  0.5f},  {0.0f,  1.0f,  0.0f},  {1.0f, 0.0f}},
      {{ 0.5f,  0.5f,  0.5f},  {0.0f,  1.0f,  0.0f},  {1.0f, 0.0f}},
      {{-0.5f,  0.5f,  0.5f},  {0.0f,  1.0f,  0.0f},  {0.0f, 0.0f}},
      {{-0.5f,  0.5f, -0.5f},  {0.0f,  1.0f,  0.0f},  {0.0f, 1.0f}}
    };
    std::vector<unsigned int> indices(36);
    std::iota(indices.begin(), indices.end(), 0);

    glm::vec3 lampPositions[] = {
      glm::vec3( 0.7f,  0.2f,  2.0f),
      glm::vec3( 2.3f, -3.3f, -4.0f),
      glm::vec3(-4.0f,  2.0f, -12.0f),
      glm::vec3( 0.0f,  0.0f, -3.0f)
    };

    Mesh lamp(vertices, indices, {});
    Shader lampShader("shaders/lamp.vs", "shaders/lamp.fs");

    //load suit model
    Model suit("textures/suit/nanosuit.obj");
    Shader suitShader("shaders/suit.vs", "shaders/suit.fs");
    suitShader.use();

    suitShader.setUniform("material.shininess", 32.0f);
    //set dirLight
    suitShader.setUniform("dirLight.direction", -0.2f, -1.0f, -0.3f);
    suitShader.setUniform("dirLight.ambient", glm::vec3(0.05f));
    suitShader.setUniform("dirLight.diffuse", glm::vec3(0.4f));
    suitShader.setUniform("dirLight.specular", glm::vec3(0.5f));
    //set pointLight
    suitShader.setUniform("pointLight[0].position", lampPositions[0]);
    suitShader.setUniform("pointLight[1].position", lampPositions[1]);
    suitShader.setUniform("pointLight[2].position", lampPositions[2]);
    suitShader.setUniform("pointLight[3].position", lampPositions[3]);
    suitShader.setUniform("pointLight[0].constant", 1.0f);
    suitShader.setUniform("pointLight[1].constant", 1.0f);
    suitShader.setUniform("pointLight[2].constant", 1.0f);
    suitShader.setUniform("pointLight[3].constant", 1.0f);
    suitShader.setUniform("pointLight[0].linear", 0.09f);
    suitShader.setUniform("pointLight[1].linear", 0.09f);
    suitShader.setUniform("pointLight[2].linear", 0.09f);
    suitShader.setUniform("pointLight[3].linear", 0.09f);
    suitShader.setUniform("pointLight[0].quadratic", 0.032f);
    suitShader.setUniform("pointLight[1].quadratic", 0.032f);
    suitShader.setUniform("pointLight[2].quadratic", 0.032f);
    suitShader.setUniform("pointLight[3].quadratic", 0.032f);
    suitShader.setUniform("pointLight[0].ambient", glm::vec3(0.05f));
    suitShader.setUniform("pointLight[1].ambient", glm::vec3(0.05f));
    suitShader.setUniform("pointLight[2].ambient", glm::vec3(0.05f));
    suitShader.setUniform("pointLight[3].ambient", glm::vec3(0.05f));
    suitShader.setUniform("pointLight[0].diffuse", glm::vec3(0.8f));
    suitShader.setUniform("pointLight[1].diffuse", glm::vec3(0.8f));
    suitShader.setUniform("pointLight[2].diffuse", glm::vec3(0.8f));
    suitShader.setUniform("pointLight[3].diffuse", glm::vec3(0.8f));
    suitShader.setUniform("pointLight[0].specular", glm::vec3(1.0f));
    suitShader.setUniform("pointLight[1].specular", glm::vec3(1.0f));
    suitShader.setUniform("pointLight[2].specular", glm::vec3(1.0f));
    suitShader.setUniform("pointLight[3].specular", glm::vec3(1.0f));
    //set spotLight
    suitShader.setUniform("spotLight.cutoff", glm::cos(glm::radians(12.5f)));
    suitShader.setUniform("spotLight.outerCutoff", glm::cos(glm::radians(15.0f)));
    suitShader.setUniform("spotLight.constant", 1.0f);
    suitShader.setUniform("spotLight.linear", 0.09f);
    suitShader.setUniform("spotLight.quadratic", 0.032f);
    suitShader.setUniform("spotLight.ambient", glm::vec3(0.0f));
    suitShader.setUniform("spotLight.diffuse", glm::vec3(1.0f));
    suitShader.setUniform("spotLight.specular", glm::vec3(1.0f));

    //load suit model
    Model suitScaled("textures/suit/nanosuit.obj");
    Shader suitScaledShader("shaders/suit.vs", "shaders/color.fs");
    suitScaledShader.use();

    //create a camera
    Camera camera(scene.window);

    //enable tests
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    //rendering loop
    while(!glfwWindowShouldClose(scene.window)){
    	//input
    	scene.processInput();
      camera.update();

    	//clear screen
    	glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
    	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
      glStencilMask(0x00);

      //draw lamps
      lampShader.use();
      lampShader.setUniform("view", camera.getView());
      lampShader.setUniform("projection", camera.getProjection());
      for(auto i=0; i<4; ++i){
        auto model = glm::mat4(1.0f);
        model = glm::translate(model, lampPositions[i]);
        model = glm::scale(model, glm::vec3(0.2f));
        lampShader.setUniform("model", model);

        lamp.draw(lampShader);
      }

      //draw suit
      suitShader.use();
      auto model = glm::mat4(1.0f);
      model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
      model = glm::scale(model, glm::vec3(0.2f));
      suitShader.setUniform("model", model);
      suitShader.setUniform("spotLight.position", camera.getPos());
      suitShader.setUniform("spotLight.direction", camera.getFront());
      suitShader.setUniform("viewPos", camera.getPos());
      suitShader.setUniform("view", camera.getView());
      suitShader.setUniform("projection", camera.getProjection());

      //set stencil
      glStencilFunc(GL_ALWAYS, 1, 0xFF);
      glStencilMask(0xFF);
      suit.draw(suitShader);

      //draw suitScaled
      suitScaledShader.use();
      model = glm::scale(model, glm::vec3(1.004f));
      suitScaledShader.setUniform("model", model);
      suitScaledShader.setUniform("view", camera.getView());
      suitScaledShader.setUniform("projection", camera.getProjection());

      glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
      glStencilMask(0x00);
      suitScaledShader.use();
      suitScaled.draw(suitScaledShader);

      //reset tests
      glStencilMask(0xFF);

    	//check status
    	glfwSwapBuffers(scene.window);
    	glfwPollEvents();
    }

    suitShader.deleteProgram();

    glfwTerminate();

    return 0;
}