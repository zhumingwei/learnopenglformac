#version 330 core
out vec4 FragColor;
in vec2 TexCoords;


uniform sampler2D texture2;

uniform vec3 viewPos;//摄像机位置
in vec3 FragPos;
in vec3 Normal;
struct Material {
   vec3 ambient;
   sampler2D diffuse;
   sampler2D specular;
   float shininess;
}; 

struct Light {
   vec3 position;//灯光位置
   //vec3 direction;//平行光使用
   vec3 ambient;
   vec3 diffuse;
   vec3 specular;

   //点光源衰减相关
   float constant;
   float linear;
   float quadratic;

   //聚光相关
   vec3  direction;
   float cutOff;
   float outerCutOff;
};
//材质属性
uniform Material material;
//光照属性
uniform Light light;
void main()
{
   //FragColor = texture(texture1, TexCoord);

   //FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);
   //FragColor = lightColor * objectColor;

   //平行光
   // //环境光
   // vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;//环境光材料的初始值可以用漫反射的值
   //  //漫反射
   // vec3 norm = normalize(Normal);
   // vec3 lightDir = normalize(-light.direction);
   // float diff = max(dot(norm, lightDir), 0.0);
   // vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;
   // //镜面光照
   // vec3 viewDir = normalize(viewPos - FragPos);
   // vec3 reflectDir = reflect(-lightDir, norm);
   // float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
   // vec3 specular = material.specular * spec * light.specular;

   // vec3 result = (ambient + diffuse + specular) ;
   // FragColor =  vec4(result, 1.0);




   //点光源
   // vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;//环境光材料的初始值可以用漫反射的值
   //  //漫反射
   // vec3 norm = normalize(Normal);
   // float diff = max(dot(norm, lightDir), 0.0);
   // vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;
   // //镜面光照
   // vec3 viewDir = normalize(viewPos - FragPos);
   // vec3 reflectDir = reflect(-lightDir, norm);
   // float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
   // vec3 specular = material.specular * spec * light.specular;
   
   // float distance    = length(light.position - FragPos);
   // float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
   // ambient  *= attenuation; 
   // diffuse  *= attenuation;
   // specular *= attenuation;
   
   // vec3 result = (ambient + diffuse + specular) ;
   // FragColor =  vec4(result, 1.0);

   //聚光
   vec3 lightDir = normalize(light.position - FragPos);
   
   //环境光
   vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;
   
   //漫反射
   vec3 norm = normalize(Normal);
   float diff = max(dot(norm, lightDir),0.0f);
   vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;
   
   //镜面反射
   vec3 viewDir = normalize(viewPos - FragPos);
   vec3 reflectDir = reflect(-lightDir, norm);  
   float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
   vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;  

   // spotlight (soft edges)
   float theta = dot(lightDir, normalize(-light.direction)); 
   float epsilon = (light.cutOff - light.outerCutOff);
   float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
   diffuse  *= intensity;
   specular *= intensity;
   
   // attenuation
   float distance    = length(light.position - FragPos);
   float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
   ambient  *= attenuation; 
   diffuse   *= attenuation;
   specular *= attenuation;  
   vec3 result = ambient + diffuse + specular;
   FragColor = vec4(result, 1.0);
   

   //聚光1

   // vec3 lightDir = normalize(light.position - FragPos);
    
   //  // check if lighting is inside the spotlight cone
   //  float theta = dot(lightDir, normalize(-light.direction)); 
    
   //  if(theta > light.cutOff) // remember that we're working with angles as cosines instead of degrees so a '>' is used.
   //  {    
   //      // ambient
   //      vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;
        
   //      // diffuse 
   //      vec3 norm = normalize(Normal);
   //      float diff = max(dot(norm, lightDir), 0.0);
   //      vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;  
        
   //      // specular
   //      vec3 viewDir = normalize(viewPos - FragPos);
   //      vec3 reflectDir = reflect(-lightDir, norm);  
   //      float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
   //      vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;  
        
   //      // attenuation
   //      float distance    = length(light.position - FragPos);
   //      float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    

   //      // ambient  *= attenuation; // remove attenuation from ambient, as otherwise at large distances the light would be darker inside than outside the spotlight due the ambient term in the else branche
   //      diffuse   *= attenuation;
   //      specular *= attenuation;   
            
   //      vec3 result = ambient + diffuse + specular;
   //      FragColor = vec4(result, 1.0);
   //  }
   //  else 
   //  {
   //      // else, use ambient light so scene isn't completely dark outside the spotlight.
   //      FragColor = vec4(light.ambient * texture(material.diffuse, TexCoords).rgb, 1.0);
   //  }
}