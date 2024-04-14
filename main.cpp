#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "stb/stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>
#include <winsmcrd.h>

#include "renders/sceneRender.h"
#include "gl/pipeline.h"

using namespace std;

//extern "C" __declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;

// settings
const unsigned int SCR_WIDTH = 1000;
const unsigned int SCR_HEIGHT = 750;

float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float xoffset = 0;
float yoffset = 0;

void processInput(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
// void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);

int isInform = 0; // 是否需要输出当前状态信息
int isCameraRotate = 0; // 当前摄像机是否需要旋转

Camera_Movement movementNow = STILLNESS;

// 简化main函数过程
CPipeline getLightPipeline(string lightVShader_path, string lightFShader_path);
CPipeline getPhongPipeline(string phongVShader_path, string phongFShader_path);

int main()
{   
    // 必要初始化：glfw glad 以及完成窗口创建以及设置
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "MyWork0", NULL, NULL);

    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // 完成窗口交互函数的绑定 
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    // glfwSetCursorPosCallback(window, mouse_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // stbi_set_flip_vertically_on_load(true);

    // 主程序流程

    string lightVShader_path = "src/shaders/lightshaders/vertex.glsl";
    string lightFShader_path = "src/shaders/lightshaders/fragment.glsl";

    string phongVShader_path = "src/shaders/phongshaders/vertex.glsl";
    string phongFShader_path = "src/shaders/phongshaders/fragment.glsl";

    string model_path = "assets/mary/Marry.obj";

    // 创建pipelien
    // 光源pipe 需要先创建光源的两个shader
    // 目前名称数组并没有真正的体现用途 后面要尝试通过名称数组绑定各种unifrom的话 需要完成类型设置
    // 感觉要从vector换成mat 这只是想法 实现起来还需要具体设计 变量名称规则需要统一
    
    // light pipe
    CPipeline lightPipe = getLightPipeline(lightVShader_path, lightFShader_path);
    // Phong pipe
    CPipeline phongPipe = getPhongPipeline(phongVShader_path, phongFShader_path);

    CSceneRender scene;
    scene.addModel(model_path);

    scene.addLight(250, glm::vec3(50.f,75.f,50.f), glm::vec3(1.f, 1.f, 1.f));

    scene.addCamera(glm::vec3(-20.f, 150.f, 250.f), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));

    // 调整一下相机的位置
    scene.cameras[0].translateToTarget(80);
    scene.cameras[0].translatetToUp(70);

    unsigned int lightPipe_id = scene.addPipeline(lightPipe);
    unsigned int phongPipe_id = scene.addPipeline(phongPipe);

    unsigned int n = 0;
    
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    while (!glfwWindowShouldClose(window))
    {
        
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // 设置一下场景内全局MVP
        // 让光源动起来
        float camX = sin(glfwGetTime()* 0.6 * 2) * 100;
        float camY = cos(glfwGetTime()* 0.4 * 2) * 150;
        float camZ = cos(glfwGetTime()* 0.2 * 2) * 100;
        scene.setModelMatrixLight(scene.getModelMatrix(glm::vec3(camX, camY, camZ), (float)glfwGetTime(), glm::vec3(0,1,0), glm::vec3(1,1,1)));
        scene.setModelMatrixModel(scene.getModelMatrix(glm::vec3(0, 0, 0), 0, glm::vec3(0,0,1), glm::vec3(52,52,52)));
        scene.setViewMatrix(scene.getViewMatrix(0));
        scene.setProjectionMatrix(scene.getProjectionMatrix(75, SCR_WIDTH/SCR_HEIGHT, 0.1, 1000));

        // 更新光源位置
        scene.lights[0].setPosition(glm::vec3(camX, camY, camZ));

        // 更新摄像机位置 这边暂时只考虑单摄像机处理情况
        if (movementNow != STILLNESS){
            scene.cameras[0].processKeyboard(movementNow, deltaTime);
            movementNow = STILLNESS; // 按一次只运动一次
        }
        

        if (isInform){
            // 输出当前摄像机的属性 用于后续设置更好的初始化摄像机坐标
            cout << "相机属性：" << endl;
            scene.cameras[0].log();
            isInform = 0;
        }

        // 鼠标捕捉逻辑出现错误
        // if (isCameraRotate){
        //     // 旋转摄像机 恢复状态为0
        //     if (xoffset != 0){
        //         scene.cameras[0].updateByYaw(xoffset);
        //     }
        //     if (yoffset != 0){
        //         scene.cameras[0].updateByPitch(yoffset);
        //     }
        //     isCameraRotate = 0;
        // }

        if (isCameraRotate == 1){
            scene.cameras[0].updateByPitch();
            isCameraRotate = 0;
        }
        if (isCameraRotate == 2){
            scene.cameras[0].updateByPitchNeg();
            isCameraRotate = 0;
        }
        if (isCameraRotate == 3){
            scene.cameras[0].updateByYaw();
            isCameraRotate = 0;
        }
        if (isCameraRotate == 4){
            scene.cameras[0].updateByYawNeg();
            isCameraRotate = 0;
        }

        // 绘制
        scene.draw(lightPipe_id, phongPipe_id);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        movementNow = FORWARD;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        movementNow = BACKWARD;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        movementNow = LEFT;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        movementNow = RIGHT;
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        isInform = 1;
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)  
        isCameraRotate = 1;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)  
        isCameraRotate = 2;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)  
        isCameraRotate = 4;
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)  
        isCameraRotate = 3;

}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    isCameraRotate = 1;

    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    xoffset = xpos - lastX;
    yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

}

CPipeline getLightPipeline(string lightVShader_path, string lightFShader_path)
{
    CShader lightVShader(lightVShader_path, 0);
    vector<string> light_attriNamesV = {
        "aVertexPosition"
    };
    vector<string> light_unifromNamesV = {
        "uModelViewMatrix",
        "uProjectionMatrix"
    };
    lightVShader.setAttributeName(light_attriNamesV);
    lightVShader.setUniformName(light_unifromNamesV);

    CShader lightFShader(lightFShader_path, 1);
    vector<string> light_uniformNamesF = {
        "uLigIntensity",
        "uLightColor"
    };
    lightFShader.setUniformName(light_uniformNamesF);

    // link program
    CPipeline lightPipe(lightVShader, lightFShader);
    return lightPipe;
}

CPipeline getPhongPipeline(string phongVShader_path, string phongFShader_path)
{
    // Phong pipe
    CShader phongVShader(phongVShader_path, 0);
    vector<string> phong_attriNamesV = {
        "aVertexPosition",
        "aNormalPosition",
        "aTextureCoord"
    };
    vector<string> phong_uniformNamesV = {
        "uModelViewMatrix",
        "uProjectionMatrix"
    };
    phongVShader.setAttributeName(phong_attriNamesV);
    phongVShader.setUniformName(phong_uniformNamesV);

    CShader phongFShader(phongFShader_path, 1);
    vector<string> phong_unifromNamesF = {
        "uKd",
        "uKs",
        "uLightPos",
        "uCameraPos",
        "uLightIntensity",
        "uTextureSample"
    };
    vector<string> phong_textureNamesF = {
        "uSampler"
    };
    phongFShader.setUniformName(phong_unifromNamesF);
    phongFShader.setTextureName(phong_textureNamesF);

    // link program
    CPipeline phongPipe(phongVShader, phongFShader);
    return phongPipe;
}