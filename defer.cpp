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
#include <utility>

#include "utility.h"
#include "shader.h"
#include "scene.h"
#include "camera.h"
#include "texture.h"
#include "mesh.h"
#include "model.h"
#include "framebuffer.h"
#include "shadow.h"
#include "gbuffer.h"

//global vars
int screenWidth = 800, screenHeight = 600;

float farPlane = 25.0f;

glm::vec3 lampPositions[] = {
  glm::vec3( 0.9f,  1.1f,  1.5f),
  glm::vec3( 2.3f, 3.3f, -4.0f),
  glm::vec3(-4.0f,  2.0f, -5.0f),
   glm::vec3( 0.0f,  1.0f, -3.0f)
};

glm::vec3 lightDir = {-2.0f, 2.0f, 2.0f};



Mesh createLamp(){
  //create lamp by hand
  return Mesh({
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
  });
}

void setLightUniforms(Shader &shader){
  shader.use();

  //set args for shadow
  shader.setUniform("farPlane", farPlane);
  //set dirLight
  shader.setUniform("dirLight.direction", lightDir);
  shader.setUniform("dirLight.ambient", glm::vec3(0.05f));
  shader.setUniform("dirLight.diffuse", glm::vec3(0.4f));
  shader.setUniform("dirLight.specular", glm::vec3(0.5f));
  //set pointLight
  for(auto i=0; i<4; ++i){
    shader.setUniform("pointLight["+std::to_string(i)+"].position", lampPositions[i]);
    shader.setUniform("pointLight["+std::to_string(i)+"].constant", 1.0f);
    shader.setUniform("pointLight["+std::to_string(i)+"].linear", 0.09f);
    shader.setUniform("pointLight["+std::to_string(i)+"].quadratic", 0.032f);
    shader.setUniform("pointLight["+std::to_string(i)+"].ambient", glm::vec3(0.05f));
    shader.setUniform("pointLight["+std::to_string(i)+"].diffuse", glm::vec3(0.8f));
    shader.setUniform("pointLight["+std::to_string(i)+"].specular", glm::vec3(1.0f));
  }
  //set spotLight
  shader.setUniform("spotLight.cutoff", glm::cos(glm::radians(12.5f)));
  shader.setUniform("spotLight.outerCutoff", glm::cos(glm::radians(15.0f)));
  shader.setUniform("spotLight.constant", 10.0f);
  shader.setUniform("spotLight.linear", 0.09f);
  shader.setUniform("spotLight.quadratic", 0.032f);
  shader.setUniform("spotLight.ambient", glm::vec3(0.0f));
  shader.setUniform("spotLight.diffuse", glm::vec3(1.0f));
  shader.setUniform("spotLight.specular", glm::vec3(1.0f));
}

