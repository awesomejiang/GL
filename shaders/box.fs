#version 330 core

in vec3 objPos;
in vec3 norm;
in vec2 texCoords;

out vec4 FragColor;

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

uniform vec3 viewPos;
uniform Material material;

uniform DirLight dirLight;
uniform PointLight pointLight[4];
uniform SpotLight spotLight;

vec3 computeDirLight(DirLight light){
   vec3 lightDir = normalize(-light.direction);

   //ambient
   vec3 ambient = light.ambient;

   //diffuse
   float diffStr = max(0.0, dot(lightDir, normalize(norm)));
   vec3 diffuse = diffStr * light.diffuse * vec3(texture(material.textureDiffuse1, texCoords));

   //specular
   vec3 viewDir = normalize(viewPos - objPos);
   vec3 reflectDir = reflect(-lightDir, normalize(norm));
   float specStr = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
   vec3 specular = specStr * light.specular * vec3(texture(material.textureSpecular1, texCoords));

   return ambient + diffuse + specular;
}

vec3 computePointLight(PointLight light){
   //set 3 kind of lights
   vec3 lightDir = normalize(light.position - objPos);
   //ambient
   vec3 ambient = light.ambient * vec3(texture(material.textureDiffuse1, texCoords));

   //diffuse
   float diffStr = max(0.0, dot(lightDir, normalize(norm)));
   vec3 diff = diffStr * light.diffuse * vec3(texture(material.textureDiffuse1, texCoords));

   //specular
   vec3 viewDir = normalize(viewPos - objPos);
   vec3 reflectDir = reflect(-lightDir, normalize(norm));
   float specStr = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
   vec3 specular = specStr * light.specular * vec3(texture(material.textureSpecular1, texCoords));

   //attenuation
   float dist = length(vec3(light.position) - objPos);
   float attenuation = 1.0/(light.constant + light.linear * dist + light.quadratic * dist * dist);

   return (ambient + diff+specular) * attenuation;
}

vec3 computeSpotLight(SpotLight light){
   //set 3 kind of lights
   vec3 lightDir = normalize(light.position - objPos);
   //ambient
   vec3 ambient = light.ambient * vec3(texture(material.textureDiffuse1, texCoords));

   //diffuse
   float diffStr = max(0.0, dot(lightDir, normalize(norm)));
   vec3 diff = diffStr * light.diffuse * vec3(texture(material.textureDiffuse1, texCoords));

   //specular
   vec3 viewDir = normalize(viewPos - objPos);
   vec3 reflectDir = reflect(-lightDir, normalize(norm));
   float specStr = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
   vec3 specular = specStr * light.specular * vec3(texture(material.textureSpecular1, texCoords));

   //attenuation
   float dist = length(vec3(light.position) - objPos);
   float attenuation = 1.0/(light.constant + light.linear * dist + light.quadratic * dist * dist);

   //spotlight
   float theta = dot(lightDir, normalize(-light.direction));
   float intensity = clamp((theta-light.outerCutoff)/(light.cutoff-light.outerCutoff), 0.0, 1.0);

   return (ambient + (diff+specular)*intensity) * attenuation;
}

void main(){
   vec3 res = computeDirLight(dirLight);

   for(int i=0; i<4; ++i){
      res += computePointLight(pointLight[i]);
   }

   res += computeSpotLight(spotLight);

   FragColor = vec4(res, 1.0);
}
