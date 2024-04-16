#include "objects/mesh.h"
#include <vector>


CMesh::CMesh()
{
    genCube();
}


CMesh::CMesh(const vector<SVertex> &_vertices, const vector<unsigned int> &_indices,const unsigned int &ID)
{
    vertices = _vertices;
    indices = _indices;
    toMaterial_ID = ID;
}

int CMesh::getVertexCount()
{
    return vertices.size();
}

unsigned int CMesh::getMeshToMateria()
{
    return toMaterial_ID;
}

void CMesh::genCube()
{
    const vector<glm::vec3> positions = {
        glm::vec3(-1.0,-1.0,1),
        glm::vec3(1.0,-1.0,1.0),
        glm::vec3(1.0,1.0,1.0),
        glm::vec3(-1.0,1.0,1.0),

        glm::vec3(-1.0,-1.0,-1.0),
        glm::vec3(-1.0,1.0,-1.0),
        glm::vec3(1.0,1.0,-1.0),
        glm::vec3(1.0,-1.0,-1.0),

        glm::vec3(-1.0, 1.0, -1.0),
        glm::vec3(-1.0, 1.0, 1.0),
        glm::vec3(1.0, 1.0, 1.0),
        glm::vec3(1.0, 1.0, -1.0),

        glm::vec3(-1.0, -1.0, -1.0),
        glm::vec3(1.0, -1.0, -1.0),
        glm::vec3(1.0, -1.0, 1.0),
        glm::vec3(-1.0, -1.0, 1.0),

        glm::vec3(1.0, -1.0, -1.0),
        glm::vec3(1.0, 1.0, -1.0),
        glm::vec3(1.0, 1.0, 1.0),
        glm::vec3(1.0, -1.0, 1.0),

        glm::vec3(-1.0, -1.0, -1.0),
        glm::vec3(-1.0, -1.0, 1.0),
        glm::vec3(-1.0, 1.0, 1.0),
        glm::vec3(-1.0, 1.0, -1.0),
    };

    const vector<unsigned int> _indices = {
        0, 1, 2, 0, 2, 3, // front
        4, 5, 6, 4, 6, 7, // back
        8, 9, 10, 8, 10, 11, // top
        12, 13, 14, 12, 14, 15, // bottom
        16, 17, 18, 16, 18, 19, // right
        20, 21, 22, 20, 22, 23, // left
    };

    for (auto vec : positions){
        SVertex vertex;
        vertex.Position = vec;
        vertex.Normal = glm::vec3();
        vertex.TexCoords = glm::vec3();
        vertices.push_back(vertex);
    }

    indices = _indices;

    toMaterial_ID = 0;
}