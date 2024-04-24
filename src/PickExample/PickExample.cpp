#include "PickExample.h"

#include <QMouseEvent>
#include <QOpenGLFunctions>
#include <QTime>
#include <QVBoxLayout>
#include <QtMath>

/**
 * @brief Pos2Ray 点击的屏幕坐标转为 由相机朝向相机观察方向发射的一根射线的世界坐标
 */
Ray Pos2Ray(float x, float y, float screenWidth, float screenHeight, const QMatrix4x4 &viewMat, const QMatrix4x4 &projectionMat)
{
    // 转换到标准设备坐标.
    x = (2.0f * x / screenWidth) - 1.0f;
    y = 1.0f - (2.0f * y / screenHeight);
    QVector4D near1(x, y, -1.f, 1.f);
    QVector4D far1(x, y, 1.f, 1.f);
    // 转换到视图坐标.
    QMatrix4x4 iProjectionMat = projectionMat.inverted();

    near1 = iProjectionMat * near1;
    far1 = iProjectionMat * far1;
    near1 /= near1.w();
    far1 /= far1.w();
    // 转换到世界坐标.
    QMatrix4x4 iViewMat = viewMat.inverted();
    near1 = iViewMat * near1;
    far1 = iViewMat * far1;

    return Ray {
        QVector3D(near1),
        QVector3D(far1),
        QVector3D(far1 - near1)
    };
}

/**
 * @brief Rect2Frustum 框选的屏幕坐标转为 由相机朝向相机观察方向的平截头体的世界坐标
 */
Frustum Rect2Frustum(float x1, float y1, float x2, float y2, float screenWidth, float screenHeight, const QMatrix4x4 &viewMat, const QMatrix4x4 &projectionMat)
{
    // 转换标准设备坐标
    x1 = (2.0f * x1 / screenWidth) - 1.0f;
    y1 = 1.0f - (2.0f * y1 / screenHeight);
    x2 = (2.0f * x2 / screenWidth) - 1.0f;
    y2 = 1.0f - (2.0f * y2 / screenHeight);

    QVector4D near1(x1, y1, -1.f, 1.f);
    QVector4D near2(x2, y1, -1.f, 1.f);
    QVector4D near3(x1, y2, -1.f, 1.f);
    QVector4D near4(x2, y2, -1.f, 1.f);
    QVector4D far1(x1, y1, 1.f, 1.f);
    QVector4D far2(x2, y1, 1.f, 1.f);
    QVector4D far3(x1, y2, 1.f, 1.f);
    QVector4D far4(x2, y2, 1.f, 1.f);

    // 转换到视图坐标
    QMatrix4x4 iProjectionMat = projectionMat.inverted();
    near1 = iProjectionMat * near1;
    near2 = iProjectionMat * near2;
    near3 = iProjectionMat * near3;
    near4 = iProjectionMat * near4;
    far1 = iProjectionMat * far1;
    far2 = iProjectionMat * far2;
    far3 = iProjectionMat * far3;
    far4 = iProjectionMat * far4;
    near1 /= near1.w();
    near2 /= near2.w();
    near3 /= near3.w();
    near4 /= near4.w();
    far1 /= far1.w();
    far2 /= far2.w();
    far3 /= far3.w();
    far4 /= far4.w();
    // 转换到世界坐标
    QMatrix4x4 iViewMat = viewMat.inverted();
    near1 = iViewMat * near1;
    near2 = iViewMat * near2;
    near3 = iViewMat * near3;
    near4 = iViewMat * near4;
    far1 = iViewMat * far1;
    far2 = iViewMat * far2;
    far3 = iViewMat * far3;
    far4 = iViewMat * far4;

    return Frustum {
        QVector3D(far1),
        QVector3D(far2),
        QVector3D(far3),
        QVector3D(far4),
        QVector3D(near1),
        QVector3D(near2),
        QVector3D(near3),
        QVector3D(near4),
    };
}

