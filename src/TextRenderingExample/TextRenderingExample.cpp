#include "FontFace.h"
#include "TextRenderingExample.h"

#include <QOpenGLFunctions>

TextRenderingExample::TextRenderingExample(QWidget *parent)
    : QOpenGLWidget(parent)
{
}

TextRenderingExample::~TextRenderingExample() {
    makeCurrent();
    vao.destroy();
    vbo.destroy();
    ebo.destroy();
    texture->destroy();
    doneCurrent();
}

void TextRenderingExample::initializeGL()
{
    QOpenGLFunctions *f = context()->functions();
    f->glClearColor(0.f, 0.f, 0.f, 0.f);
    shaderProgram.addCacheableShaderFromSourceFile(QOpenGLShader::Vertex, "shaders/text_rendering/text_rendering.vs");
    shaderProgram.addCacheableShaderFromSourceFile(QOpenGLShader::Fragment, "shaders/text_rendering/text_rendering.fs");
    if(!shaderProgram.link()) {
        qDebug() << shaderProgram.log();
    };
    QVector<QVector4D> vertices;
    QVector<uint32_t> indices;
    FontFace fontFace;
    fontFace.getVertexData(vertices, indices, QStringLiteral("This is sample text\n这是一个示例文本"), 0, 0, Qt::AlignLeft, 20, 0, 0.5);
    vao.create();
    vao.bind();
    vbo.create();
    vbo.bind();
    ebo.create();
    ebo.bind();

    vbo.allocate(vertices.data(), sizeof(QVector4D) * vertices.size());
    ebo.allocate(indices.data(), sizeof(uint32_t) * indices.size());
    shaderProgram.enableAttributeArray(0);
    shaderProgram.setAttributeBuffer(0, GL_FLOAT, 0, 2, 4 * sizeof(float));
    shaderProgram.enableAttributeArray(1);
    shaderProgram.setAttributeBuffer(1, GL_FLOAT, 2 * sizeof(float), 2, 4 * sizeof(float));
    texture = new QOpenGLTexture(QOpenGLTexture::Target2D);
    texture->setMinMagFilters(QOpenGLTexture::LinearMipMapLinear, QOpenGLTexture::Linear);
    texture->setWrapMode(QOpenGLTexture::ClampToEdge);
    texture->setData(fontFace.getImage());

    shaderProgram.bind();
    shaderProgram.setUniformValue("uFontColor", QColor(128, 200, 150));
    shaderProgram.setUniformValue("uFontTextureSize", fontFace.textureWidth(), fontFace.textureHeight());
    count = indices.size();
}

void TextRenderingExample::resizeGL(int w, int h)
{
    QOpenGLFunctions *f = context()->functions();
    f->glViewport(0, 0, w, h);
    mat.setToIdentity();
    mat.ortho(0, w, 0, h, 0, 1);
    QMatrix4x4 mat2;
    mat2.translate(0, h - 24);
    mat *= mat2;

}

void TextRenderingExample::paintGL()
{
    QOpenGLFunctions* f = context()->functions();
    f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    shaderProgram.bind();
    shaderProgram.setUniformValue("uMatrix", mat);
    vao.bind();
    texture->bind();
    f->glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, 0);
}
