#include "material.h"


CMaterial::CMaterial()
{
    type = PHONG;
    ka = glm::vec3();
    kd = glm::vec3();
    ks = glm::vec3();
}

CMaterial::CMaterial(const Type &_type)
{
    type = _type;
    ka = glm::vec3();
    kd = glm::vec3();
    ks = glm::vec3();
}

CMaterial::CMaterial(const Type &_type, const glm::vec3 &_ka, const glm::vec3 &_kd, const glm::vec3 &_ks)
{
    type = _type;
    ka = _ka;
    kd = _kd;
    ks = _ks;
}

void CMaterial::addTexture(const Texture &_texture)
{
    textures.push_back(_texture);
}

void CMaterial::setTextures(const vector<Texture> &_textures)
{
    textures = _textures;
}

Type CMaterial::getType()
{
    return type;
}

bool CMaterial::getPhongPara(glm::vec3 &_ka, glm::vec3 &_kd, glm::vec3 &_ks)
{
    if (ka == glm::vec3() && kd == glm::vec3() && ks == glm::vec3()){
        return false; // 没有给三个参数赋值过，全部为0，返回这个信息
    }
    _ka = ka;
    _kd = kd;
    _ks = ks;
    return true;
}

int CMaterial::getTexturesSize()
{
    return textures.size();
}

Texture CMaterial::getTexture(const int &id)
{
    return textures[id];
}



bool CMaterial::compare(CMaterial &_material)
{
    bool _return = true;
    glm::vec3 _ka, _kd, _ks;
    _material.getPhongPara(_ka,_kd,_ks);
    if(ka != _ka || kd != _kd || ks != _ks){ // 比较三个参数
        _return = false;
    }

    if (textures.size() != _material.getTexturesSize()){ // 比较材质里存储的纹理图片数量
        _return = false;
    }

    for (int i=0; i<_material.getTexturesSize(); i++){
        if (textures[i].id != _material.getTexture(i).id){ // 比较每一张是否一样
            _return = false;
        }
    }

    return _return;

}