Mesh createSkybox(){
  std::vector<Vertex> vertices = {
    // positions          // normals           // texture coords
    {{-1.0f, -1.0f, -1.0f},  {0.0f,  0.0f, -1.0f},  {0.0f, 0.0f}},
    {{ 1.0f, -1.0f, -1.0f},  {0.0f,  0.0f, -1.0f},  {1.0f, 0.0f}},
    {{ 1.0f,  1.0f, -1.0f},  {0.0f,  0.0f, -1.0f},  {1.0f, 1.0f}},
    {{ 1.0f,  1.0f, -1.0f},  {0.0f,  0.0f, -1.0f},  {1.0f, 1.0f}},
    {{-1.0f,  1.0f, -1.0f},  {0.0f,  0.0f, -1.0f},  {0.0f, 1.0f}},
    {{-1.0f, -1.0f, -1.0f},  {0.0f,  0.0f, -1.0f},  {0.0f, 0.0f}},

    {{-1.0f, -1.0f,  1.0f},  {0.0f,  0.0f, 1.0f},   {0.0f, 0.0f}},
    {{ 1.0f, -1.0f,  1.0f},  {0.0f,  0.0f, 1.0f},   {1.0f, 0.0f}},
    {{ 1.0f,  1.0f,  1.0f},  {0.0f,  0.0f, 1.0f},   {1.0f, 1.0f}},
    {{ 1.0f,  1.0f,  1.0f},  {0.0f,  0.0f, 1.0f},   {1.0f, 1.0f}},
    {{-1.0f,  1.0f,  1.0f},  {0.0f,  0.0f, 1.0f},   {0.0f, 1.0f}},
    {{-1.0f, -1.0f,  1.0f},  {0.0f,  0.0f, 1.0f},   {0.0f, 0.0f}},

    {{-1.0f,  1.0f,  1.0f}, {-1.0f,  0.0f,  0.0f},  {1.0f, 0.0f}},
    {{-1.0f,  1.0f, -1.0f}, {-1.0f,  0.0f,  0.0f},  {1.0f, 1.0f}},
    {{-1.0f, -1.0f, -1.0f}, {-1.0f,  0.0f,  0.0f},  {0.0f, 1.0f}},
    {{-1.0f, -1.0f, -1.0f}, {-1.0f,  0.0f,  0.0f},  {0.0f, 1.0f}},
    {{-1.0f, -1.0f,  1.0f}, {-1.0f,  0.0f,  0.0f},  {0.0f, 0.0f}},
    {{-1.0f,  1.0f,  1.0f}, {-1.0f,  0.0f,  0.0f},  {1.0f, 0.0f}},

    {{ 1.0f,  1.0f,  1.0f},  {1.0f,  0.0f,  0.0f},  {1.0f, 0.0f}},
    {{ 1.0f,  1.0f, -1.0f},  {1.0f,  0.0f,  0.0f},  {1.0f, 1.0f}},
    {{ 1.0f, -1.0f, -1.0f},  {1.0f,  0.0f,  0.0f},  {0.0f, 1.0f}},
    {{ 1.0f, -1.0f, -1.0f},  {1.0f,  0.0f,  0.0f},  {0.0f, 1.0f}},
    {{ 1.0f, -1.0f,  1.0f},  {1.0f,  0.0f,  0.0f},  {0.0f, 0.0f}},
    {{ 1.0f,  1.0f,  1.0f},  {1.0f,  0.0f,  0.0f},  {1.0f, 0.0f}},

    {{-1.0f, -1.0f, -1.0f},  {0.0f, -1.0f,  0.0f},  {0.0f, 1.0f}},
    {{ 1.0f, -1.0f, -1.0f},  {0.0f, -1.0f,  0.0f},  {1.0f, 1.0f}},
    {{ 1.0f, -1.0f,  1.0f},  {0.0f, -1.0f,  0.0f},  {1.0f, 0.0f}},
    {{ 1.0f, -1.0f,  1.0f},  {0.0f, -1.0f,  0.0f},  {1.0f, 0.0f}},
    {{-1.0f, -1.0f,  1.0f},  {0.0f, -1.0f,  0.0f},  {0.0f, 0.0f}},
    {{-1.0f, -1.0f, -1.0f},  {0.0f, -1.0f,  0.0f},  {0.0f, 1.0f}},

    {{-1.0f,  1.0f, -1.0f},  {0.0f,  1.0f,  0.0f},  {0.0f, 1.0f}},
    {{ 1.0f,  1.0f, -1.0f},  {0.0f,  1.0f,  0.0f},  {1.0f, 1.0f}},
    {{ 1.0f,  1.0f,  1.0f},  {0.0f,  1.0f,  0.0f},  {1.0f, 0.0f}},
    {{ 1.0f,  1.0f,  1.0f},  {0.0f,  1.0f,  0.0f},  {1.0f, 0.0f}},
    {{-1.0f,  1.0f,  1.0f},  {0.0f,  1.0f,  0.0f},  {0.0f, 0.0f}},
    {{-1.0f,  1.0f, -1.0f},  {0.0f,  1.0f,  0.0f},  {0.0f, 1.0f}}
  };

  return  Mesh(vertices, {}, {
    {
      {"textures/skybox/right.jpg",
       "textures/skybox/left.jpg",
       "textures/skybox/top.jpg",
       "textures/skybox/bottom.jpg",
       "textures/skybox/back.jpg",
       "textures/skybox/front.jpg"},
      "textureCube", "skybox"}
  });
}

