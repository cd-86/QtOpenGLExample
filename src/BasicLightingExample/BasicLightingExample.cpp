#include "BasicLightingExample.h"

#include <QKeyEvent>
#include <QOpenGLFunctions>
#include <QPainter>
#include <QtMath>

BasicLightingExample::BasicLightingExample(QWidget *parent) :
    QOpenGLWidget(parent)
{
    setMouseTracking(true);
}

BasicLightingExample::~BasicLightingExample()
{
    makeCurrent();
    lightVao.destroy();
    cubeVao.destroy();
    vbo.destroy();
    doneCurrent();
}

void BasicLightingExample::timerEvent(QTimerEvent *event)
{
    float s = time.restart() / 1000.f;
    if (keys.W)
        camera.ProcessKeyboard(FORWARD, s);
    if (keys.S)
        camera.ProcessKeyboard(BACKWARD, s);
    if (keys.A)
        camera.ProcessKeyboard(LEFT, s);
    if (keys.D)
        camera.ProcessKeyboard(RIGHT, s);
    view = camera.GetViewMatrix();
    update();
}

void BasicLightingExample::enterEvent(QEvent *event)
{
    // 隐藏鼠标指针，将指针置于窗口中心
    setCursor(Qt::BlankCursor);
    QCursor::setPos(mapToGlobal(rect().center()));
}

void BasicLightingExample::leaveEvent(QEvent *event)
{
}

void BasicLightingExample::mouseMoveEvent(QMouseEvent *event)
{
    float xoffset = rect().center().x() - event->x();
    float yoffset = rect().center().y() - event->y();

    float sensitivity = 0.1f; // change this value to your liking
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    camera.ProcessMouseMovement(xoffset, yoffset);

    // 将指针置于窗口中心
    QCursor::setPos(mapToGlobal(rect().center()));
}

void BasicLightingExample::wheelEvent(QWheelEvent *event)
{
    float f = event->angleDelta().y() > 0 ? 1.0f : -1.0f;
    camera.ProcessMouseScroll(f);
    projection.setToIdentity();
    projection.perspective(camera.Zoom, float(width()) / float(height()), 0.1f, 100.f);
}

void BasicLightingExample::keyPressEvent(QKeyEvent *event)
{
    switch(event->key()) {
    case Qt::Key_W:
        keys.W = true;
        break;
    case Qt::Key_S:
        keys.S = true;
        break;
    case Qt::Key_A:
        keys.A = true;
        break;
    case Qt::Key_D:
        keys.D = true;
        break;
    default:
        return;
    }
}

void BasicLightingExample::keyReleaseEvent(QKeyEvent *event)
{
    switch(event->key()) {
    case Qt::Key_W:
        keys.W = false;
        break;
    case Qt::Key_S:
        keys.S = false;
        break;
    case Qt::Key_A:
        keys.A = false;
        break;
    case Qt::Key_D:
        keys.D = false;
        break;
    default:
        return;
    }
}

void BasicLightingExample::initializeGL()
{
    QOpenGLFunctions *f = context()->functions();
    f->glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    lightingShader.addCacheableShaderFromSourceFile(QOpenGLShader::Vertex, "shaders/basic_lighting/basic_lighting.vs");
    lightingShader.addCacheableShaderFromSourceFile(QOpenGLShader::Fragment, "shaders/basic_lighting/basic_lighting.fs");
    if(!lightingShader.link()) {
        qDebug() << lightingShader.log();
    };
    lightCubeShader.addCacheableShaderFromSourceFile(QOpenGLShader::Vertex, "shaders/basic_lighting/light_cube.vs");
    lightCubeShader.addCacheableShaderFromSourceFile(QOpenGLShader::Fragment, "shaders/basic_lighting/light_cube.fs");
    if(!lightCubeShader.link()) {
        qDebug() << lightCubeShader.log();
    };
    // 顶点数据
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };
    vbo.create();
    vbo.bind();
    vbo.allocate(vertices, sizeof(vertices));

    cubeVao.create();
    cubeVao.bind();
    lightingShader.enableAttributeArray(0);
    lightingShader.setAttributeBuffer(0, GL_FLOAT, 0, 3, 6 * sizeof(float));
    lightingShader.enableAttributeArray(1);
    lightingShader.setAttributeBuffer(1, GL_FLOAT, 3 * sizeof(float), 3, 6 * sizeof(float));

    lightVao.create();
    lightVao.bind();
    lightCubeShader.enableAttributeArray(0);
    lightCubeShader.setAttributeBuffer(0, GL_FLOAT, 0, 3, 6 * sizeof(float));
    startTimer(1);
}

void BasicLightingExample::resizeGL(int w, int h)
{
    QOpenGLFunctions *f = context()->functions();
    f->glViewport(0, 0, w, h);
    projection.setToIdentity();
    projection.perspective(camera.Zoom, float(w) / float(h), 0.1f, 100.f);
}

void BasicLightingExample::paintGL()
{
    QOpenGLFunctions* f = context()->functions();
    f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // 启用深度测试
    f->glEnable(GL_DEPTH_TEST);

    lightingShader.bind();
    lightingShader.setUniformValue("uObjectColor", 1.0f, 0.5f, 0.31f);
    lightingShader.setUniformValue("uLightColor", 1.0f, 1.0f, 1.0f);
    lightingShader.setUniformValue("uLightPos", lightPos);
    lightingShader.setUniformValue("uViewPos", camera.Position);
    lightingShader.setUniformValue("uProjection", projection);
    lightingShader.setUniformValue("uView", view);
    lightingShader.setUniformValue("uModel", QMatrix4x4());
    cubeVao.bind();
    f->glDrawArrays(GL_TRIANGLES, 0, 36);

    lightCubeShader.bind();
    lightCubeShader.setUniformValue("uProjection", projection);
    lightCubeShader.setUniformValue("uView", view);
    QMatrix4x4 model;
    model.translate(lightPos);
    model.scale(0.2f);
    lightCubeShader.setUniformValue("uModel", model);
    lightVao.bind();
    f->glDrawArrays(GL_TRIANGLES, 0, 36);
}
