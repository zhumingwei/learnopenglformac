#version 330 core
out vec4 FragColor;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;

uniform vec3 viewPos;//摄像机位置
in vec3 FragPos;
in vec3 Normal;
struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 

struct Light {
    vec3 position;//灯光位置

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
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

   //环境光
   vec3 ambient  = light.ambient * material.ambient;
    
    //漫反射
   vec3 norm = normalize(Normal);
   vec3 lightDir = normalize(light.position - FragPos);
   float diff = max(dot(norm, lightDir), 0.0);
   vec3 diffuse  = light.diffuse * (diff * material.diffuse);

   //镜面光照
   vec3 viewDir = normalize(viewPos - FragPos);
   vec3 reflectDir = reflect(-lightDir, norm);
   float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
   vec3 specular = material.specular * spec * light.specular;

   vec3 result = (ambient + diffuse + specular) ;
   FragColor = vec4(result, 1.0);

}