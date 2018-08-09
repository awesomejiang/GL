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

#include "shader.h"
#include "object.h"
#include "scene.h"
#include "texture.h"
#include "camera.h"

using namespace std;


int main(){
    //init scene
    Scene scene(800, 600);

    //enable z buffer
    glEnable(GL_DEPTH_TEST);

    Object boxes;

    //setup vertex data and configure vertex attr
    boxes.setVBO({
    // positions          // colors           // texture coords
      //front
       0.5f,  0.5f,  0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
       0.5f, -0.5f,  0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
      -0.5f, -0.5f,  0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
      -0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,   // top left 
      //top
       0.5f,  0.5f,  0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
       0.5f,  0.5f, -0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
      -0.5f,  0.5f, -0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
      -0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,   // top left 
      //bottom
       0.5f, -0.5f,  0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
       0.5f, -0.5f, -0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
      -0.5f, -0.5f, -0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
      -0.5f, -0.5f,  0.5f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,   // top left 
      //left
      -0.5f,  0.5f,  0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
      -0.5f, -0.5f,  0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
      -0.5f, -0.5f, -0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
      -0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,   // top left 
      //right
       0.5f,  0.5f,  0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
       0.5f, -0.5f,  0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
       0.5f, -0.5f, -0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
       0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,   // top left 
      //back
       0.5f,  0.5f, -0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
       0.5f, -0.5f, -0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
      -0.5f, -0.5f, -0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
      -0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
    });

    boxes.setEBO({
      0, 1, 3,
      1, 2, 3,

      4, 5, 7,
      5, 6, 7,

      8, 9, 11,
      9,10, 11,

      12, 13, 15,
      13, 14, 15,

      16, 17, 19,
      17, 18, 19,

      20, 21, 23,
      21, 22, 23
    });

    //set attribs
    //position
    boxes.setVAO(0, 3, 8, 0);
    //color
    boxes.setVAO(1, 3, 8, 3);
    //texture
    boxes.setVAO(2, 2, 8, 6);
    Texture texture1("textures/container.jpg", GL_TEXTURE0);
    Texture texture2("textures/awesomeface.png", GL_TEXTURE1);

    //build program shader
    Shader sh("shaders/first.vs", "shaders/first.fs");

    //cube positions
    glm::vec3 cubePositions[] = {
      glm::vec3( 0.0f,  0.0f,  0.0f), 
      glm::vec3( 2.0f,  5.0f, -15.0f), 
      glm::vec3(-1.5f, -2.2f, -2.5f),  
      glm::vec3(-3.8f, -2.0f, -12.3f),  
      glm::vec3( 2.4f, -0.4f, -3.5f),  
      glm::vec3(-1.7f,  3.0f, -7.5f),  
      glm::vec3( 1.3f, -2.0f, -2.5f),  
      glm::vec3( 1.5f,  2.0f, -2.5f), 
      glm::vec3( 1.5f,  0.2f, -1.5f), 
      glm::vec3(-1.3f,  1.0f, -1.5f)  
    };

    //create a camera
    Camera camera(scene.window);

    //rendering loop
    while(!glfwWindowShouldClose(scene.window)){
    	//input
    	scene.processInput();
      camera.update();

    	//clear screen
    	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      //bind texture
      texture1.bind();
      texture2.bind();

      //use shader
    	sh.use();

    	//set color each iteration
    	float timeValue = glfwGetTime();
    	float rate = (sin(timeValue)/2.0f) + 0.5f;
      sh.setUniform("rate", rate);
      sh.setUniform("texture1", 0);
      sh.setUniform("texture2", 1);

      sh.setUniform("projection", camera.getProjection());

      for(auto i=0; i<10; ++i){
        auto objView = glm::translate(camera.getView(), cubePositions[i]);
        sh.setUniform("view", objView);
        //build trans matrices and send them to vertex shader
        auto model = glm::mat4(1.0f);
        model = glm::rotate(model, timeValue*glm::radians(5.0f-10.0f*i), glm::vec3(1.0f*i, 0.3f*i, 0.5f*i));
        sh.setUniform("model", model);
        //draw on window
        boxes.draw(36);
      }

    	//check status
    	glfwSwapBuffers(scene.window);
    	glfwPollEvents();
    }

    boxes.clearBuffers();

    sh.deleteProgram();

    glfwTerminate();

    return 0;
}