#ifndef MYWORK_LIGHT_H
#define MYWORK_LIGHT_H

#include "objects/mesh.h"
#include "material/material.h"
#include "gl/pipeline.h"

class CLight
{
    public:
    // 网格体积
    CMesh mesh;
    // 材质
    CMaterial material;
    // 强度
    float intensity;
    // 位置
    glm::vec3 position;
    // 颜色
    glm::vec3 color;

    CLight(float _intensity, glm::vec3 _position, glm::vec3 _color); // 构造函数 传入强度和位置

    void draw(const CPipeline &pipeline); // 渲染启动

    float getIntensity(); // 获得成员接口
    glm::vec3 getPosition();

    void setPosition(const glm::vec3 &_position); // 设置新位置


};



#endif