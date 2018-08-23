#version 330 core

out vec4 fragColor;

in vec2 texCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gDiff;
uniform sampler2D gSpec;
uniform sampler2D gShin;

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

struct Property{
	vec3 fragPos;
	vec3 norm;
	vec3 diffuse;
	vec3 specular;
   float alpha;
   float shininess;
};

Property property;

uniform vec3 viewPos;

//for lights
uniform DirLight dirLight;
uniform PointLight pointLight[4];
uniform SpotLight spotLight;

//for directional shadows
uniform mat4 lightSpaceMatrix;
uniform sampler2D shadowMap;

//for omnidirectional shadows
uniform float farPlane;
uniform samplerCube shadowCube[4];

//for reflection
uniform samplerCube skybox;

void setGvals(){
	property.fragPos = texture(gPosition, texCoords).rgb;
	property.norm = normalize(texture(gNormal, texCoords).rgb);
	property.diffuse = texture(gDiff, texCoords).rgb;
   property.specular = texture(gSpec, texCoords).rgb;
   property.alpha = texture(gDiff, texCoords).a;
   property.shininess = texture(gShin, texCoords).r;
}


float computeShadowOrtho(vec4 fragPosLightSpace, sampler2D map){
   //change to proj coords
   vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
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
   vec3 fragToLight = property.fragPos - lightPos;
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
   float viewDist = length(property.fragPos - viewPos);
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
   vec3 ambient = light.ambient * property.diffuse;

   //diffuse
   float diffStr = max(0.0, dot(lightDir, normalize(property.norm)));
   vec3 diffuse = diffStr * light.diffuse * property.diffuse;

   //specular
   vec3 viewDir = normalize(viewPos - property.fragPos);
   vec3 halfwayDir = normalize(lightDir + viewDir);
   float specStr = pow(max(dot(property.norm, halfwayDir), 0.0), property.shininess);
   vec3 specular = specStr * light.specular * property.specular;

   return ambient + (1.0-shadow)*(diffuse+specular);
}

vec3 computePointLight(PointLight light, float shadow){
   vec3 lightDir = normalize(light.position - property.fragPos);

   //ambient
   vec3 ambient = light.ambient * property.diffuse;

   //diffuse
   float diffStr = max(0.0, dot(lightDir, property.norm));
   vec3 diff = diffStr * light.diffuse * property.diffuse;

   //specular
   vec3 viewDir = normalize(viewPos - property.fragPos);
   vec3 halfwayDir = normalize(lightDir + viewDir);
   float specStr = pow(max(dot(property.norm, halfwayDir), 0.0), property.shininess);
   vec3 specular = specStr * light.specular * property.specular;

   //attenuation
   float dist = length(vec3(light.position) - property.fragPos);
   float attenuation = 1.0/(light.constant + light.linear * dist + light.quadratic * dist * dist);

   return (ambient + (1.0-shadow)*(diff+specular)) * attenuation;
}

vec3 computeSpotLight(SpotLight light, float shadow){
   vec3 lightDir = normalize(light.position - property.fragPos);
   
   //ambient
   vec3 ambient = light.ambient * property.diffuse;

   //diffuse
   float diffStr = max(0.0, dot(lightDir, property.norm));
   vec3 diff = diffStr * light.diffuse * property.diffuse;

   //specular
   vec3 viewDir = normalize(viewPos - property.fragPos);
   vec3 halfwayDir = normalize(lightDir + viewDir);
   float specStr = pow(max(dot(property.norm, halfwayDir), 0.0), property.shininess);
   vec3 specular = specStr * light.specular * property.specular;

   //attenuation
   float dist = length(vec3(light.position) - property.fragPos);
   float attenuation = 1.0/(light.constant + light.linear * dist + light.quadratic * dist * dist);

   //spotlight
   float theta = dot(lightDir, normalize(-light.direction));
   float intensity = clamp((theta-light.outerCutoff)/(light.cutoff-light.outerCutoff), 0.0, 1.0);

   return (ambient + (diff+specular)*intensity) * attenuation;
}

vec3 computeReflection(float shadow){
   vec3 I = normalize(property.fragPos - viewPos);
   vec3 R = reflect(I, property.norm);

   return (1.0-shadow) * texture(skybox, R).rgb;
}

void main(){
	//get data from gbuffer
	setGvals();

   //shadow
   float shadow = 0.0;
   vec4 fragPosLightSpace = lightSpaceMatrix * vec4(property.fragPos, 1.0f);
   shadow += computeShadowOrtho(fragPosLightSpace, shadowMap);
   for(int i=0; i<4; ++i)
      shadow += computeShadowPoint(pointLight[i].position, shadowCube[i]);
   shadow = clamp(shadow, 0.0, 1.0);
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
   fragColor = vec4(lightRes, 1.0);
   //fragColor = vec4(texture(gNormal, texCoords).rgb, 1.0);
}
