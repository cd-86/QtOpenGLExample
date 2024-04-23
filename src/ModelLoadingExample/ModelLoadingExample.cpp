#include "ModelLoadingExample.h"

#include <QKeyEvent>
#include <QOpenGLFunctions>
#include <QPainter>
#include <QtMath>

ModelLoadingExample::ModelLoadingExample(QWidget *parent) :
    QOpenGLWidget(parent)
{
    setMouseTracking(true);
}

ModelLoadingExample::~ModelLoadingExample()
{
    makeCurrent();
    delete ourModel;
    doneCurrent();
}

void ModelLoadingExample::timerEvent(QTimerEvent *event)
{
    float s = time.restart() / 1000.0;
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

void ModelLoadingExample::enterEvent(QEvent *event)
{
    // 隐藏鼠标指针，将指针置于窗口中心
    setCursor(Qt::BlankCursor);
    QCursor::setPos(mapToGlobal(rect().center()));
}

void ModelLoadingExample::leaveEvent(QEvent *event)
{
}

void ModelLoadingExample::mouseMoveEvent(QMouseEvent *event)
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

void ModelLoadingExample::wheelEvent(QWheelEvent *event)
{
    float f = event->angleDelta().y() > 0 ? 1.0f : -1.0f;
    camera.ProcessMouseScroll(f);
    projection.setToIdentity();
    projection.perspective(camera.Zoom, float(width()) / float(height()), 0.1f, 100.f);
}

void ModelLoadingExample::keyPressEvent(QKeyEvent *event)
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

void ModelLoadingExample::keyReleaseEvent(QKeyEvent *event)
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

void ModelLoadingExample::initializeGL()
{
    ourModel = new Model("model/nanosuit/nanosuit.obj");

    QOpenGLFunctions *f = context()->functions();
    f->glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    shader.addCacheableShaderFromSourceFile(QOpenGLShader::Vertex, "shaders/model_loading/model_loading.vs");
    shader.addCacheableShaderFromSourceFile(QOpenGLShader::Fragment, "shaders/model_loading/model_loading.fs");
    if(!shader.link()) {
        qDebug() << shader.log();
    };
    startTimer(1);
}

void ModelLoadingExample::resizeGL(int w, int h)
{
    QOpenGLFunctions *f = context()->functions();
    f->glViewport(0, 0, w, h);
    projection.setToIdentity();
    projection.perspective(camera.Zoom, float(w) / float(h), 0.1f, 100.f);
}

void ModelLoadingExample::paintGL()
{
    QOpenGLFunctions* f = context()->functions();
    f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // 启用深度测试
    f->glEnable(GL_DEPTH_TEST);

    shader.bind();

    shader.setUniformValue("uViewPos", camera.Position);
    // directional light
    shader.setUniformValue("uShininess", 32.0f);
    // 平行光
    shader.setUniformValue("dirLight.direction", -0.001000f, 0.001000f, -0.999999f);
    shader.setUniformValue("dirLight.ambient", 0.2f, 0.2f, 0.2f);
    shader.setUniformValue("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
    shader.setUniformValue("dirLight.specular", 0.5f, 0.5f, 0.5f);
    // spotLight
    shader.setUniformValue("spotLight.ambient", 0.0f, 0.0f, 0.0f);
    shader.setUniformValue("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
    shader.setUniformValue("spotLight.specular", 1.0f, 1.0f, 1.0f);
    shader.setUniformValue("spotLight.constant", 1.0f);
    shader.setUniformValue("spotLight.linear", 0.09f);;
    shader.setUniformValue("spotLight.quadratic", 0.032f);
    shader.setUniformValue("spotLight.cutOff", cosf(qDegreesToRadians(12.5f)));
    shader.setUniformValue("spotLight.outerCutOff", cosf(qDegreesToRadians(30.0f)));

    shader.setUniformValue("spotLight.position", camera.Position);
    shader.setUniformValue("spotLight.direction", camera.Front);
    shader.setUniformValue("uViewPos", camera.Position);
    // uMatrix = projection * view * model
    shader.setUniformValue("uMatrix", projection * view);
    shader.setUniformValue("uModel", QMatrix4x4());
    shader.setUniformValue("uNormalMatrix", QMatrix3x3());
    ourModel->draw(shader);
}
