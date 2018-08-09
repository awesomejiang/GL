#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNorm;
layout(location = 2) in vec2 aTex;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 norm;
out vec3 objPos;
out vec2 texCoords;

void main(){
   gl_Position = projection * view * model * vec4(aPos, 1.0);
   objPos = vec3(model * vec4(aPos, 1.0));
   norm = mat3(transpose(inverse(model))) * aNorm;
   texCoords = aTex;
}