Mesh createFloor(){
  std::vector<Vertex> vertices = {
    // positions          // normals           // texture coords
    {{-1.0f,  0.0f, -1.0f},  {0.0f,  1.0f,  0.0f},  {0.0f, 1.0f}},
    {{ 1.0f,  0.0f, -1.0f},  {0.0f,  1.0f,  0.0f},  {1.0f, 1.0f}},
    {{ 1.0f,  0.0f,  1.0f},  {0.0f,  1.0f,  0.0f},  {1.0f, 0.0f}},
    {{ 1.0f,  0.0f,  1.0f},  {0.0f,  1.0f,  0.0f},  {1.0f, 0.0f}},
    {{-1.0f,  0.0f,  1.0f},  {0.0f,  1.0f,  0.0f},  {0.0f, 0.0f}},
    {{-1.0f,  0.0f, -1.0f},  {0.0f,  1.0f,  0.0f},  {0.0f, 1.0f}}
  };

  return  Mesh(vertices, {}, {
    {"textures/brick.jpg", "textureDiffuse"},
    //{"textures/brickBump.jpg", "texture2D", "normalMap"}
  });
}

Mesh createQuad(){  
  return Mesh({
      // positions          // normals           // texture coords
      {{-1.0f, -1.0f,  0.0f},  {0.0f,  0.0f, 1.0f},   {0.0f, 0.0f}},
      {{ 1.0f, -1.0f,  0.0f},  {0.0f,  0.0f, 1.0f},   {1.0f, 0.0f}},
      {{ 1.0f,  1.0f,  0.0f},  {0.0f,  0.0f, 1.0f},   {1.0f, 1.0f}},
      {{ 1.0f,  1.0f,  0.0f},  {0.0f,  0.0f, 1.0f},   {1.0f, 1.0f}},
      {{-1.0f,  1.0f,  0.0f},  {0.0f,  0.0f, 1.0f},   {0.0f, 1.0f}},
      {{-1.0f, -1.0f,  0.0f},  {0.0f,  0.0f, 1.0f},   {0.0f, 0.0f}}
  });
}

