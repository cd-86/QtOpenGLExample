#include "GeometryShaderExample2.h"

#include <QMouseEvent>
#include <QOpenGLFunctions>
#include <QTime>
#include <QVBoxLayout>
#include <QtMath>


GeometryShaderExample2::GeometryShaderExample2(QWidget *parent) :
    QOpenGLWidget(parent)
{
}

GeometryShaderExample2::~GeometryShaderExample2()
{
    makeCurrent();
    vao.destroy();
    vbo.destroy();
    texture->destroy();
    delete texture;
    doneCurrent();
}

void GeometryShaderExample2::mousePressEvent(QMouseEvent *event)
{
    // 记录按下去的位置
    previousPos = event->pos();
}

void GeometryShaderExample2::mouseReleaseEvent(QMouseEvent *event)
{
}
void GeometryShaderExample2::mouseMoveEvent(QMouseEvent *event)
{
    float xoffset = previousPos.x() - event->x();
    float yoffset = event->y() - previousPos.y();
    previousPos = event->pos();

    camera.ProcessMouseMovement(xoffset, yoffset);
    view = camera.GetViewMatrix();
    update();
}

void GeometryShaderExample2::wheelEvent(QWheelEvent *event)
{
    // 调整相机的距离
    float f = event->angleDelta().y() > 0 ? 0.9f : 1.1f;
    camera.ProcessMouseScroll(f);
    view = camera.GetViewMatrix();
    update();
}

void GeometryShaderExample2::initializeGL()
{
    texture = new QOpenGLTexture(QImage("images/dog.png").mirrored());
    // 纹理环绕方式
    // texture1->setWrapMode(GL_REPEAT);
    // 纹理过滤方式
    texture->setMinMagFilters(QOpenGLTexture::LinearMipMapLinear, QOpenGLTexture::Linear);
    QOpenGLFunctions *f = context()->functions();
    f->glClearColor(0.5f, 0.6f, 0.7f, 0.0f);
    f->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // 模型的着色器程序
    shader.addCacheableShaderFromSourceFile(QOpenGLShader::Vertex, "shaders/geometry_shader2/shader.vs");
    shader.addCacheableShaderFromSourceFile(QOpenGLShader::Fragment, "shaders/geometry_shader2/shader.fs");
    shader.addCacheableShaderFromSourceFile(QOpenGLShader::Geometry, "shaders/geometry_shader2/shader.geom");
    if(!shader.link()) {
        qDebug() << shader.log();
    };

    QVector3D d[100];
    qsrand(QDateTime::currentMSecsSinceEpoch());
    for (int i = 0; i < 100; ++i) {
        d[i].setX(qrand() % 100 - 50);
        d[i].setY(qrand() % 100 - 50);
        d[i].setZ(qrand() % 100 - 50);
    }

    vao.create();
    vbo.create();
    vao.bind();
    vbo.bind();
    vbo.allocate(d, 100 * sizeof(QVector3D));
    shader.enableAttributeArray(0);
    shader.setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(QVector3D));

    // 更新一次 view 矩阵
    view = camera.GetViewMatrix();
}

void GeometryShaderExample2::resizeGL(int w, int h)
{
    QOpenGLFunctions *f = context()->functions();
    f->glViewport(0, 0, w, h);
    projection.setToIdentity();
    projection.perspective(45.f, float(w) / float(h), 0.01f, 10000.f);
}

void GeometryShaderExample2::paintGL()
{
    QMatrix4x4 mat = projection * view;

    QOpenGLFunctions* f = context()->functions();
    f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // 启用深度测试
    f->glEnable(GL_DEPTH_TEST);
    f->glEnable(GL_BLEND);
    shader.bind();
    shader.setUniformValue("uMatrix", mat);
    texture->bind();
    vao.bind();
    f->glDrawArrays(GL_POINTS, 0, 100);
}
