#include "MultipleLightsExample.h"

#include <QKeyEvent>
#include <QOpenGLFunctions>
#include <QPainter>
#include <QtMath>

MultipleLightsExample::MultipleLightsExample(QWidget *parent) :
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
    pointLightPositions << QVector3D( 0.7f,  0.2f,  2.0f)
                        << QVector3D( 2.3f, -3.3f, -4.0f)
                        << QVector3D(-4.0f,  2.0f, -12.0f)
                        << QVector3D( 0.0f,  0.0f, -3.0f);
}

MultipleLightsExample::~MultipleLightsExample()
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

void MultipleLightsExample::timerEvent(QTimerEvent *event)
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

void MultipleLightsExample::enterEvent(QEvent *event)
{
    // 隐藏鼠标指针，将指针置于窗口中心
    setCursor(Qt::BlankCursor);
    QCursor::setPos(mapToGlobal(rect().center()));
}

void MultipleLightsExample::leaveEvent(QEvent *event)
{
}

void MultipleLightsExample::mouseMoveEvent(QMouseEvent *event)
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

void MultipleLightsExample::wheelEvent(QWheelEvent *event)
{
    float f = event->angleDelta().y() > 0 ? 1.0f : -1.0f;
    camera.ProcessMouseScroll(f);
    projection.setToIdentity();
    projection.perspective(camera.Zoom, float(width()) / float(height()), 0.1f, 100.f);
}

void MultipleLightsExample::keyPressEvent(QKeyEvent *event)
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

void MultipleLightsExample::keyReleaseEvent(QKeyEvent *event)
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

void MultipleLightsExample::initializeGL()
{
    diffuseMap = new QOpenGLTexture(QImage("images/container2.png").mirrored());
    diffuseMap->setMinMagFilters(QOpenGLTexture::LinearMipMapLinear, QOpenGLTexture::Linear);
    specularMap = new QOpenGLTexture(QImage("images/container2_specular.png").mirrored());
    specularMap->setMinMagFilters(QOpenGLTexture::LinearMipMapLinear, QOpenGLTexture::Linear);

    QOpenGLFunctions *f = context()->functions();
    f->glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    lightingShader.addCacheableShaderFromSourceFile(QOpenGLShader::Vertex, "shaders/multiple_lights/multiple_lights.vs");
    lightingShader.addCacheableShaderFromSourceFile(QOpenGLShader::Fragment, "shaders/multiple_lights/multiple_lights.fs");
    if(!lightingShader.link()) {
        qDebug() << lightingShader.log();
    };
    lightCubeShader.addCacheableShaderFromSourceFile(QOpenGLShader::Vertex, "shaders/multiple_lights/light_cube.vs");
    lightCubeShader.addCacheableShaderFromSourceFile(QOpenGLShader::Fragment, "shaders/multiple_lights/light_cube.fs");
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

void MultipleLightsExample::resizeGL(int w, int h)
{
    QOpenGLFunctions *f = context()->functions();
    f->glViewport(0, 0, w, h);
    projection.setToIdentity();
    projection.perspective(camera.Zoom, float(w) / float(h), 0.1f, 100.f);
}

void MultipleLightsExample::paintGL()
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
    // directional light
    lightingShader.setUniformValue("uDirLight.direction", -0.2f, -1.0f, -0.3f);
    lightingShader.setUniformValue("uDirLight.ambient", 0.05f, 0.05f, 0.05f);
    lightingShader.setUniformValue("uDirLight.diffuse", 0.4f, 0.4f, 0.4f);
    lightingShader.setUniformValue("uDirLight.specular", 0.5f, 0.5f, 0.5f);
    // point light 1
    lightingShader.setUniformValue("uPointLights[0].position", pointLightPositions[0]);
    lightingShader.setUniformValue("uPointLights[0].ambient", 0.05f, 0.05f, 0.05f);
    lightingShader.setUniformValue("uPointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
    lightingShader.setUniformValue("uPointLights[0].specular", 1.0f, 1.0f, 1.0f);
    lightingShader.setUniformValue("uPointLights[0].constant", 1.0f);
    lightingShader.setUniformValue("uPointLights[0].linear", 0.09f);
    lightingShader.setUniformValue("uPointLights[0].quadratic", 0.032f);
    // point light 2
    lightingShader.setUniformValue("uPointLights[1].position", pointLightPositions[1]);
    lightingShader.setUniformValue("uPointLights[1].ambient", 0.05f, 0.05f, 0.05f);
    lightingShader.setUniformValue("uPointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
    lightingShader.setUniformValue("uPointLights[1].specular", 1.0f, 1.0f, 1.0f);
    lightingShader.setUniformValue("uPointLights[1].constant", 1.0f);
    lightingShader.setUniformValue("uPointLights[1].linear", 0.09f);
    lightingShader.setUniformValue("uPointLights[1].quadratic", 0.032f);
    // point light 3
    lightingShader.setUniformValue("uPointLights[2].position", pointLightPositions[2]);
    lightingShader.setUniformValue("uPointLights[2].ambient", 0.05f, 0.05f, 0.05f);
    lightingShader.setUniformValue("uPointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
    lightingShader.setUniformValue("uPointLights[2].specular", 1.0f, 1.0f, 1.0f);
    lightingShader.setUniformValue("uPointLights[2].constant", 1.0f);
    lightingShader.setUniformValue("uPointLights[2].linear", 0.09f);
    lightingShader.setUniformValue("uPointLights[2].quadratic", 0.032f);
    // point light 4
    lightingShader.setUniformValue("uPointLights[3].position", pointLightPositions[3]);
    lightingShader.setUniformValue("uPointLights[3].ambient", 0.05f, 0.05f, 0.05f);
    lightingShader.setUniformValue("uPointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
    lightingShader.setUniformValue("uPointLights[3].specular", 1.0f, 1.0f, 1.0f);
    lightingShader.setUniformValue("uPointLights[3].constant", 1.0f);
    lightingShader.setUniformValue("uPointLights[3].linear", 0.09f);
    lightingShader.setUniformValue("uPointLights[3].quadratic", 0.032f);
    // spotLight
    lightingShader.setUniformValue("uSpotLight.position", camera.Position);
    lightingShader.setUniformValue("uSpotLight.direction", camera.Front);
    lightingShader.setUniformValue("uSpotLight.ambient", 0.0f, 0.0f, 0.0f);
    lightingShader.setUniformValue("uSpotLight.diffuse", 1.0f, 1.0f, 1.0f);
    lightingShader.setUniformValue("uSpotLight.specular", 1.0f, 1.0f, 1.0f);
    lightingShader.setUniformValue("uSpotLight.constant", 1.0f);
    lightingShader.setUniformValue("uSpotLight.linear", 0.09f);
    lightingShader.setUniformValue("uSpotLight.quadratic", 0.032f);
    lightingShader.setUniformValue("uSpotLight.cutOff", float(qCos(qDegreesToRadians(12.5f))));
    lightingShader.setUniformValue("uSpotLight.outerCutOff", float(qCos(qDegreesToRadians(15.0f))));

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
    lightVao.bind();
    for (auto it = pointLightPositions.cbegin(); it != pointLightPositions.cend(); ++it) {
        QMatrix4x4 model;
        model.translate(*it);
        model.scale(0.2f);
        lightCubeShader.setUniformValue("uModel", model);
        f->glDrawArrays(GL_TRIANGLES, 0, 36);
    }
}
