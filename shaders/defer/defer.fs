#version 330 core

// ugly hack: if we enable blend, all vec3s have to change to vec4 with a=1.0 and
// also, we can not use gSpec.a store shininess.
layout(location = 0) out vec4 gPosition;
layout(location = 1) out vec4 gNormal;
layout(location = 2) out vec4 gDiff;
layout(location = 3) out vec4 gSpec;
layout(location = 4) out vec4 gShin;

in VS_OUT {
	vec2 texCoords;
	vec3 fragPos;
	vec3 normal;
} fs_in;

struct Material{
   sampler2D textureDiffuse1;
   sampler2D textureSpecular1;
   float shininess;
};

uniform Material material;

void main(){
	//gPosition = fs_in.fragPos;
	//gNormal = normalize(fs_in.normal);
	gPosition = vec4(fs_in.fragPos, 1.0);
	gNormal = vec4(normalize(fs_in.normal), 1.0);
	gDiff = texture(material.textureDiffuse1, fs_in.texCoords);
	gSpec = texture(material.textureSpecular1, fs_in.texCoords);
	gShin = vec4(vec3(material.shininess), 1.0);
}