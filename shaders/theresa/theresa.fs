#version 330 core

struct Material{
   sampler2D textureDiffuse1;
   sampler2D textureSpecular1;
   float shininess;
};

struct DirLight{
   vec3 direction;

   vec3 ambient;
   vec3 diffuse;
   vec3 specular;
};

struct PointLight{
   vec3 position;

   vec3 ambient;
   vec3 diffuse;
   vec3 specular;

   float constant;
   float linear;
   float quadratic;
};

struct SpotLight{
   vec3 position;
   vec3 direction;

   float cutoff;
   float outerCutoff;

   vec3 ambient;
   vec3 diffuse;
   vec3 specular;

   float constant;
   float linear;
   float quadratic;
};

in VS_OUT{
   vec3 norm;
   vec3 fragPos;
   vec2 texCoords;
   vec4 fragPosLightSpace;
} fs_in;

out vec4 fragColor;

uniform vec3 viewPos;

//for material
uniform Material material;

//for lights
uniform DirLight dirLight;
uniform PointLight pointLight[4];
uniform SpotLight spotLight;

//for directional shadows
uniform sampler2D shadowMap;

//for omnidirectional shadows
uniform float farPlane;
uniform samplerCube shadowCube[4];

//for reflection
uniform samplerCube skybox;

float computeShadowOrtho(sampler2D map){
   //change to proj coords
   vec3 projCoords = fs_in.fragPosLightSpace.xyz / fs_in.fragPosLightSpace.w;
   projCoords = projCoords * 0.5 + 0.5;
   if(projCoords.z > 1.0)  //no shadow outside depth map
      return 0.0;

   float currentDepth = projCoords.z;

   //implement pcf here
   float bias = 0.01;
   float shadow = 0.0;
   vec2 texelSize = 1.0/textureSize(map, 0);
   for(int x=-1; x<1; ++x)
      for(int y=-1; y<1; ++y){
         float pcfDepth = texture(map, projCoords.xy+vec2(x,y)*texelSize).r;
         shadow += currentDepth - bias > pcfDepth? 1.0: 0.0;
      }
   shadow /= 9.0;

   //return 0.0;
   return shadow;
}

