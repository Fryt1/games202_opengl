#ifndef MYWORK_CAMERA_H
#define MYWORK_CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    STILLNESS
};

class CCamera
{
    public:
    // 位置
    glm::vec3 position; 
    // 目标
    glm::vec3 target;
    // 向上方向
    glm::vec3 lookUp;

    // 自身坐标
    // -z 与摄像机实际方向相反
    glm::vec3 dir;
    glm::vec3 front; // 正Z方向 后面可以用于代替target用
    // x
    glm::vec3 right;
    // y
    glm::vec3 up;

    // 用于摄像机移动需要的属性
    // 欧拉角 在我的架构里用不到
    float yaw; 
    float pitch;
    // 移动参数
    float moveSpeed;
    float mouseSensitivity;

    CCamera(); // 默认构造
    CCamera(const glm::vec3 &_position, const glm::vec3 &_target, const glm::vec3 &_lookup); // 构造

    void getViewMatrix(glm::mat4 &view); // 得到view矩阵

    void translateToTarget(float dis); // 朝目标方向移动

    void translatetToUp(float dis); // 朝上方向移动

    void processKeyboard(Camera_Movement movement, float deltaTime); // 按键触发接口

    void log(); // 输出信息

    void updateByPitch(float _pitch); // 摄像机旋转
    void updateByPitch();
    void updateByPitchNeg();

    void updateByYaw(float _yaw); 
    void updateByYaw();
    void updateByYawNeg();

};

#endif