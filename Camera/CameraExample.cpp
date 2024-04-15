#include "CameraExample.h"

#include <QKeyEvent>
#include <QOpenGLFunctions>
#include <QPainter>
#include <QtMath>

CameraExample::CameraExample(QWidget *parent)
{
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
    setMouseTracking(true);
}

CameraExample::~CameraExample()
{
    makeCurrent();
    vao.destroy();
    vbo.destroy();
    delete texture1;
    delete texture2;
    doneCurrent();
}

void CameraExample::timerEvent(QTimerEvent *event)
{
    float cameraSpeed = time.restart() / 1000.f * 2.5f;
    if (keys.W)
        cameraPos += cameraSpeed * cameraFront;
    if (keys.S)
        cameraPos -= cameraSpeed * cameraFront;
    if (keys.A)
        cameraPos -= QVector3D::crossProduct(cameraFront, cameraUp).normalized() * cameraSpeed;
    if (keys.D)
        cameraPos += QVector3D::crossProduct(cameraFront, cameraUp).normalized() * cameraSpeed;
    view.setToIdentity();
    view.lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    update();
}

void CameraExample::enterEvent(QEvent *event)
{
    // 隐藏鼠标指针，将指针置于窗口中心
    setCursor(Qt::BlankCursor);
    QCursor::setPos(mapToGlobal(rect().center()));
}

void CameraExample::leaveEvent(QEvent *event)
{
}

void CameraExample::mouseMoveEvent(QMouseEvent *event)
{

    float xoffset = rect().center().x() - event->x();
    float yoffset = rect().center().y() - event->y();

    float sensitivity = 0.1f; // change this value to your liking
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;
    // 使用四元数计算当前相机看向的方向
    cameraFront = QQuaternion::fromEulerAngles(pitch, yaw, 0) * QVector3D(0.0f, 0.0f, -1.0f);

    view.setToIdentity();
    view.lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    update();
    // 将指针置于窗口中心
    QCursor::setPos(mapToGlobal(rect().center()));
}

void CameraExample::wheelEvent(QWheelEvent *event)
{
    fov -= event->angleDelta().y() > 0 ? 1.0f : -1.0f;
    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 45.0f)
        fov = 45.0f;
    projection.setToIdentity();
    projection.perspective(fov, float(width()) / float(height()), 0.1f, 100.f);
    update();
}

void CameraExample::keyPressEvent(QKeyEvent *event)
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
    if (!timerID){
        timerID = startTimer(1);
        time.restart();
    }
}

void CameraExample::keyReleaseEvent(QKeyEvent *event)
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
    if (!(keys.W || keys.S || keys.A || keys.D)) {
        killTimer(timerID);
        timerID = 0;
    }
}

void CameraExample::initializeGL()
{
    texture1 = new QOpenGLTexture(QImage("images/container.jpg").mirrored());
    texture1->setMinMagFilters(QOpenGLTexture::LinearMipMapLinear, QOpenGLTexture::Linear);
    texture2 = new QOpenGLTexture(QImage("images/awesomeface.png").mirrored());
    texture2->setMinMagFilters(QOpenGLTexture::LinearMipMapLinear, QOpenGLTexture::Linear);

    QOpenGLFunctions *f = context()->functions();
    f->glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
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
}

void CameraExample::resizeGL(int w, int h)
{
    QOpenGLFunctions *f = context()->functions();
    f->glViewport(0, 0, w, h);
    projection.setToIdentity();
    projection.perspective(fov, float(w) / float(h), 0.1f, 100.f);
}

void CameraExample::paintGL()
{
    QOpenGLFunctions* f = context()->functions();
    f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // 启用深度测试
    f->glEnable(GL_DEPTH_TEST);
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
        model.rotate(20 * i, 1.5f, 0.3f, 0.5f);
        shaderProgram.setUniformValue("uModel", model);
        f->glDrawArrays(GL_TRIANGLES, 0, 36);
    }
}
