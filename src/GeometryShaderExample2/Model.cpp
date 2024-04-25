#include "Model.h"

#include <QOpenGLFunctions>
#include <QPixmap>

Model::Model(const QString &filePath)
{
    loadModel(filePath);
}

Model::~Model()
{
    qDeleteAll(m_meshes);
    qDeleteAll(m_Textures);
}

bool Model::loadModel(const QString &filePath)
{
    Assimp::Importer imp;
    const aiScene *scene = imp.ReadFile(filePath.toStdString(), aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals | aiProcess_CalcTangentSpace);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        qDebug() << QString("ERROR::ASSIMP::%1").arg(imp.GetErrorString());
        return false;
    }
    m_fileDir = QFileInfo(filePath).dir();
    processNode(scene->mRootNode, scene);
    return true;
}

void Model::draw(QOpenGLShaderProgram &shader)
{
    for (int i = 0; i < m_meshes.size(); ++i) {
        auto *meshObj = m_meshes[i];
        int diffuseNr = 1;
        int specularNr = 1;
        int normalsNr = 1;
        int j = 0;
        for (; j < meshObj->textureInfos.size(); ++j) {
            QString num;
            if (meshObj->textureInfos[j].type == DIFFUSE_TEX_NAME)
                num = QString::number(diffuseNr++);
            else if (meshObj->textureInfos[j].type == SPECULAR_TEX_NAME)
                num = QString::number(specularNr++);
            else if (meshObj->textureInfos[j].type == NORMALS_TEX_NAME)
                num = QString::number(normalsNr++);
            m_Textures[meshObj->textureInfos[j].id]->bind(j);
            shader.setUniformValue((meshObj->textureInfos[j].type + num).toStdString().c_str(), j);
        }
        // 如果没有贴图 就用默认的颜色.
        if (diffuseNr == 1 && specularNr == 1) {
            shader.setUniformValue("uDiffuseColor", meshObj->diffuseColor);
            shader.setUniformValue("uSpecularColor", QVector4D(0.5, 0.5, 0.5, 0.5));
            shader.setUniformValue("uTextureMapping", 1);
        } else {
            shader.setUniformValue("uTextureMapping", 0);
        }
        shader.setUniformValue("uNormalMapping", normalsNr - 1);

        QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
        meshObj->vao.bind();
        f->glDrawElements(GL_TRIANGLES, meshObj->count, GL_UNSIGNED_INT, 0);
        meshObj->vao.release();
        for (auto it = meshObj->textureInfos.cbegin(); it != meshObj->textureInfos.cend(); ++it) {
            m_Textures[it->id]->release();
        }
    }
}

void Model::processNode(const aiNode *node, const aiScene *scene)
{
    // 处理节点所有的网格（如果有的话）.
    for(uint i = 0; i < node->mNumMeshes; ++i)
    {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        auto m = processMesh(mesh, scene);
        m_meshes.append(m);
    }
    // 接下来对它的子节点重复这一过程.
    uint j = 0;
    for(; j < node->mNumChildren; ++j)
    {
        processNode(node->mChildren[j], scene);
    }
}

ModelMesh* Model::processMesh(const aiMesh *mesh, const aiScene *scene)
{
    QVector<ModelVertex> vertices;
    QVector<uint> indices;
    QVector<TextureInfo> textureInfos;
    for(uint i = 0; i < mesh->mNumVertices; i++)
    {
        ModelVertex vertex;
        // 顶点
        vertex.Position.x = mesh->mVertices[i].x;
        vertex.Position.y = mesh->mVertices[i].y;
        vertex.Position.z = mesh->mVertices[i].z;
        // 法线
        vertex.Normal.x = mesh->mNormals[i].x;
        vertex.Normal.y = mesh->mNormals[i].y;
        vertex.Normal.z = mesh->mNormals[i].z;
        // 纹理坐标
        if(mesh->mTextureCoords[0]) // 网格是否有纹理坐标？
        {
            vertex.TexCoord.x = mesh->mTextureCoords[0][i].x;
            vertex.TexCoord.y = mesh->mTextureCoords[0][i].y;
        }
        if (mesh->mTangents) {
            // 切线
            vertex.Tangent.x = mesh->mTangents[i].x;
            vertex.Tangent.y = mesh->mTangents[i].y;
            vertex.Tangent.z = mesh->mTangents[i].z;
            // 副切线
            vertex.Bitangent.x = mesh->mBitangents[i].x;
            vertex.Bitangent.y = mesh->mBitangents[i].y;
            vertex.Bitangent.z = mesh->mBitangents[i].z;
        }
        vertices.append(vertex);
    }
    // 索引
    for(uint i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for(uint j = 0; j < face.mNumIndices; j++)
            indices.append(face.mIndices[j]);
    }
    // 材质
    aiColor4D diffuseColor;
    if(mesh->mMaterialIndex >= 0)
    {
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
        loadMaterialTextures(scene, material, aiTextureType_DIFFUSE, DIFFUSE_TEX_NAME, textureInfos);
        loadMaterialTextures(scene, material, aiTextureType_SPECULAR, SPECULAR_TEX_NAME, textureInfos);
        loadMaterialTextures(scene, material, aiTextureType_NORMALS, NORMALS_TEX_NAME, textureInfos);

        material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuseColor);
    }
    return createModelMesh(vertices, indices, textureInfos, diffuseColor);
}

