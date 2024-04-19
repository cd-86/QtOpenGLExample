#include "MaterialsExample.h"

#include <QKeyEvent>
#include <QOpenGLFunctions>
#include <QPainter>
#include <QtMath>

MaterialsExample::MaterialsExample(QWidget *parent) :
    QOpenGLWidget(parent)
{
    setMouseTracking(true);
}

MaterialsExample::~MaterialsExample()
{
    makeCurrent();
    lightVao.destroy();
    cubeVao.destroy();
    vbo.destroy();
    doneCurrent();
}

void MaterialsExample::timerEvent(QTimerEvent *event)
{
    float s = time.elapsed() / 1000.0;
    float delta = s - lastFrameTime;
    lastFrameTime = s;
    if (keys.W)
        camera.ProcessKeyboard(FORWARD, delta);
    if (keys.S)
        camera.ProcessKeyboard(BACKWARD, delta);
    if (keys.A)
        camera.ProcessKeyboard(LEFT, delta);
    if (keys.D)
        camera.ProcessKeyboard(RIGHT, delta);
    view = camera.GetViewMatrix();
    QVector3D lightColor(qSin(lastFrameTime * 2.0), qSin(lastFrameTime * 0.7), qSin(lastFrameTime * 1.3));
    diffuseColor = lightColor * 0.5;
    ambientColor = lightColor * 0.2;
    update();
}

void MaterialsExample::enterEvent(QEvent *event)
{
    // 隐藏鼠标指针，将指针置于窗口中心
    setCursor(Qt::BlankCursor);
    QCursor::setPos(mapToGlobal(rect().center()));
}

void MaterialsExample::leaveEvent(QEvent *event)
{
}

void MaterialsExample::mouseMoveEvent(QMouseEvent *event)
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

void MaterialsExample::wheelEvent(QWheelEvent *event)
{
    float f = event->angleDelta().y() > 0 ? 1.0f : -1.0f;
    camera.ProcessMouseScroll(f);
    projection.setToIdentity();
    projection.perspective(camera.Zoom, float(width()) / float(height()), 0.1f, 100.f);
}

void MaterialsExample::keyPressEvent(QKeyEvent *event)
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

void MaterialsExample::keyReleaseEvent(QKeyEvent *event)
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

void MaterialsExample::initializeGL()
{
    QOpenGLFunctions *f = context()->functions();
    f->glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    lightingShader.addCacheableShaderFromSourceFile(QOpenGLShader::Vertex, "shaders/materials/materials.vs");
    lightingShader.addCacheableShaderFromSourceFile(QOpenGLShader::Fragment, "shaders/materials/materials.fs");
    if(!lightingShader.link()) {
        qDebug() << lightingShader.log();
    };
    lightCubeShader.addCacheableShaderFromSourceFile(QOpenGLShader::Vertex, "shaders/materials/light_cube.vs");
    lightCubeShader.addCacheableShaderFromSourceFile(QOpenGLShader::Fragment, "shaders/materials/light_cube.fs");
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
    time.start();
}

void MaterialsExample::resizeGL(int w, int h)
{
    QOpenGLFunctions *f = context()->functions();
    f->glViewport(0, 0, w, h);
    projection.setToIdentity();
    projection.perspective(camera.Zoom, float(w) / float(h), 0.1f, 100.f);
}

void MaterialsExample::paintGL()
{
    QOpenGLFunctions* f = context()->functions();
    f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // 启用深度测试
    f->glEnable(GL_DEPTH_TEST);

    lightingShader.bind();
    lightingShader.setUniformValue("uViewPos", camera.Position);
    lightingShader.setUniformValue("uLight.position", lightPos);
    lightingShader.setUniformValue("uLight.ambient", ambientColor);
    lightingShader.setUniformValue("uLight.diffuse", diffuseColor);
    lightingShader.setUniformValue("uLight.specular", 1.0f, 1.0f, 1.0f);

    lightingShader.setUniformValue("uMaterial.ambient", 1.0f, 0.5f, 0.31f);
    lightingShader.setUniformValue("uMaterial.diffuse", 1.0f, 0.5f, 0.31f);
    lightingShader.setUniformValue("uMaterial.specular", 0.5f, 0.5f, 0.5f);
    lightingShader.setUniformValue("uMaterial.shininess", 32.0f);

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
