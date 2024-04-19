#include "LightCastersExample.h"

#include <QKeyEvent>
#include <QOpenGLFunctions>
#include <QPainter>
#include <QtMath>

LightCastersExample::LightCastersExample(QWidget *parent) :
    QOpenGLWidget(parent)
{
    setMouseTracking(true);
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

LightCastersExample::~LightCastersExample()
{
    makeCurrent();
    lightVao.destroy();
    cubeVao.destroy();
    vbo.destroy();
    diffuseMap->destroy();
    delete diffuseMap;
    specularMap->destroy();
    delete specularMap;
    doneCurrent();
}

void LightCastersExample::timerEvent(QTimerEvent *event)
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

void LightCastersExample::enterEvent(QEvent *event)
{
    // 隐藏鼠标指针，将指针置于窗口中心
    setCursor(Qt::BlankCursor);
    QCursor::setPos(mapToGlobal(rect().center()));
}

void LightCastersExample::leaveEvent(QEvent *event)
{
}

void LightCastersExample::mouseMoveEvent(QMouseEvent *event)
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

void LightCastersExample::wheelEvent(QWheelEvent *event)
{
    float f = event->angleDelta().y() > 0 ? 1.0f : -1.0f;
    camera.ProcessMouseScroll(f);
    projection.setToIdentity();
    projection.perspective(camera.Zoom, float(width()) / float(height()), 0.1f, 100.f);
}

void LightCastersExample::keyPressEvent(QKeyEvent *event)
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

void LightCastersExample::keyReleaseEvent(QKeyEvent *event)
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

void LightCastersExample::initializeGL()
{
    diffuseMap = new QOpenGLTexture(QImage("images/container2.png").mirrored());
    diffuseMap->setMinMagFilters(QOpenGLTexture::LinearMipMapLinear, QOpenGLTexture::Linear);
    specularMap = new QOpenGLTexture(QImage("images/container2_specular.png").mirrored());
    specularMap->setMinMagFilters(QOpenGLTexture::LinearMipMapLinear, QOpenGLTexture::Linear);

    QOpenGLFunctions *f = context()->functions();
    f->glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    lightingShader.addCacheableShaderFromSourceFile(QOpenGLShader::Vertex, "shaders/light_casters/light_casters.vs");
    lightingShader.addCacheableShaderFromSourceFile(QOpenGLShader::Fragment, "shaders/light_casters/light_casters.fs");
    if(!lightingShader.link()) {
        qDebug() << lightingShader.log();
    };
    lightCubeShader.addCacheableShaderFromSourceFile(QOpenGLShader::Vertex, "shaders/light_casters/light_cube.vs");
    lightCubeShader.addCacheableShaderFromSourceFile(QOpenGLShader::Fragment, "shaders/light_casters/light_cube.fs");
    if(!lightCubeShader.link()) {
        qDebug() << lightCubeShader.log();
    };
    // 顶点数据
    float vertices[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
    };
    vbo.create();
    vbo.bind();
    vbo.allocate(vertices, sizeof(vertices));

    cubeVao.create();
    cubeVao.bind();
    lightingShader.enableAttributeArray(0);
    lightingShader.setAttributeBuffer(0, GL_FLOAT, 0, 3, 8 * sizeof(float));
    lightingShader.enableAttributeArray(1);
    lightingShader.setAttributeBuffer(1, GL_FLOAT, 3 * sizeof(float), 3, 8 * sizeof(float));
    lightingShader.enableAttributeArray(2);
    lightingShader.setAttributeBuffer(2, GL_FLOAT, 6 * sizeof(float), 2, 8 * sizeof(float));

    lightVao.create();
    lightVao.bind();
    lightCubeShader.enableAttributeArray(0);
    lightCubeShader.setAttributeBuffer(0, GL_FLOAT, 0, 3, 8 * sizeof(float));
    startTimer(1);
}

void LightCastersExample::resizeGL(int w, int h)
{
    QOpenGLFunctions *f = context()->functions();
    f->glViewport(0, 0, w, h);
    projection.setToIdentity();
    projection.perspective(camera.Zoom, float(w) / float(h), 0.1f, 100.f);
}

void LightCastersExample::paintGL()
{
    QOpenGLFunctions* f = context()->functions();
    f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // 启用深度测试
    f->glEnable(GL_DEPTH_TEST);

    lightingShader.bind();
    diffuseMap->bind(0);
    specularMap->bind(1);
    lightingShader.setUniformValue("uMaterial.diffuse", 0);
    lightingShader.setUniformValue("uMaterial.specular", 1);

    lightingShader.setUniformValue("uViewPos", camera.Position);
    lightingShader.setUniformValue("uLight.position", camera.Position);
    lightingShader.setUniformValue("uLight.direction", camera.Front);
    lightingShader.setUniformValue("uLight.cutOff", qDegreesToRadians(17.5f));

    lightingShader.setUniformValue("uLight.ambient", 0.1f, 0.1f, 0.1f);
    lightingShader.setUniformValue("uLight.diffuse", 0.8f, 0.8f, 0.8f);
    lightingShader.setUniformValue("uLight.specular", 1.0f, 1.0f, 1.0f);
    lightingShader.setUniformValue("uLight.constant", 1.0f);
    lightingShader.setUniformValue("uLight.linear", 0.09f);
    lightingShader.setUniformValue("uLight.quadratic", 0.032f);

    lightingShader.setUniformValue("uMaterial.shininess", 32.0f);

    lightingShader.setUniformValue("uProjection", projection);
    lightingShader.setUniformValue("uView", view);
    cubeVao.bind();
    for (int i = 0; i < cubePositions.size(); ++i) {
        QMatrix4x4 mat;
        mat.translate(cubePositions[i]);
        float angle = 20.0f * i;
        mat.rotate(angle, 1.0f, 0.3f, 0.5f);
        lightingShader.setUniformValue("uModel", mat);
        f->glDrawArrays(GL_TRIANGLES, 0, 36);
    }

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
