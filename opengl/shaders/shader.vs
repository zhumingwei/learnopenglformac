#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out VS_OUT {
    vec2 texCoords;
    out vec3 FragPos;
    out vec3 Normal;
} vs_out;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);    
    vs_out.texCoords = aTexCoord;
    vs_out.Normal = mat3(transpose(inverse(model))) * aNormal; //反矩阵运算，转置运算计算出法向量矩阵
    vs_out.FragPos = vec3(model * vec4(aPos, 1.0));
}