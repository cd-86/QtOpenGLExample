#include "TexturesExample.h"

#include <QOpenGLFunctions>

TexturesExample::TexturesExample(QWidget *parent)
{

}

TexturesExample::~TexturesExample()
{
    makeCurrent();
    vao.destroy();
    vbo.destroy();
    ebo.destroy();
    delete texture1;
    delete texture2;
    doneCurrent();
}

void TexturesExample::initializeGL()
{
    texture1 = new QOpenGLTexture(QImage("images/container.jpg").mirrored());
    // 纹理环绕方式
    // texture1->setWrapMode(GL_REPEAT);
    // 纹理过滤方式
    texture1->setMinMagFilters(QOpenGLTexture::LinearMipMapLinear, QOpenGLTexture::Linear);
    texture2 = new QOpenGLTexture(QImage("images/awesomeface.png").mirrored());
    texture2->setMinMagFilters(QOpenGLTexture::LinearMipMapLinear, QOpenGLTexture::Linear);

    QOpenGLFunctions *f = context()->functions();
    f->glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    // 顶点着色器
    shaderProgram.addCacheableShaderFromSourceFile(QOpenGLShader::Vertex, "shaders/textures/texture.vs");
    // 片段着色器
    shaderProgram.addCacheableShaderFromSourceFile(QOpenGLShader::Fragment, "shaders/textures/texture.fs");
    // 编译链接
    if(!shaderProgram.link()) {
        qDebug() << shaderProgram.log();
    };
    // 顶点数据
    float vertices[] = {
        //     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
        0.5f,  0.5f, 0.0f, 1.0f, 1.0f,   // 右上
        0.5f, -0.5f, 0.0f, 1.0f, 0.0f,   // 右下
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,   // 左下
        -0.5f,  0.5f, 0.0f, 0.0f, 1.0f    // 左上
    };
    // 顶点索引
    uint indices[] = {
        0, 1, 2, // 第一个三角形
        0, 2, 3  // 第二个三角形
    };
    vao.create();
    vao.bind();
    vbo.create();
    vbo.bind();
    ebo.create();
    ebo.bind();
    vbo.allocate(vertices, sizeof(vertices));
    ebo.allocate(indices, sizeof(indices));
    shaderProgram.enableAttributeArray(0);
    shaderProgram.setAttributeBuffer(0, GL_FLOAT, 0, 3, 5 * sizeof(float));
    shaderProgram.enableAttributeArray(1);
    shaderProgram.setAttributeBuffer(1, GL_FLOAT, 3 * sizeof(float), 2, 5 * sizeof(float));
}

void TexturesExample::resizeGL(int w, int h)
{
    QOpenGLFunctions *f = context()->functions();
    f->glViewport(0, 0, w, h);
}

void TexturesExample::paintGL()
{
    QOpenGLFunctions* f = context()->functions();
    f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shaderProgram.bind();
    vao.bind();
    texture1->bind(0);
    texture2->bind(1);
    shaderProgram.setUniformValue("uTexture1", 0);
    shaderProgram.setUniformValue("uTexture2", 1);
    f->glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
