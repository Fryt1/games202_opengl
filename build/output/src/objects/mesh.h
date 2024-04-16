#ifndef MYWORK_MESH_H
#define MYWORK_MESH_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

using namespace std;

struct SVertex {
    // position
    glm::vec3 Position;
    // normal
    glm::vec3 Normal;
    // texCoords
    glm::vec2 TexCoords;
};


class CMesh
{
    public:
    // 顶点属性
    vector<SVertex> vertices;
    // 组成图元的序号
    vector<unsigned int> indices;
    // 对应物体内的哪种材质 该mesh
    unsigned int toMaterial_ID;

    CMesh(); // 需要一个默认构造，我想生成一个立方体

    CMesh(const vector<SVertex> &_vertices, const vector<unsigned int> &_indices,const unsigned int &ID); // 构造函数 传入三个成员变量赋值

    int getVertexCount(); // 得到顶点个数

    unsigned int getMeshToMateria(); // 得到对应的哪种材质序号

    private:
    void genCube(); // 生成一个默认cube 
    
};


#endif // MYWORK_MESH_H