#include "renders/meshRender.h"

CMeshRender::CMeshRender()
{
    // 默认构造
}

CMeshRender::CMeshRender(const CPipeline &_pipeline)
{
    pipeline = _pipeline;
}

void CMeshRender::setBufferData(const CMesh &_mesh)
{
    // 完成两个缓冲区对象的创建加赋值
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

void CMeshRender::bindAttributeData()
{
    VAO = pipeline.setVAO(VBO, EBO);
}

void CMeshRender::bindUniform_mvp(const glm::mat4 &model, const glm::mat4 &view, const glm::mat4 &projection)
{
    pipeline.setMat4("uModelViewMatrix", view * model); // 传入mv一起作用的矩阵
    pipeline.setMat4("uProjectionMatrix", projection);
    pipeline.setMat4("uModelMatrix", model); // 传入m矩阵

}
void CMeshRender::bindUniform_modelToWorld_normal(const glm::mat3 &modeltoworld_normal)
{
    pipeline.setMat3("uModelToWorldNormalMatrix", modeltoworld_normal);//传入法线矩阵

}

void CMeshRender::bindUniform_camera(const CCamera &camera)
{
    pipeline.setVec3("uCameraPos", camera.position);
}

void CMeshRender::bindUniform_light(const vector<CLight> &lights)
{
    pipeline.setVec3("uLightPos", lights[0].position);
    pipeline.setFloat("uLightIntensity", lights[0].intensity);
}

void CMeshRender::bindUnifrom_lightPos(const glm::vec3 &_position)
{
    pipeline.setVec3("uLightPos", _position);
}

void CMeshRender::bindUniform_material(const CMaterial &_material)
{
   pipeline.setVec3("ukd", _material.kd);
   pipeline.setVec3("uks", _material.ks);
}

void CMeshRender::bindTexture(const CMaterial &_material)
{
    for (int i=0; i<_material.textures.size(); i++){
        pipeline.setTexture(_material.textures[i].type, _material.textures[i].id);
    }
}

void CMeshRender::bindTextureSample(const int &_t)
{  
    pipeline.setInt("uTextureSample", _t);
}

void CMeshRender::use()
{
    glUseProgram(pipeline.id);
}

void CMeshRender::draw()
{  
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices_size), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glActiveTexture(GL_TEXTURE0);
}

void CMeshRender::freeData()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}