void Model::loadMaterialTextures(const aiScene *scene, const aiMaterial *mat, aiTextureType type, const QString &typeName, QVector<TextureInfo> &textures)
{
    for (uint i = 0; i < mat->GetTextureCount(type); ++i) {
        aiString fileName;
        mat->GetTexture(type, i, &fileName);
        bool skip = false;
        QString qFileName(fileName.C_Str());
        for (uint j = 0; j < m_loaded.size(); ++j) {
            if (m_loaded.value(j).path == qFileName) {
                textures.append(m_loaded.value(j));
                skip = true;
                break;
            }
        }
        if (skip) continue;
        TextureInfo texture;

        const aiTexture *aiT = scene->GetEmbeddedTexture(fileName.C_Str());
        QOpenGLTexture *t = new QOpenGLTexture(QOpenGLTexture::Target2D);
        t->setMinMagFilters(QOpenGLTexture::LinearMipMapLinear, QOpenGLTexture::Linear);
        t->setWrapMode(QOpenGLTexture::Repeat);
        if (aiT){
            QImage img;
            if (aiT->mHeight == 0) {
                img = QImage::fromData(reinterpret_cast<uchar*>(aiT->pcData), aiT->mWidth, aiT->achFormatHint);
            } else {
                QPixmap pixmap(aiT->mWidth, aiT->mHeight);
                pixmap.loadFromData(reinterpret_cast<uchar*>(aiT->pcData), aiT->mWidth * aiT->mHeight);
                img = pixmap.toImage();
            }
            t->setData(img);
        } else {
            t->setData(QImage(m_fileDir.filePath(qFileName)));
        }
        m_Textures.append(t);
        texture.id = m_Textures.size() - 1;
        texture.type = typeName;
        texture.path = qFileName;
        textures.append(texture);
        m_loaded.append(texture);
    }
}

ModelMesh *Model::createModelMesh(const QVector<ModelVertex> &vertices, const QVector<uint> &indices, const QVector<TextureInfo> &textureInfos, const aiColor4D &diffuseColor)
{
    ModelMesh *modelMesh = new ModelMesh;
    modelMesh->vao.create();
    modelMesh->vbo.create();
    modelMesh->ebo.create();

    modelMesh->vao.bind();
    modelMesh->vbo.bind();
    modelMesh->vbo.allocate(vertices.data(), vertices.size() * sizeof(ModelVertex));
    modelMesh->ebo.bind();
    modelMesh->ebo.allocate(indices.data(), indices.size() * sizeof(uint));

    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    // 顶点
    f->glEnableVertexAttribArray(0);
    f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, sizeof(ModelVertex), 0);
    // 法线
    f->glEnableVertexAttribArray(1);
    f->glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, sizeof(ModelVertex), (void*)offsetof(ModelVertex, Normal));
    // 纹理坐标
    f->glEnableVertexAttribArray(2);
    f->glVertexAttribPointer(2, 2, GL_FLOAT, GL_TRUE, sizeof(ModelVertex),  (void*)offsetof(ModelVertex, TexCoord));
    // 切线
    f->glEnableVertexAttribArray(3);
    f->glVertexAttribPointer(3, 3, GL_FLOAT, GL_TRUE, sizeof(ModelVertex), (void*)offsetof(ModelVertex, Tangent));
    // 副切线
    f->glEnableVertexAttribArray(4);
    f->glVertexAttribPointer(4, 3, GL_FLOAT, GL_TRUE, sizeof(ModelVertex),  (void*)offsetof(ModelVertex, Bitangent));

    modelMesh->vbo.release();
    modelMesh->vao.release();
    modelMesh->ebo.release();
    modelMesh->textureInfos = textureInfos;
    modelMesh->diffuseColor = QVector4D(diffuseColor.r, diffuseColor.g, diffuseColor.b, diffuseColor.a);
    modelMesh->count = indices.size();
    return modelMesh;
}