PickExample::PickExample(QWidget *parent) :
    QOpenGLWidget(parent),
    models(QVector<model>(20))
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignTop | Qt::AlignLeft);

    AABBChechBox = new QCheckBox(QStringLiteral("显示 AABB"));
    layout->addWidget(AABBChechBox);

    OBBChechBox = new QCheckBox(QStringLiteral("显示 OBB"));
    layout->addWidget(OBBChechBox);

    pickChechBox = new QCheckBox(QStringLiteral("显示 射线/选择框"));
    layout->addWidget(pickChechBox);

    connect(AABBChechBox, &QCheckBox::clicked, this, [=](){update();});
    connect(OBBChechBox, &QCheckBox::clicked, this, [=](){update();});
    connect(pickChechBox, &QCheckBox::clicked, this, [=](){update();});


    // 20只甲虫
    world = new World;
    qsrand(QDateTime::currentMSecsSinceEpoch());
    for (int i = 0; i < models.size(); ++i) {
        // 随机位置和朝向
        QVector3D pos(qrand() % 100 - 50, qrand() % 100 - 50, qrand() % 100 - 50);
        QQuaternion q = QQuaternion::fromEulerAngles(qrand() % 360, qrand() % 360, 0);
        models[i].matrix.translate(pos);
        models[i].matrix.rotate(q);
        world->addBoxRigidBody(pos + q * QVector3D(0.f, 5.0f, -2.7f), QVector3D(6.7f, 3.5f, 13.6f), q, nullptr, i); // 记录下对应甲虫的索引
    }
    // 获取 碰撞体的包围盒矩阵
    boundingBox = world->getAllCollisionAabbAndObb();
}

PickExample::~PickExample()
{
    delete world;
    makeCurrent();
    delete line;
    delete ourModel;
    doneCurrent();
}

void PickExample::mousePressEvent(QMouseEvent *event)
{
    // 记录按下去的位置
    mousePressed.previousPos = event->pos();
    mousePressed.firstPos = event->pos();
    if (event->button() == Qt::LeftButton) {
        mousePressed.isLeft = true;
        mousePressed.isMove = false;
    }
}

void PickExample::mouseReleaseEvent(QMouseEvent *event)
{
    // 只关心左键
    if (!mousePressed.isLeft) return;
    mousePressed.isLeft = false;
    // 先把所有甲虫设为未选中状态
    for (int i = 0; i < models.size(); ++i) {
        models[i].pick = false;
    }

    if (mousePressed.isMove) { // 矩形框选
        Frustum f = Rect2Frustum(
            mousePressed.firstPos.x(),
            mousePressed.firstPos.y(),
            event->pos().x(),
            event->pos().y(),
            width(),
            height(),
            view,
            projection);
        QVector3D d[] = {
            f.near1, f.near2,
            f.near3, f.near4,
            f.near1, f.near3,
            f.near2, f.near4,

            f.far1, f.far2,
            f.far3, f.far4,
            f.far1, f.far3,
            f.far2, f.far4,

            f.near1, f.far1,
            f.near2, f.far2,
            f.near3, f.far3,
            f.near4, f.far4,
        };
        // 设置框选的平截头体的顶点
        line->setFrustum(d);
        // 获取碰撞体，根据碰撞体里记录的索引修改对应的甲虫为选中状态
        QVector<btCollisionObject*> cs = world->getCollisionTestObjects(&f.far1, 8);
        for (auto obj : cs) {
            models[obj->getUserIndex()].pick = true;
        }

    } else { // 点选
        Ray r = Pos2Ray(
            mousePressed.firstPos.x(),
            mousePressed.firstPos.y(),
            width(),
            height(),
            view,
            projection);
        // 设置射线的顶点
        line->setRay((void*)&r);
        // 获取碰撞体，根据碰撞体里记录的索引修改对应的甲虫为选中状态
        const btCollisionObject* obj = world->getRayTestObject(r.lightStartPos, r.lightDir);
        if (obj) {
            models[obj->getUserIndex()].pick = true;
        }
    }
    update();

}
void PickExample::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() == Qt::RightButton) {
        // 右键移动 是旋转相机 更新 view 矩阵
        float xoffset = mousePressed.previousPos.x() - event->x();
        float yoffset = event->y() - mousePressed.previousPos.y();
        mousePressed.previousPos = event->pos();

        camera.ProcessMouseMovement(xoffset, yoffset);
        view = camera.GetViewMatrix();
    } else if (event->buttons() == Qt::LeftButton) {
        // 左键移动 是框选
        mousePressed.isMove = true;
        // 计算出矩形框区域，写入缓冲
        float x1 = (2.0f * mousePressed.firstPos.x() / width()) - 1.0f;
        float y1 = 1.0f - (2.0f * mousePressed.firstPos.y() / height());
        float x2 = (2.0f * event->pos().x() / width()) - 1.0f;
        float y2 = 1.0f - (2.0f * event->pos().y() / height());
        float d[] = {
            x1, y1, 0,
            x2, y1, 0,
            x2, y1, 0,
            x2, y2, 0,

            x1, y1, 0,
            x1, y2, 0,
            x1, y2, 0,
            x2, y2, 0,
        };
        line->setRect(d);
    }
    update();
}

