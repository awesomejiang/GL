#include "object.h"

Object::Object(){
    //generate EBO, VBO, VAO, wait to be set.
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
}


void Object::setVBO(std::vector<float> const &vertices) const{
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(float), vertices.data(), GL_STATIC_DRAW); 
}


void Object::setEBO(std::vector<int> const &indices) const{
    //glBindVertexArray(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(int), indices.data(), GL_STATIC_DRAW);
}


void Object::setVAO(unsigned int loc, long int size, size_t stride, long int offset) const{
    glBindVertexArray(VAO);

    //position
    glVertexAttribPointer(loc, size, GL_FLOAT, GL_FALSE, stride*sizeof(float), (void*)(offset*sizeof(float)));
    glEnableVertexAttribArray(loc);
}


void Object::clearBuffers(){
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}


void Object::draw(int n) const{
    //draw triangle
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, n, GL_UNSIGNED_INT, 0);

}


Object::~Object(){
    //TODO: If clear buffer at destructor -> segmentation fault.
	//clearBuffers();
}