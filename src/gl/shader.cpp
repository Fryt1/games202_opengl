#include "gl/shader.h"
#include <fstream>
#include <sstream>

CShader::CShader()
{
    // 默认构造
}

CShader::CShader(const string &path, int _type)
{
    string code;
    ifstream shaderFile;
    shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try{
        shaderFile.open(path);
        std::stringstream shaderStream;
        shaderStream << shaderFile.rdbuf();
        shaderFile.close();
        code = shaderStream.str();
    } catch (ifstream::failure& e) {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
    }
    
    const char *shaderCode = code.c_str();
    unsigned int _id;
    if (_type == 0)
        _id = glCreateShader(GL_VERTEX_SHADER);
    else
        _id = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(_id, 1, &shaderCode, NULL);
    glCompileShader(_id);
    id = _id;

    // 打印编译错误
    int success;
    char infoLog[512];
    glGetShaderiv(_id, GL_COMPILE_STATUS, &success);
    if (!success){
        glGetShaderInfoLog(_id, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

}

void CShader::setAttributeName(const vector<string> &_attributeName)
{
    attributeName = _attributeName;
}

void CShader::setUniformName(const vector<string> &_unifromName)
{
    uniformName = _unifromName;
}

void CShader::setTextureName(const vector<string> &_textureName)
{
    textureName = _textureName;
}

unsigned int CShader::getId()
{
    return id;
}

vector<string> CShader::getAttributeName()
{
    return attributeName;
}

vector<string> CShader::getUniformName()
{
    return uniformName;
}

vector<string> CShader::getTextureName()
{
    return textureName;
}

bool CShader::judgeNameIn(const string &_name, const string &_type)
{
    bool _result = false;
    if (_type == "attribute"){
        for (string name : attributeName){
            if (name == _name){
                _result = true;
                break;
            }
        }
    } else if (_type == "uniform"){
        for (string name : uniformName){
            if (name == _name){
                _result = true;
                break;
            }
        }
    } else if (_type == "texture"){
        for (string name : textureName){
            if (name == _name){
                _result = true;
                break;
            }
        }
    }

    return _result;
}
