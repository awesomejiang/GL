#version 330 core

in vec4 fragPos;

uniform float farPlane;
uniform vec3 lightPos;

void main(){
	gl_FragDepth = length(fragPos.xyz - lightPos)/farPlane;
}