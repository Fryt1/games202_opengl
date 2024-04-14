#include "light.h"

CLight::CLight(float _intensity, glm::vec3 _position, glm::vec3 _color)
{
    // 先绑定强度和位置
    intensity = _intensity;
    position = _position;
    color = _color;
    // 完成mesh和material的默认创造
    mesh = CMesh();
    material = CMaterial(LIGHT);
}

float CLight::getIntensity()
{
    return intensity;
}

glm::vec3 CLight::getPosition()
{
    return position;
}

void CLight::setPosition(const glm::vec3 &_position)
{
    position = _position;
};
