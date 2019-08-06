#include <iostream>
#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include "shader_s.h"
#include <unistd.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <map>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
int genTexture(const char *spath);
void mouse_callback(GLFWwindow *window,double xpos,double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void justDraw(Shader* ourShader,Shader* lampShader,glm::vec3* pointLightPositions,glm::vec3* cubePositions,const unsigned int VAO,const unsigned int lightVAO,const float scale = 1.0f);
// settings
const unsigned int SCR_WIDTH = 1000;
const unsigned int SCR_HEIGHT = 800;

Camera camera(glm::vec3(0.0f,0.0f,5.0f));
float lastX = SCR_WIDTH/2.0f;
float lastY = SCR_HEIGHT/2.0f;
bool firstMouse = true;

//timing 
float deltaTime = 0.0f;
float lastFrame = 0.0f;



unsigned int texture1, texture2;
int main(){
    const int MAXPATH = 250;
    char buffer[MAXPATH];
    getcwd(buffer, MAXPATH);
    std::cout << "The current directory is:" << buffer << std::endl;

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
    #endif
    
    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    Shader ourShader("opengl/shaders/shader.vs", "opengl/shaders/shader.fs");
    Shader shader2("opengl/shaders/lamp.vs", "opengl/shaders/lamp.fs");

    glEnable(GL_DEPTH_TEST);
    // glEnable(GL_STENCIL_TEST);//模板缓冲
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


    float vertices[] = {
        -0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,    0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,     0.0f,  0.0f, -1.0f,    1.0f, 0.0f,
        0.5f, 0.5f, -0.5f,      0.0f,  0.0f, -1.0f,     1.0f, 1.0f,
        0.5f, 0.5f, -0.5f,      0.0f,  0.0f, -1.0f,     1.0f, 1.0f,
        -0.5f, 0.5f, -0.5f,     0.0f,  0.0f, -1.0f,     0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     0.0f, 0.0f,

        -0.5f, -0.5f, 0.5f,     0.0f,  0.0f,  1.0f,     0.0f, 0.0f,
        0.5f, -0.5f, 0.5f,      0.0f,  0.0f,  1.0f,     1.0f, 0.0f,
        0.5f, 0.5f, 0.5f,       0.0f,  0.0f,  1.0f,     1.0f, 1.0f,
        0.5f, 0.5f, 0.5f,       0.0f,  0.0f,  1.0f,     1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f,      0.0f,  0.0f,  1.0f,     0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f,     0.0f,  0.0f,  1.0f,     0.0f, 0.0f,

        -0.5f, 0.5f, 0.5f,      -1.0f,  0.0f,  0.0f,    1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f,     -1.0f,  0.0f,  0.0f,    1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f,     -1.0f,  0.0f,  0.0f,    0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f,      -1.0f,  0.0f,  0.0f,    1.0f, 0.0f,

        0.5f, 0.5f, 0.5f,       1.0f,  0.0f,  0.0f,     1.0f, 0.0f,
        0.5f, 0.5f, -0.5f,      1.0f,  0.0f,  0.0f,     1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,     1.0f,  0.0f,  0.0f,     0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,     1.0f,  0.0f,  0.0f,     0.0f, 1.0f,
        0.5f, -0.5f, 0.5f,      1.0f,  0.0f,  0.0f,     0.0f, 0.0f,
        0.5f, 0.5f, 0.5f,       1.0f,  0.0f,  0.0f,     1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,     0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,     0.0f, -1.0f,  0.0f,     1.0f, 1.0f,
        0.5f, -0.5f, 0.5f,      0.0f, -1.0f,  0.0f,     1.0f, 0.0f,
        0.5f, -0.5f, 0.5f,      0.0f, -1.0f,  0.0f,     1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f,     0.0f, -1.0f,  0.0f,     0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,     0.0f, 1.0f,

        -0.5f, 0.5f, -0.5f,     0.0f,  1.0f,  0.0f,     0.0f, 1.0f,
        0.5f, 0.5f, -0.5f,      0.0f,  1.0f,  0.0f,     1.0f, 1.0f,
        0.5f, 0.5f, 0.5f,       0.0f,  1.0f,  0.0f,     1.0f, 0.0f,
        0.5f, 0.5f, 0.5f,       0.0f,  1.0f,  0.0f,     1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f,      0.0f,  1.0f,  0.0f,     0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f,     0.0f,  1.0f,  0.0f,     0.0f, 1.0f
        };
    // positions of the point lights
    glm::vec3 pointLightPositions[] = {
        glm::vec3( 0.7f,  0.2f,  2.0f),
        glm::vec3( 2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f,  2.0f, -12.0f),
        glm::vec3( 0.0f,  0.0f, -3.0f)
    };

    unsigned int VBO,VAO;
    
    glGenVertexArrays(1,&VAO);
    glGenBuffers(1, &VBO);

    //绑定
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);

    //绑定position (0)
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,8 * sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);
    //绑定法向量
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,8 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    //绑定texture(2)
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    //解绑
    glBindBuffer(GL_ARRAY_BUFFER,0);
    //FIXME 这里不能解绑EBO，否则会导致无法显示
    glBindVertexArray(0);

    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
    //绑定position (0)
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,8 * sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);

    float transparentVertices[] = {
        // positions         // texture Coords (swapped y coordinates because texture is flipped upside down)
        0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
        0.0f, -0.5f,  0.0f,  0.0f,  1.0f,
        1.0f, -0.5f,  0.0f,  1.0f,  1.0f,

        0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
        1.0f, -0.5f,  0.0f,  1.0f,  1.0f,
        1.0f,  0.5f,  0.0f,  1.0f,  0.0f
    };

    // transparent VAO
    unsigned int transparentVAO, transparentVBO;
    glGenVertexArrays(1, &transparentVAO);
    glGenBuffers(1, &transparentVBO);
    glBindVertexArray(transparentVAO);
    glBindBuffer(GL_ARRAY_BUFFER, transparentVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(transparentVertices), transparentVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glBindVertexArray(0);
   

    // load and create a texture
    // -------------------------

    // load image, create texture and generate mipmaps
    texture1 = genTexture("opengl/textures/container2.png");
    texture2 = genTexture("opengl/textures/container2_specular.png");
    unsigned inttexture_kuang = genTexture("opengl/textures/blending_transparent_window.png");

    ourShader.use();
    //配置textureID 后面需要绑定根据这个数字， GL_TEXTUREX
    ourShader.setInt("material.diffuse",0);
    ourShader.setInt("material.specular",1);
    shader2.setInt("texture1",0);
    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f), 
        glm::vec3( 2.0f,  5.0f, -15.0f), 
        glm::vec3(-1.5f, -2.2f, -2.5f),  
        glm::vec3(-3.8f, -2.0f, -12.3f),  
        glm::vec3( 2.4f, -0.4f, -3.5f),  
        glm::vec3(-1.7f,  3.0f, -7.5f),  
        glm::vec3( 1.3f, -2.0f, -2.5f),  
        glm::vec3( 1.5f,  2.0f, -2.5f), 
        glm::vec3( 1.5f,  0.2f, -1.5f), 
        glm::vec3(-1.3f,  1.0f, -1.5f)  
    };

    // std::vector<glm::vec3> w123123;
    // w123123.push_back(glm::vec3(-1.5f, 0.0f, -0.48f));
    // w123123.insert(glm::vec3(-1.5f, 0.0f, -0.48f));
    // = {
    //     glm::vec3(-1.5f, 0.0f, -0.48f),
    //     glm::vec3( 1.5f, 0.0f, 0.51f),
    //     glm::vec3( 0.0f, 0.0f, 0.7f),
    //     glm::vec3(-0.3f, 0.0f, -2.3f),
    //     glm::vec3( 0.5f, 0.0f, -0.6f)
    // };
    glm::vec3 windows[] = {
        // glm::vec3( 3.0f,  4.0f, -5.5f), 
        glm::vec3(-1.5f, 0.0f, -0.48f),
        glm::vec3( 1.5f, 0.0f, 0.51f),
        glm::vec3( 0.0f, 0.0f, 0.7f),
        glm::vec3(-0.3f, 0.0f, -2.3f),
        glm::vec3( 0.5f, 0.0f, -0.6f)
    };
    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        
        justDraw(&ourShader,&shader2,pointLightPositions,cubePositions,VAO,lightVAO);
        
        shader2.use();
        glBindVertexArray(transparentVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, inttexture_kuang);
        // glBindTexture(GL_TEXTURE_2D, texture1);

        std::map<float, glm::vec3> sorted;

        for (unsigned int i = 0; i < 5; i++)
        {
            float distance = glm::length(camera.Position - windows[i]);
            sorted[distance] = windows[i];
        }

        glm::mat4 view = camera.GetViewMatrix();
       shader2.setMat4("view", view);

       glm::mat4 projection = glm::mat4(1.0f);
       projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
       // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
       shader2.setMat4("projection", projection);

        for (std::map<float, glm::vec3>::reverse_iterator it = sorted.rbegin(); it != sorted.rend(); ++it)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, it->second);
            shader2.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }
       // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
       // -------------------------------------------------------------------------------
       glfwSwapBuffers(window);
       glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
 }

 void justDraw(Shader* ourShader,Shader* lampShader,glm::vec3* pointLightPositions,glm::vec3* cubePositions,const unsigned int VAO,const unsigned int lightVAO,const float scale){
  //bind Texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

       //render container
       ourShader->use();
       ourShader->setVec3("viewPos", camera.Position);

        ourShader->setVec3("material.ambient",  1.0f, 0.5f, 0.31f);
        // ourShader->setVec3("material.diffuse",  1.0f, 0.5f, 0.31f);
        ourShader->setFloat("material.shininess", 32.0f);
        //平行光
        ourShader->setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
        ourShader->setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
        ourShader->setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
        ourShader->setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
        //点光
        // point light 1
        ourShader->setVec3("pointLights[0].position", pointLightPositions[0]);
        ourShader->setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
        ourShader->setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
        ourShader->setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
        ourShader->setFloat("pointLights[0].constant", 1.0f);
        ourShader->setFloat("pointLights[0].linear", 0.09);
        ourShader->setFloat("pointLights[0].quadratic", 0.032);
        // point light 2
        ourShader->setVec3("pointLights[1].position", pointLightPositions[1]);
        ourShader->setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
        ourShader->setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
        ourShader->setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
        ourShader->setFloat("pointLights[1].constant", 1.0f);
        ourShader->setFloat("pointLights[1].linear", 0.09);
        ourShader->setFloat("pointLights[1].quadratic", 0.032);
        // point light 3
        ourShader->setVec3("pointLights[2].position", pointLightPositions[2]);
        ourShader->setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
        ourShader->setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
        ourShader->setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
        ourShader->setFloat("pointLights[2].constant", 1.0f);
        ourShader->setFloat("pointLights[2].linear", 0.09);
        ourShader->setFloat("pointLights[2].quadratic", 0.032);
        // point light 4
        ourShader->setVec3("pointLights[3].position", pointLightPositions[3]);
        ourShader->setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
        ourShader->setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
        ourShader->setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
        ourShader->setFloat("pointLights[3].constant", 1.0f);
        ourShader->setFloat("pointLights[3].linear", 0.09);
        ourShader->setFloat("pointLights[3].quadratic", 0.032);

        //聚光
        ourShader->setVec3("spotLight.ambient",  0.1f, 0.1f, 0.1f);
        ourShader->setVec3("spotLight.diffuse",  0.8f, 0.8f, 0.8f); // 将光照调暗了一些以搭配场景
        ourShader->setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f); 
        ourShader->setVec3("spotLight.position",  camera.Position);
        ourShader->setVec3("spotLight.direction", camera.Front);
        ourShader->setFloat("spotLight.cutOff",   glm::cos(glm::radians(12.5f)));
        ourShader->setFloat("spotLight.outerCutOff", glm::cos(glm::radians(17.5f)));
        ourShader->setFloat("spotLight.constant", 1.0f);
        ourShader->setFloat("spotLight.linear", 0.09f);
        ourShader->setFloat("spotLight.quadratic", 0.032f);
        
       //create transformations
       
       glm::mat4 view = camera.GetViewMatrix();
       ourShader->setMat4("view", view);

       glm::mat4 projection = glm::mat4(1.0f);
       projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
       // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
       ourShader->setMat4("projection", projection);

       for (unsigned int i = 0; i < 10; i++)
       {
            glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
            model = glm::translate(model, cubePositions[i]);
            model = glm::rotate(model, (float)glfwGetTime()/2.0f, glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::scale(model, glm::vec3(scale));
            glBindVertexArray(VAO);
            ourShader->setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        //画灯泡
        // lampShader->use();
        // lampShader->setMat4("projection",projection);
        // lampShader->setMat4("view", view);
        // for (unsigned i = 0; i < 4; i++)
        // {
        //     glm::mat4 model = glm::mat4(1.0f);
        //     model = glm::translate(model, pointLightPositions[i]);
        //     model = glm::scale(model, glm::vec3(0.1f));
        //     lampShader->setMat4("model",model);
        //     glBindVertexArray(lightVAO);
        //     glDrawArrays(GL_TRIANGLES, 0, 36);
        // }
 }

 // process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

int genTexture(const char *path)
{
    unsigned int texture;
    glGenTextures(1, &texture);



    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);
    if (data)
    {

        GLenum format;
        if (nrChannels == 1)
            format = GL_RED;
        else if (nrChannels == 3)
            format = GL_RGB;
        else if (nrChannels == 4)
            format = GL_RGBA;
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        
        
        glGenerateMipmap(GL_TEXTURE_2D);
                    //环绕参数
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    return texture;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;
    camera.ProcessMouseMovement(xoffset, yoffset);
    
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}

