#ifndef MYWORK_LIGHTRENDER_H
#define MYWORK_LIGHTRENDER_H

#include "mesh.h"
#include "material.h"
#include "pipeline.h"
#include "light.h"

class CLightRender
{
    public:
    // 三个上下文里对象的id
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;

    unsigned int indices_size;

    // CMesh mesh;
    // CMaterial material; // 不想重复储存内容 浪费空间
    CPipeline pipeline;

    CLightRender();
    CLightRender(const CPipeline &_pipeline);

    void setBufferData(const CMesh &_mesh); // 数据加载到缓冲区 返回上下文缓冲区的id

    void bindAttributeData(); // 绑定顶点数据 与meshRender不一样 不调用管线类的接口 自己设置VAO

    void bindUniform_mvp(const glm::mat4 &model, const glm::mat4 &view, const glm::mat4 &projection); // 绑定uniform mvp矩阵

    void bindUniform_light(const CLight &light); // 绑定光源类相关属性 只需要绑定着色器里的颜色变量

    void use();

    void draw(); // 渲染启动接口

    void freeData(); // 释放资源

};


#endif