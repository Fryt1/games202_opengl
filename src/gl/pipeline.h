#ifndef MYWORK_PIPELINE_H
#define MYWORK_PIPELINE_H

#include "gl/shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include "objects/mesh.h"

class CPipeline
{
    public:
    // 顶点着色器
    CShader vertexShader;
    // 片段着色器
    CShader fragmentShader;
    // 用于绑定纹理贴图用的序号
    unsigned int texture_id;
    // 自身在上下文的id
    unsigned int id;

    CPipeline();

    CPipeline(const CShader &_vertexShader, const CShader &_fragmentShader); // 传入两个着色器类 完成初始化 创建program

    bool judgeNameIn(const string &_name , const string &_type); // 判断这个赋值的变量是否在着色器里存在 目前这个函数 确实没有作用

    // unifrom数据绑定接口
    void setInt(const std::string &name, int value);
    void setFloat(const std::string &name, float value);
    void setVec2(const std::string &name, const glm::vec2 &value);
    void setVec2(const std::string &name, float x, float y);
    void setVec3(const std::string &name, const glm::vec3 &value);
    void setVec3(const std::string &name, float x, float y, float z);
    void setVec4(const std::string &name, const glm::vec4 &value);
    void setVec4(const std::string &name, float x, float y, float z, float w);
    void setMat3(const std::string &name, const glm::mat3 &mat);
    void setMat4(const std::string &name, const glm::mat4 &mat);

    unsigned int setVAO(const unsigned int &VBO, const unsigned int &EBO); // attribute数据绑定接口

    void setTexture(const string &_type, unsigned int id); // 纹理对象数据绑定接口 前面参数类型 后面参数对象上下文id

};

#endif