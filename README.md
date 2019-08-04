# learnopenglformac
##代码记得修改tasks.json和launch.json路径配置
1，源码安装glfw （动态库 cmake -D BUILD_SHARED_LIBS=ON ..）
2，安装glad
3，https://github.com/nothings/stb/blob/master/stb_image.h 图片库安装
4, 安装glm https://glm.g-truc.net/0.9.8/index.html 


```
note:
    使用glm生成变换4x4矩阵，传入shader.vs,然后gl_Position = transform * vec(aPos, 1.0);

```