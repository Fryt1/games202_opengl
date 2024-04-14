#include "lightRender.h"

CLightRender::CLightRender()
{
    // 默认构造
}

CLightRender::CLightRender(const CPipeline &_pipeline)
{
    pipeline = _pipeline;
}

void CLightRender::setBufferData(const CMesh &_mesh)
{
    unsigned int _VBO, _EBO;
    glGenBuffers(1, &_VBO);
    glGenBuffers(1, &_EBO);

    glBindBuffer(GL_ARRAY_BUFFER, _VBO); 
    glBufferData(GL_ARRAY_BUFFER, _mesh.vertices.size() * sizeof(SVertex), &_mesh.vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, _mesh.indices.size() * sizeof(unsigned int), &_mesh.indices[0], GL_STATIC_DRAW);

    VBO = _VBO;
    EBO = _EBO;
    indices_size = _mesh.indices.size();

}

void CLightRender::bindAttributeData()
{
    unsigned int _VAO;
    glGenVertexArrays(1, &_VAO);

    VAO = _VAO;

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    string name = "aVertexPosition";
    glEnableVertexAttribArray(glGetAttribLocation(pipeline.id, name.c_str()));
    glVertexAttribPointer(glGetAttribLocation(pipeline.id, name.c_str()), 3, GL_FLOAT, GL_FALSE, sizeof(SVertex), (void*)0); 
    // 最后把VAO的状态区域设置空
    glBindVertexArray(0);

}

void CLightRender::bindUniform_mvp(const glm::mat4 &model, const glm::mat4 &view, const glm::mat4 &projection)
{
    pipeline.setMat4("uModelViewMatrix", view * model); // 传入mv一起作用的矩阵
    pipeline.setMat4("uProjectionMatrix", projection);
}

void CLightRender::bindUniform_light(const CLight &light)
{
    pipeline.setVec3("uLightColor", light.color);
}

void CLightRender::use()
{
    glUseProgram(pipeline.id);
}

void CLightRender::draw()
{
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices_size), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glActiveTexture(GL_TEXTURE0);
}

void CLightRender::freeData()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}
