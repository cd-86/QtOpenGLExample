#include "CoordinateSystemsExample.h"

#include <QOpenGLFunctions>
#include <QtMath>

CoordinateSystemsExample::CoordinateSystemsExample(QWidget *parent)
{
    time.start();
    view.translate(0.0f, 0.0f, -3.0f);

    cubePositions << QVector3D( 0.0f,  0.0f,  0.0f)
                  << QVector3D( 2.0f,  5.0f, -15.0f)
                  << QVector3D(-1.5f, -2.2f, -2.5f)
                  << QVector3D(-3.8f, -2.0f, -12.3f)
                  << QVector3D( 2.4f, -0.4f, -3.5f)
                  << QVector3D(-1.7f,  3.0f, -7.5f)
                  << QVector3D( 1.3f, -2.0f, -2.5f)
                  << QVector3D( 1.5f,  2.0f, -2.5f)
                  << QVector3D( 1.5f,  0.2f, -1.5f)
                  << QVector3D(-1.3f,  1.0f, -1.5f);
}

CoordinateSystemsExample::~CoordinateSystemsExample()
{
    makeCurrent();
    vao.destroy();
    vbo.destroy();
    delete texture1;
    delete texture2;
    doneCurrent();
}

void CoordinateSystemsExample::timerEvent(QTimerEvent *event)
{
    update();
}

void CoordinateSystemsExample::initializeGL()
{
    texture1 = new QOpenGLTexture(QImage("images/container.jpg").mirrored());
    texture1->setMinMagFilters(QOpenGLTexture::LinearMipMapLinear, QOpenGLTexture::Linear);
    texture2 = new QOpenGLTexture(QImage("images/awesomeface.png").mirrored());
    texture2->setMinMagFilters(QOpenGLTexture::LinearMipMapLinear, QOpenGLTexture::Linear);

    QOpenGLFunctions *f = context()->functions();
    f->glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    // 启用深度测试
    f->glEnable(GL_DEPTH_TEST);
    // 顶点着色器
    shaderProgram.addCacheableShaderFromSourceFile(QOpenGLShader::Vertex, "shaders/coordinate_systems/texture.vs");
    // 片段着色器
    shaderProgram.addCacheableShaderFromSourceFile(QOpenGLShader::Fragment, "shaders/coordinate_systems/texture.fs");
    // 编译链接
    if(!shaderProgram.link()) {
        qDebug() << shaderProgram.log();
    };
    // 顶点数据
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };
    vao.create();
    vao.bind();
    vbo.create();
    vbo.bind();
    vbo.allocate(vertices, sizeof(vertices));
    shaderProgram.enableAttributeArray(0);
    shaderProgram.setAttributeBuffer(0, GL_FLOAT, 0, 3, 5 * sizeof(float));
    shaderProgram.enableAttributeArray(1);
    shaderProgram.setAttributeBuffer(1, GL_FLOAT, 3 * sizeof(float), 2, 5 * sizeof(float));
    startTimer(20);
}

void CoordinateSystemsExample::resizeGL(int w, int h)
{
    QOpenGLFunctions *f = context()->functions();
    f->glViewport(0, 0, w, h);
    projection.setToIdentity();
    projection.perspective(45.0f, float(w) / float(h), 0.1f, 100.f);

}

void CoordinateSystemsExample::paintGL()
{
    QOpenGLFunctions* f = context()->functions();
    f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shaderProgram.bind();
    vao.bind();
    texture1->bind(0);
    texture2->bind(1);
    shaderProgram.setUniformValue("uTexture1", 0);
    shaderProgram.setUniformValue("uTexture2", 1);
    shaderProgram.setUniformValue("uProjection", projection);
    shaderProgram.setUniformValue("uView", view);
    for(int i = 0; i < cubePositions.size(); ++i) {
        QMatrix4x4 model;
        model.translate(cubePositions[i]);
        model.rotate(time.elapsed() / 20.f + 20 * i, 1.5f, 0.3f, 0.5f);
        shaderProgram.setUniformValue("uModel", model);
        f->glDrawArrays(GL_TRIANGLES, 0, 36);
    }
}
