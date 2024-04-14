#ifndef MYWORK_SCENERENDER_H
#define MYWORK_SCENERENDER_H

#include "model.h"
#include "light.h"
#include "pipeline.h"
#include "camera.h"
#include "meshRender.h"
#include "lightRender.h"

class CSceneRender
{
    public:
    // 一套场景内需要用到的所有东西都存进来
    vector<CModel> models;
    vector<CLight> lights;
    vector<CCamera> cameras;
    vector<CPipeline> pipelines; 

    // MVP矩阵储存
    glm::mat4 modelMatrix_light;
    glm::mat4 modelMatrix_model;
    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;

    CSceneRender(); // 默认初始 什么都不干

    void addModel(const string &path); // 增加一个model类进入数组 选择在里面直接创造后保存 在外面创造会增加临时空间
    void addLight(float _intensity, glm::vec3 _position, glm::vec3 _color); // 增加一个光源到数组里 需要参数就是创建对应类的构造函数参数
    void addCamera(const glm::vec3 &_position, const glm::vec3 &_target, const glm::vec3 &_lookup); // 摄像机
    unsigned int addPipeline(const CPipeline &_pipelien); // 直接赋值传入一个管线 管线不像场景 存在通用的可能性


    // 辅助函数
    glm::mat4 getModelMatrix(const glm::vec3 &_translate, float _r, const glm::vec3 &_rotate, const glm::vec3 &_scale); // 得到m变换矩阵
    glm::mat4 getViewMatrix(unsigned int camera_id); // 得到v变换矩阵
    glm::mat4 getProjectionMatrix(float _r, float _ratio, float _near, float _far); // 得到p变换矩阵
    void setModelMatrixLight(const glm::mat4 &_m);
    void setModelMatrixModel(const glm::mat4 &_m);
    void setViewMatrix(const glm::mat4 &_m);
    void setProjectionMatrix(const glm::mat4 &_m);


    // 渲染启动接口
    void draw(unsigned int light_pipe_id, unsigned int Phong_pipe_id); 

    private:

    void modelRendering(unsigned int model_id, unsigned int pipeline_id); // 渲染一个model 直接指定数组序号就行
    void lightRendering(unsigned int light_id, unsigned int pipeline_id); // 同理 渲染一个光源

};

#endif