float computeShadowPoint(vec3 lightPos, samplerCube cube){
   //current depth
   vec3 fragToLight = fs_in.fragPos - lightPos;
   float currentDepth = length(fragToLight);

   //pcf
   vec3 sampleOffsetDirections[20] = vec3[](
      vec3( 1, 1, 1), vec3( 1, -1, 1), vec3(-1, -1, 1), vec3(-1, 1, 1),
      vec3( 1, 1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
      vec3( 1, 1, 0), vec3( 1, -1, 0), vec3(-1, -1, 0), vec3(-1, 1, 0),
      vec3( 1, 0, 1), vec3(-1, 0, 1), vec3( 1, 0, -1), vec3(-1, 0, -1),
      vec3( 0, 1, 1), vec3( 0, -1, 1), vec3( 0, -1, -1), vec3( 0, 1, -1)
   );
   float bias = 0.15;
   int samples= 20;
   float viewDist = length(fs_in.fragPos - viewPos);
   float diskRadius = (1.0 + (viewDist / farPlane)) / 25.0;

   float shadow = 0.0;
   for(int i=0; i<samples; ++i){
      float pcfDepth = texture(cube, fragToLight+sampleOffsetDirections[i]*diskRadius).r * farPlane;
      shadow += currentDepth - bias > pcfDepth? 1.0: 0.0;
   }
   shadow /= float(samples);

   //return 0.0;
   return shadow;
}

vec3 computeDirLight(DirLight light, float shadow){
   vec3 lightDir = normalize(-light.direction);

   //ambient
   vec3 ambient = light.ambient * vec3(texture(material.textureDiffuse1, fs_in.texCoords));

   //diffuse
   float diffStr = max(0.0, dot(lightDir, normalize(fs_in.norm)));
   vec3 diffuse = diffStr * light.diffuse * vec3(texture(material.textureDiffuse1, fs_in.texCoords));

   //specular
   vec3 viewDir = normalize(viewPos - fs_in.fragPos);
   vec3 reflectDir = reflect(-lightDir, normalize(fs_in.norm));
   float specStr = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
   vec3 specular = specStr * light.specular * vec3(texture(material.textureSpecular1, fs_in.texCoords));

   return ambient + (1.0-shadow)*(diffuse+specular);
}

vec3 computePointLight(PointLight light, float shadow){
   //set 3 kind of lights
   vec3 lightDir = normalize(light.position - fs_in.fragPos);
   //ambient
   vec3 ambient = light.ambient * vec3(texture(material.textureDiffuse1, fs_in.texCoords));

   //diffuse
   float diffStr = max(0.0, dot(lightDir, normalize(fs_in.norm)));
   vec3 diff = diffStr * light.diffuse * vec3(texture(material.textureDiffuse1, fs_in.texCoords));

   //specular
   vec3 viewDir = normalize(viewPos - fs_in.fragPos);
   vec3 reflectDir = reflect(-lightDir, normalize(fs_in.norm));
   float specStr = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
   vec3 specular = specStr * light.specular * vec3(texture(material.textureSpecular1, fs_in.texCoords));

   //attenuation
   float dist = length(vec3(light.position) - fs_in.fragPos);
   float attenuation = 1.0/(light.constant + light.linear * dist + light.quadratic * dist * dist);

   return (ambient + (1.0-shadow)*(diff+specular)) * attenuation;
}

vec3 computeSpotLight(SpotLight light, float shadow){
   //set 3 kind of lights
   vec3 lightDir = normalize(light.position - fs_in.fragPos);
   //ambient
   vec3 ambient = light.ambient * vec3(texture(material.textureDiffuse1, fs_in.texCoords));

   //diffuse
   float diffStr = max(0.0, dot(lightDir, normalize(fs_in.norm)));
   vec3 diff = diffStr * light.diffuse * vec3(texture(material.textureDiffuse1, fs_in.texCoords));

   //specular
   vec3 viewDir = normalize(viewPos - fs_in.fragPos);
   vec3 reflectDir = reflect(-lightDir, normalize(fs_in.norm));
   float specStr = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
   vec3 specular = specStr * light.specular * vec3(texture(material.textureSpecular1, fs_in.texCoords));

   //attenuation
   float dist = length(vec3(light.position) - fs_in.fragPos);
   float attenuation = 1.0/(light.constant + light.linear * dist + light.quadratic * dist * dist);

   //spotlight
   float theta = dot(lightDir, normalize(-light.direction));
   float intensity = clamp((theta-light.outerCutoff)/(light.cutoff-light.outerCutoff), 0.0, 1.0);

   return (ambient + (diff+specular)*intensity) * attenuation;
}

vec3 computeReflection(float shadow){
   vec3 I = normalize(fs_in.fragPos - viewPos);
   vec3 R = reflect(I, normalize(fs_in.norm));

   return (1.0-shadow) * texture(skybox, R).rgb;
}

void main(){
   //shadow
   float shadow = 0.0;
   shadow += computeShadowOrtho(shadowMap);
   for(int i=0; i<4; ++i)
      shadow += computeShadowPoint(pointLight[i].position, shadowCube[i]);
   shadow /= 5.0;
   shadow = 0.0;

   //lights
   vec3 lightRes;
   lightRes += computeDirLight(dirLight, shadow);
   for(int i=0; i<4; ++i){
      lightRes += computePointLight(pointLight[i], shadow);
   }
   //lightRes += computeSpotLight(spotLight, shadow);

   //reflection
   //lightRes = computeReflection(shadow);

   //final result
   float alpha = texture(material.textureDiffuse1, fs_in.texCoords).a;
   fragColor = vec4(lightRes, alpha);
}
