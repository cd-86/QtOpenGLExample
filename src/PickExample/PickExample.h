#ifndef PICKEXAMPLE_H
#define PICKEXAMPLE_H

#include "PickExample_global.h"
#include <QOpenGLShaderProgram>
#include <QOpenGLWidget>
#include <QCheckBox>

#include "Camera.h"
#include "Model.h"
#include "Line.h"
#include "World.h"

struct Ray {
    // 射线的位置和方向
    QVector3D lightStartPos;
    QVector3D lightEndPos;
    QVector3D lightDir;
};

struct Frustum {
    // 平截头体的 8 个顶点
    QVector3D far1;   // top left
    QVector3D far2;   // top right
    QVector3D far3;   // bottom left
    QVector3D far4;   // bottom right
    QVector3D near1;
    QVector3D near2;
    QVector3D near3;
    QVector3D near4;
};

struct model {
    QMatrix4x4 matrix;
    bool pick {false};
};

Ray Pos2Ray(float x, float y, float screenWidth, float screenHeight, const QMatrix4x4 &viewMat, const QMatrix4x4 &projectionMat);

Frustum Rect2Frustum(float x1, float y1, float x2, float y2, float screenWidth, float screenHeight, const QMatrix4x4 &viewMat, const QMatrix4x4 &projectionMat);

class PICKEXAMPLE_EXPORT PickExample : public QOpenGLWidget
{
    Q_OBJECT
public:
    explicit PickExample(QWidget *parent = nullptr);
    ~PickExample();

protected:
    // QWidget interface
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void wheelEvent(QWheelEvent *event) override;
    // QOpenGLWidget interface
    virtual void initializeGL() override;
    virtual void resizeGL(int w, int h) override;
    virtual void paintGL() override;

public:
    QCheckBox *AABBChechBox;
    QCheckBox *OBBChechBox;
    QCheckBox *pickChechBox;

    QOpenGLShaderProgram modelShader;
    QOpenGLShaderProgram lineShader;
    Line *line;
    World *world;
    Model *ourModel;
    QMatrix4x4 projection;
    QMatrix4x4 view;
    QVector<model> models;
    QVector<World::CollisionMatrix> boundingBox;
    Camera camera;
    struct {
        QPoint firstPos;
        QPoint previousPos;
        bool isLeft {false};
        bool isMove {false};
    } mousePressed;
};

#endif // PICKEXAMPLE_H
