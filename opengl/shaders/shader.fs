#version 330 core
out vec4 FragColor;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform vec3 objectColor;
uniform vec3 lightColor;

uniform vec3 lightPos;//灯光位置
uniform vec3 viewPos;//摄像机位置
in vec3 FragPos;
in vec3 Normal;
void main()
{
   //FragColor = texture(texture1, TexCoord);

   //FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);
   //FragColor = lightColor * objectColor;

   //环境光
   float ambientStrength = 0.1;
   vec3 ambient = ambientStrength * lightColor;
    
    //漫反射
   vec3 norm = normalize(Normal);
   vec3 lightDir = normalize(lightPos - FragPos);
   float diff = max(dot(norm, lightDir), 0.0);
   vec3 diffuse = diff * lightColor;

   //镜面光照
   float specularStrength = 0.5;
   vec3 viewDir = normalize(viewPos - FragPos);
   vec3 reflectDir = reflect(-lightDir, norm);
   float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
   vec3 specular = specularStrength * spec * lightColor;

   vec3 result = (ambient + diffuse + specular) * objectColor;
   FragColor = vec4(result, 1.0);

}