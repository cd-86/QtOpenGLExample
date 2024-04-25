#include "GeometryShaderExample.h"

#include <QMouseEvent>
#include <QOpenGLFunctions>
#include <QTime>
#include <QVBoxLayout>
#include <QtMath>


GeometryShaderExample::GeometryShaderExample(QWidget *parent) :
    QOpenGLWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignTop | Qt::AlignLeft);

    normalChechBox = new QCheckBox(QStringLiteral("法线"));
    layout->addWidget(normalChechBox);
    meshCheckBox = new QCheckBox(QStringLiteral("网格/模型"));
    layout->addWidget(meshCheckBox);

    connect(normalChechBox, &QCheckBox::clicked, this, [=](){update();});
    connect(meshCheckBox, &QCheckBox::clicked, this, [=](){update();});
}

GeometryShaderExample::~GeometryShaderExample()
{
    makeCurrent();
    delete ourModel;
    doneCurrent();
}

void GeometryShaderExample::mousePressEvent(QMouseEvent *event)
{
    // 记录按下去的位置
    previousPos = event->pos();
}

void GeometryShaderExample::mouseReleaseEvent(QMouseEvent *event)
{
}
void GeometryShaderExample::mouseMoveEvent(QMouseEvent *event)
{
    float xoffset = previousPos.x() - event->x();
    float yoffset = event->y() - previousPos.y();
    previousPos = event->pos();

    camera.ProcessMouseMovement(xoffset, yoffset);
    view = camera.GetViewMatrix();
    update();
}

void GeometryShaderExample::wheelEvent(QWheelEvent *event)
{
    // 调整相机的距离
    float f = event->angleDelta().y() > 0 ? 0.9f : 1.1f;
    camera.ProcessMouseScroll(f);
    view = camera.GetViewMatrix();
    update();
}

void GeometryShaderExample::initializeGL()
{
    // 加载一只甲虫
    ourModel = new Model("model/stag_beetle.glb");
    QOpenGLFunctions *f = context()->functions();
    f->glClearColor(0.5f, 0.6f, 0.7f, 0.0f);
    // 模型的着色器程序
    modelShader.addCacheableShaderFromSourceFile(QOpenGLShader::Vertex, "shaders/geometry_shader/model.vs");
    modelShader.addCacheableShaderFromSourceFile(QOpenGLShader::Fragment, "shaders/geometry_shader/model.fs");
    if(!modelShader.link()) {
        qDebug() << modelShader.log();
    };
    // 把三角型面转为法线的着色器程序 (计算每个三角面的法线)
    normalShader.addCacheableShaderFromSourceFile(QOpenGLShader::Vertex, "shaders/geometry_shader/normal.vs");
    normalShader.addCacheableShaderFromSourceFile(QOpenGLShader::Geometry, "shaders/geometry_shader/normal.geom");
    normalShader.addCacheableShaderFromSourceFile(QOpenGLShader::Fragment, "shaders/geometry_shader/normal.fs");
    if(!normalShader.link()) {
        qDebug() << normalShader.log();
    };
    // 把三角型面转为线的的着色器程序
    meshShader.addCacheableShaderFromSourceFile(QOpenGLShader::Vertex, "shaders/geometry_shader/mesh.vs");
    meshShader.addCacheableShaderFromSourceFile(QOpenGLShader::Geometry, "shaders/geometry_shader/mesh.geom");
    meshShader.addCacheableShaderFromSourceFile(QOpenGLShader::Fragment, "shaders/geometry_shader/mesh.fs");
    if(!meshShader.link()) {
        qDebug() << meshShader.log();
    };
    // 更新一次 view 矩阵
    view = camera.GetViewMatrix();
}

void GeometryShaderExample::resizeGL(int w, int h)
{
    QOpenGLFunctions *f = context()->functions();
    f->glViewport(0, 0, w, h);
    projection.setToIdentity();
    projection.perspective(45.f, float(w) / float(h), 0.01f, 10000.f);
}

void GeometryShaderExample::paintGL()
{
    QMatrix4x4 mat = projection * view;

    QOpenGLFunctions* f = context()->functions();
    f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // 启用深度测试
    f->glEnable(GL_DEPTH_TEST);

    if (meshCheckBox->isChecked()) {
        meshShader.bind();
        meshShader.setUniformValue("uMatrix", mat);
        ourModel->draw(meshShader);
    } else {
        modelShader.bind();

        modelShader.setUniformValue("uViewPos", camera.Position);
        // directional light
        modelShader.setUniformValue("uShininess", 32.0f);
        // 平行光
        modelShader.setUniformValue("dirLight.direction", -0.001000f, 0.001000f, -0.999999f);
        modelShader.setUniformValue("dirLight.ambient", 1.0f, 1.0f, 1.0f);
        modelShader.setUniformValue("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
        modelShader.setUniformValue("dirLight.specular", 0.5f, 0.5f, 0.5f);
        // spotLight
        modelShader.setUniformValue("spotLight.ambient", 0.0f, 0.0f, 0.0f);
        modelShader.setUniformValue("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
        modelShader.setUniformValue("spotLight.specular", 1.0f, 1.0f, 1.0f);
        modelShader.setUniformValue("spotLight.constant", 1.0f);
        modelShader.setUniformValue("spotLight.linear", 0.09f);;
        modelShader.setUniformValue("spotLight.quadratic", 0.032f);
        modelShader.setUniformValue("spotLight.cutOff", cosf(qDegreesToRadians(12.5f)));
        modelShader.setUniformValue("spotLight.outerCutOff", cosf(qDegreesToRadians(30.0f)));

        modelShader.setUniformValue("spotLight.position", camera.Position);
        modelShader.setUniformValue("spotLight.direction", camera.Front);
        modelShader.setUniformValue("uViewPos", camera.Position);

        modelShader.setUniformValue("uMatrix", mat);
        modelShader.setUniformValue("uModel", QMatrix4x4());
        modelShader.setUniformValue("uNormalMatrix", QMatrix3x3());
        ourModel->draw(modelShader);
    }

    if (normalChechBox->isChecked()) {
        normalShader.bind();
        normalShader.setUniformValue("uMatrix", mat);
        ourModel->draw(normalShader);
    }
}
