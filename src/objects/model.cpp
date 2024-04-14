#include "model.h"

#include <iostream>
#include <glad/glad.h>
#include "stb/stb_image.h"

using namespace std;

unsigned int TextureFromFile(const char *path, const string &directory);

CModel::CModel(const string &path)
{
    loadModel(path);
    isTexture = true;
}

int CModel::getMeshCount()
{
    return meshes.size();
}

int CModel::getMaterialCount()
{
    return materials.size();
}

bool CModel::loadModel(const string &path)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
    // check for errors
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
    {
        cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
        return false;
    }
    // retrieve the directory path of the filepath
    directory = path.substr(0, path.find_last_of('/'));
    
    // process ASSIMP's root node recursively
    processNode(scene->mRootNode, scene);

    return true;
}

void CModel::processNode(aiNode *_node, const aiScene *_scene)
{
    // cout << "预计可能添加" << _node->mNumMeshes << "个mesh" << endl;
    for(unsigned int i = 0; i < _node->mNumMeshes; i++)
    {
        // cout << "添加一个mesh" << endl;
        aiMesh* mesh = _scene->mMeshes[_node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, _scene));
        // processMaterial(mesh, _scene); 这一步优化到在处理Mesh过程里加入
    }

    for(unsigned int i = 0; i < _node->mNumChildren; i++)
    {
        processNode(_node->mChildren[i], _scene);
    }
}

CMesh CModel::processMesh(aiMesh *_mesh, const aiScene *_scene)
{
    vector<SVertex> vertices;
    vector<unsigned int> indices;
    for(unsigned int i = 0; i < _mesh->mNumVertices; i++){
        // 遍历mesh底下的所有顶点数据
        SVertex vertex;
        glm::vec3 vector;
        // 位置
        vector.x = _mesh->mVertices[i].x;
        vector.y = _mesh->mVertices[i].y;
        vector.z = _mesh->mVertices[i].z;
        vertex.Position = vector;
        // 法向量
        if (_mesh->HasNormals()){
            vector.x = _mesh->mNormals[i].x;
            vector.y = _mesh->mNormals[i].y;
            vector.z = _mesh->mNormals[i].z;
            vertex.Normal = vector;
        } else {
            vertex.Normal = glm::vec3();
        }
        // 纹理坐标
        // mesh里应该是可以保存多张纹理坐标的 但是目前 我们只考虑存在一个 将这一个纹理坐标作用到多张纹理上
        // 如果后续有需求，再回来修改便是
        if (_mesh->mTextureCoords[0]){
            glm::vec2 vec;
            vec.x = _mesh->mTextureCoords[0][i].x; 
            vec.y = _mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec;
        } else {
            vertex.TexCoords = glm::vec2();
        }

        // cout << "vertex_postion:" << vertex.Position.x << " " << vertex.Position.y << " " << vertex.Position.z  << endl;
        // cout << "vertex_normal:" << vertex.Normal.x << " " << vertex.Normal.y << " " << vertex.Normal.z  << endl;
        // cout << "vertex_texcoord:" << vertex.TexCoords.x << " " << vertex.TexCoords.y << " " <<  endl;

        vertices.push_back(vertex);

    }

    // 处理索引
    for (unsigned int i=0; i<_mesh->mNumFaces; i++){
        aiFace face = _mesh->mFaces[i];
        for(unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);   
    }

    // cout << "indiecs_size:" << indices.size() << endl;

    // 找到该Mesh对应的材质序号ID
    unsigned int id = processMaterial(_mesh, _scene);

    return CMesh(vertices, indices, id);
}

unsigned int CModel::processMaterial(aiMesh *_mesh, const aiScene *_scene)
{
    // CMaterial material(PHONG); // 默认读取的模型材质 全部按照PHONG来处理，后面有需求 在增加个type参数
    aiMaterial *aiMaterial = _scene->mMaterials[_mesh->mMaterialIndex];
    vector<Texture> textures;

    // 获取三个参数
    glm::vec3 _ka, _kd, _ks;
    glm::vec3 color;
    aiMaterial->Get(AI_MATKEY_COLOR_AMBIENT, color);
    _ka = glm::vec3(color.r, color.g, color.b);
    aiMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, color);
    _kd = glm::vec3(color.r, color.g, color.b);
    aiMaterial->Get(AI_MATKEY_COLOR_SPECULAR, color);
    _ks = glm::vec3(color.r, color.g, color.b);

    CMaterial tmaterial(PHONG, _ka, _kd, _ks); // 默认读取的模型材质 全部按照PHONG来处理，后面有需求 在增加个type参数

    // 读取纹理贴图部分 目前只考虑读取漫反射贴图
    // 漫反射部分
    vector<Texture> diffuseMaps = loadMaterialTextures(aiMaterial, aiTextureType_DIFFUSE, "texture_diffuse");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

    tmaterial.setTextures(textures);

    unsigned int tid = 0;

    // 加入到材质数组之前，需要判断是否重复添加材质
    if (materials.size() == 0){
        materials.push_back(tmaterial);
        tid = 0;
    } else {
        for (int i=0; i<materials.size(); i++){
            if (!tmaterial.compare(materials[i])){
                // 当前材质与model内存储材质类别不一样
                materials.push_back(tmaterial);
                tid = materials.size() - 1;
            } else {
                // 当前材质在model内已经存在，不用继续判断
                tid = i;
                break;
            }
        }
    }

    return tid;
}

vector<Texture> CModel::loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName)
{
    // 将aasimp--mesh里的材质部分 关于贴图的路径部分全部转为纹理结构体返回
    vector<Texture> textures;
    for(unsigned int i = 0; i < mat->GetTextureCount(type); i++){
        aiString str;
        mat->GetTexture(type, i, &str);
        bool skip = false;
        for(unsigned int j = 0; j < textures_loaded.size(); j++){
            if(std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0){
                textures.push_back(textures_loaded[j]);
                skip = true;
                break;
            }
        }
        if (!skip){
            Texture texture;
            texture.id = TextureFromFile(str.C_Str(), this->directory);
            texture.type = typeName;
            texture.path = str.C_Str();
            textures.push_back(texture);
            textures_loaded.push_back(texture);
        }
    }
    return textures;
}

unsigned int TextureFromFile(const char *path, const string &directory)
{
    // 读取图片 根据assimp拿到的纹理路径
    string filename = string(path);
    filename = directory + '/' + filename;

    unsigned int textureID;
    glGenTextures(1, &textureID); // 创建上下文对象 拿到ID

    int width, height, nrComponents;
    unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0); // 加载图片库
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        // 将对象绑定到指定状态区域 调用状态修改接口 给对象状态赋值 完成创建的对象赋值操作 
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D); // 生成mipmap 处理纹理映射问题

        // 一些纹理状态参数，st方向拉伸以及过小纹理和过大纹理的处理方法 这边是常用默认的配置
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data); // 释放图片资源 因为已经绑定到了上下文里的纹理对象
    }
    else
    {
        std::cout << "The texture was not imported correctly, the problem is in the loadMaterialTextures section" << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}