#version 330 core
out vec4 FragColor;
in vec2 TexCoords;


uniform sampler2D texture2;

uniform vec3 viewPos;//世界坐标中摄像机位置
in vec3 FragPos;//世界坐标中的定点位置
in vec3 Normal;
struct Material {
   vec3 ambient;
   sampler2D diffuse;
   sampler2D specular;
   float shininess;
}; 

//材质属性
uniform Material material;

uniform bool blinn;

struct DirLight {
   vec3 direction;//光方向
   vec3 ambient;//环境光
   vec3 diffuse;//漫反射光
   vec3 specular;//镜面光
};

struct PointLight {
   vec3 position;//点光源位置
   
   float constant;
   float linear;
   float quadratic;

   vec3 ambient;
   vec3 diffuse;
   vec3 specular; 
};

struct SpotLight{
   vec3 position ;
   vec3 direction;
   float cutOff;
   float outerCutOff;

   float constant;
   float linear;
   float quadratic;

   vec3 ambient;
   vec3 diffuse;
   vec3 specular;
};

#define NR_POINT_LIGHTS 4

uniform DirLight dirLight;//一个平行光
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLight;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);//处理平行光
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);//处理点光源
vec3 CalcSpotLight(SpotLight light,vec3 normal,vec3 fragPos, vec3 viewDir);//聚光

void main()
{
   vec3 norm = normalize(Normal);
   vec3 viewDir = normalize(viewPos - FragPos);
   vec3 result = vec3(0.0f);
   result = CalcDirLight(dirLight, norm, viewDir);
   for(int i = 0; i< NR_POINT_LIGHTS;i++ ){
      result += CalcPointLight(pointLights[i],norm, FragPos ,viewDir);
   }
   result += CalcSpotLight(spotLight,norm,FragPos,viewDir);
   FragColor = vec4(result, 1.0f);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir){

      //环境光
   vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;//环境光材料的初始值可以用漫反射的值
    //漫反射
   vec3 lightDir = normalize(-light.direction);
   float diff = max(dot(normal, lightDir), 0.0);
   vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;
   //镜面光照
   float spec;
   if(blinn){
      vec3 halfDir = normalize(lightDir + viewDir);  
      spec = pow(max(dot(viewDir, halfDir), 0.0), material.shininess);
   }else{
      vec3 reflectDir = reflect(-lightDir, normal);
      spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
   }
   vec3 specular = texture(material.specular, TexCoords).rgb * spec * light.specular;

   vec3 result = (ambient + diffuse + specular) ;
   return result;
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir){
   vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;//环境光材料的初始值可以用漫反射的值
    //漫反射
   vec3 lightDir = normalize(light.position - fragPos);
   float diff = max(dot(normal, lightDir), 0.0);
   vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;
   //镜面光照
   float spec;
   if(blinn){
      vec3 halfDir = normalize(lightDir + viewDir);  
      spec = pow(max(dot(viewDir, halfDir), 0.0), material.shininess);
   }else{
      vec3 reflectDir = reflect(-lightDir, normal);
      spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
   }
   vec3 specular = texture(material.specular, TexCoords).rgb * spec * light.specular;
   
   float distance    = length(light.position - FragPos);
   float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
   ambient  *= attenuation; 
   diffuse  *= attenuation;
   specular *= attenuation;
   
   vec3 result = (ambient + diffuse + specular) ;
   return result;
}

vec3 CalcSpotLight(SpotLight light,vec3 normal,vec3 fragPos, vec3 viewDir){
   //聚光
   vec3 lightDir = normalize(light.position - FragPos);
   
   //环境光
   vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;
   
   //漫反射
   float diff = max(dot(normal, lightDir),0.0f);
   vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;
   
   //镜面反射
   float spec;
   if(blinn){
      vec3 halfDir = normalize(lightDir + viewDir);  
      spec = pow(max(dot(viewDir, halfDir), 0.0), material.shininess);
   }else{
      vec3 reflectDir = reflect(-lightDir, normal);
      spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
   }
   vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;  

   //spotlight (soft edges)
   float theta = dot(lightDir, normalize(-light.direction)); 
   float epsilon = (light.cutOff - light.outerCutOff);
   float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
   ambient *= intensity;
   diffuse  *= intensity;
   specular *= intensity;
   
   // attenuation
   float distance    = length(light.position - FragPos);
   float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
   ambient  *= attenuation; 
   diffuse   *= attenuation;
   specular *= attenuation;  
   vec3 result = ambient + diffuse + specular;
   return result;
   
}

