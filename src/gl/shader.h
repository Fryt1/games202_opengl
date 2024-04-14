#ifndef MYWORK_SHADER_H
#define MYWORK_SHADER_H

#include <glad/glad.h>
#include <iostream>
#include <vector>
#include <string>

using namespace std;

class CShader
{
    public:
    // 上下文里的id
    unsigned int id;
    // 顶点属性名称数组
    vector<string> attributeName;
    // unifrom属性名称数组
    vector<string> uniformName;
    // 纹理属性名称数组
    vector<string> textureName;

    CShader(); // 根据GLSL文件 创建着色器 设置一个默认构造函数
    CShader(const string &path, int _type); 

    // 设置数组
    void setAttributeName(const vector<string> &_attributeName);
    void setUniformName(const vector<string> &_unifromName);
    void setTextureName(const vector<string> &_textureName);

    // 得到变量
    unsigned int getId();
    vector<string> getAttributeName();
    vector<string> getUniformName();
    vector<string> getTextureName();

    // 判断是否存在变量 这个暂时用不到 
    bool judgeNameIn(const string &_name, const string &_type);

};

#endif