void PickExample::wheelEvent(QWheelEvent *event)
{
    // 调整相机的距离
    float f = event->angleDelta().y() > 0 ? 0.9f : 1.1f;
    camera.ProcessMouseScroll(f);
    view = camera.GetViewMatrix();
    update();
}

void PickExample::initializeGL()
{
    // 加载一只甲虫
    ourModel = new Model("model/stag_beetle.glb");
    line = new Line;
    QOpenGLFunctions *f = context()->functions();
    f->glClearColor(0.5f, 0.6f, 0.7f, 0.0f);
    // 模型的着色器程序
    modelShader.addCacheableShaderFromSourceFile(QOpenGLShader::Vertex, "shaders/pick/model.vs");
    modelShader.addCacheableShaderFromSourceFile(QOpenGLShader::Fragment, "shaders/pick/model.fs");
    if(!modelShader.link()) {
        qDebug() << modelShader.log();
    };
    // 线条的着色器程序
    lineShader.addCacheableShaderFromSourceFile(QOpenGLShader::Vertex, "shaders/pick/line.vs");
    lineShader.addCacheableShaderFromSourceFile(QOpenGLShader::Fragment, "shaders/pick/line.fs");
    if(!lineShader.link()) {
        qDebug() << lineShader.log();
    };
    // 更新一次 view 矩阵
    view = camera.GetViewMatrix();
}

void PickExample::resizeGL(int w, int h)
{
    QOpenGLFunctions *f = context()->functions();
    f->glViewport(0, 0, w, h);
    projection.setToIdentity();
    projection.perspective(45.f, float(w) / float(h), 0.01f, 10000.f);
}

void PickExample::paintGL()
{
    QMatrix4x4 mat = projection * view;

    QOpenGLFunctions* f = context()->functions();
    f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // 启用深度测试
    f->glEnable(GL_DEPTH_TEST);

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
    // 画出所有甲虫
    for (int i = 0; i < models.size(); ++i) {
        // uMatrix = projection * view * model
        modelShader.setUniformValue("uMatrix", mat * models[i].matrix);
        modelShader.setUniformValue("uModel", models[i].matrix);
        modelShader.setUniformValue("uNormalMatrix", models[i].matrix.normalMatrix());
        // 设置选中状态
        modelShader.setUniformValue("uPick", models[i].pick);
        ourModel->draw(modelShader);
    }

    lineShader.bind();
    line->bind();
    lineShader.setUniformValue("uColor", QColor(255, 0, 0));
    // 画出所有甲虫的包围盒
    for ( auto it= boundingBox.cbegin(); it != boundingBox.cend(); ++it) {
        // AABB
        if (AABBChechBox->isChecked()) {
            lineShader.setUniformValue("uColor", QColor(255, 0, 0));
            lineShader.setUniformValue("uMatrix", mat * it->aabb);
            line->drawCube();
        }
        // OBB
        if (OBBChechBox->isChecked()) {
            lineShader.setUniformValue("uColor", QColor(0, 255, 0));
            lineShader.setUniformValue("uMatrix", mat * it->obb);
            line->drawCube();
        }
    }
    // 左键按下框选时，画出框选的矩形区域
    // 松开时 画出 射线或平截头体
    if (mousePressed.isLeft && mousePressed.isMove) {
        lineShader.setUniformValue("uColor", QColor(255, 255, 255));
        lineShader.setUniformValue("uMatrix", QMatrix());
        line->drawRect();
    } else if (pickChechBox->isChecked()) {
        lineShader.setUniformValue("uColor", QColor(255, 255, 0));
        lineShader.setUniformValue("uMatrix", mat);
        if (mousePressed.isMove)
            line->drawFrustum();
        else
            line->drawRay();
    }
}