int main(){
    //init scene
    Scene scene(screenWidth, screenHeight);

    //create a camera
    Camera camera(scene.window);

    //create shadow objects
    Shadow<orthographic> shadowDir(lightDir, farPlane);
    std::vector<Shadow<perspective>> shadowPts = {
      {lampPositions[0], farPlane},
      {lampPositions[1], farPlane},
      {lampPositions[2], farPlane},
      {lampPositions[3], farPlane},
    };

    //create skybox
    Mesh skybox = createSkybox();
    Shader skyboxShader("shaders/theresa/skybox.vs", "shaders/theresa/skybox.fs");

    //create lamp
    Mesh lamp = createLamp();
    Shader lampShader("shaders/theresa/lamp.vs", "shaders/theresa/lamp.fs");

    //create floor
    Mesh floor = createFloor();
    floor.setTexture({shadowDir.getDepthMap(), "texture2D", "shadowMap"});
    for(auto i=0; i<4; ++i)
      floor.setTexture({shadowPts[i].getDepthMap(), "textureCube", "shadowCube["+std::to_string(i)+"]"});

    //load body model
    Model body("textures/theresa/theresa.obj");
    body.setTexture({shadowDir.getDepthMap(), "texture2D", "shadowMap"});
    for(auto i=0; i<4; ++i)
      body.setTexture({shadowPts[i].getDepthMap(), "textureCube", "shadowCube["+std::to_string(i)+"]"});

    //create gbuffer
    Gbuffer gbuffer(screenWidth, screenHeight, 
        std::make_pair("gPosition", GL_RGBA16F),
        std::make_pair("gNormal", GL_RGBA16F),
        std::make_pair("gDiff", GL_RGBA16F),
        std::make_pair("gSpec", GL_RGBA16F),
        std::make_pair("gShin", GL_RGBA16F)
    );
    Shader gbufferShader("shaders/defer/defer.vs", "shaders/defer/defer.fs");
    Shader lightShader("shaders/defer/light.vs", "shaders/defer/light.fs");
    setLightUniforms(lightShader);
    lightShader.setUniform("lightSpaceMatrix", shadowDir.lightSpaceMatrix);

    //create quad
    Mesh quad = createQuad();
    gbuffer.setTextures(quad);
    quad.setTexture({shadowDir.getDepthMap(), "texture2D", "shadowMap"});
    for(auto i=0; i<4; ++i)
      quad.setTexture({shadowPts[i].getDepthMap(), "textureCube", "shadowCube["+std::to_string(i)+"]"});

    //rendering loop
    while(!glfwWindowShouldClose(scene.window)){
    	//input
    	scene.processInput();
      camera.update();

      //compute model mats
      glm::mat4 lampModel[4];
      for(auto i=0; i<4; ++i){
        lampModel[i] = glm::mat4(1.0f);
        lampModel[i] = glm::translate(lampModel[i], lampPositions[i]);
        lampModel[i] = glm::scale(lampModel[i], glm::vec3(0.2f));
      }

      auto floorModel = glm::mat4(1.0f);
      floorModel = glm::scale(floorModel, glm::vec3(10.0f));

      auto bodyModel = glm::mat4(1.0f);
      //bodyModel = glm::translate(bodyModel, glm::vec3(0.0f, -1.75f, 0.0f));

      //first pass: draw depth map
      shadowDir.bind();

      glCullFace(GL_FRONT); //for eliminate peter-panning
      shadowDir.setDepth(floor, floorModel);
      shadowDir.setDepth(body, bodyModel);
      
      glCullFace(GL_BACK);
      shadowDir.unbind();

      for(auto &shadow: shadowPts){
        shadow.bind();

        glCullFace(GL_FRONT); //for eliminate peter-panning
        shadow.setDepth(floor, floorModel);
        shadow.setDepth(body, bodyModel);
        
        glCullFace(GL_BACK);
        shadow.unbind();
      }

      //second pass: render objects into gbuffer
      gbuffer.bind();
      glViewport(0, 0, screenWidth, screenHeight);
    	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      glEnable(GL_DEPTH_TEST);

      gbufferShader.use();
      gbufferShader.setUniform("projection", camera.getProjection());
      gbufferShader.setUniform("view", camera.getView());

        //floor
      gbufferShader.setUniform("model", floorModel);
      gbufferShader.setUniform("material.shininess", 10000.0f);
      floor.draw(gbufferShader);

        //body
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      gbufferShader.setUniform("model", bodyModel);
      gbufferShader.setUniform("material.shininess", 10000.0f);
      body.draw(gbufferShader);
      glDisable(GL_BLEND);

      gbuffer.unbind();

      //third pass: lighting pass
      glViewport(0, 0, screenWidth, screenHeight);
      glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      lightShader.use();
      lightShader.setUniform("spotLight.position", camera.getPos());
      lightShader.setUniform("spotLight.direction", camera.getFront());
      lightShader.setUniform("viewPos", camera.getPos());
      quad.draw(lightShader);

      //skybox
      gbuffer.blitData(0, GL_DEPTH_BUFFER_BIT); //copy depth value from gbuffer
      glDepthFunc(GL_LEQUAL);
      skyboxShader.use();
      skyboxShader.setUniform("view", glm::mat4(glm::mat3(camera.getView())));
      skyboxShader.setUniform("projection", camera.getProjection());
      skybox.draw(skyboxShader);
      glDepthFunc(GL_LESS);

      //debugQuad(gbuffer.textures[1]);
      //debugQuad(shadowDir.getDepthMap());

    	//check status
    	glfwSwapBuffers(scene.window);
    	glfwPollEvents();
    }

    gbufferShader.deleteProgram();
    skyboxShader.deleteProgram();
    lampShader.deleteProgram();

    glfwTerminate();

    return 0;
}