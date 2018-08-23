#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNorm;
layout(location = 2) in vec2 aTex;

out VS_OUT {
	vec3 norm;
	vec3 fragPos;
	vec2 texCoords;
	vec4 fragPosLightSpace;
} vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpaceMatrix;

uniform sampler2D normalMap;

void main(){
   gl_Position = projection * view * model * vec4(aPos, 1.0);
   vs_out.fragPos = vec3(model * vec4(aPos, 1.0));
   vs_out.norm = mat3(transpose(inverse(model))) * aNorm;
   vs_out.texCoords = aTex;
   vs_out.fragPosLightSpace = lightSpaceMatrix * vec4(vs_out.fragPos, 1.0f);

   //vs_out.norm = texture(normalMap, vs_out.texCoords).rgb;
}