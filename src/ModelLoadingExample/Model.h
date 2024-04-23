#ifndef MODEL_H
#define MODEL_H

#include <QVector>
#include <QOpenGLShaderProgram>
#include <QDir>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "QOpenGLTexture"

#define DIFFUSE_TEX_NAME "texture_diffuse"
#define SPECULAR_TEX_NAME "texture_specular"
#define NORMALS_TEX_NAME "texture_normals"

struct TextureInfo {
    uint id;
    QString type;
    QString path;
    QVector4D color;
};

struct ModelVec3 {
    float x;
    float y;
    float z;
};

struct ModelVec2 {
    float x;
    float y;
};

struct ModelVertex {
    ModelVec3 Position;
    ModelVec3 Normal;
    ModelVec2 TexCoord;
    ModelVec3 Tangent;
    ModelVec3 Bitangent;
};

struct ModelMesh
{
    ModelMesh(): ebo(QOpenGLBuffer(QOpenGLBuffer::IndexBuffer)){}
    ~ModelMesh() {
        vao.destroy();
        vbo.destroy();
        ebo.destroy();
    }

    QOpenGLBuffer vbo;
    QOpenGLBuffer ebo;
    QOpenGLVertexArrayObject vao;
    QVector<TextureInfo> textureInfos;
    QVector4D diffuseColor;
    int count {0};
};


class Model
{

public:
    Model(const QString &filePath);
    ~Model();

    bool loadModel(const QString &filePath);
    void draw(QOpenGLShaderProgram &shader);

private:
    void processNode(const aiNode *node, const aiScene *scene);
    ModelMesh *processMesh(const aiMesh *mesh, const aiScene *scene);
    void loadMaterialTextures(const aiScene *scene, const aiMaterial *mat, aiTextureType type, const QString &typeName, QVector<TextureInfo> &textures);
    ModelMesh *createModelMesh(const QVector<ModelVertex> &vertices, const QVector<uint> &indices, const QVector<TextureInfo> &textureInfos, const aiColor4D &diffuseColor);

private:
    QDir m_fileDir;
    QVector<ModelMesh*> m_meshes;
    QVector<TextureInfo> m_loaded; // 记录加载过的纹理.
    QList<QOpenGLTexture*> m_Textures;
};

#endif // MODEL_H
