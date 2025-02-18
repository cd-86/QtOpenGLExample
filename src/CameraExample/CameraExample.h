/**
 *
 * W: 相机前进
 * S: 相机后退
 * A: 相机左移
 * D: 相机右移
 * 滚轮: 更改FOV
 * 鼠标移动: 转动相机视角
 *
 */

#ifndef CAMERAEXAMPLE_H
#define CAMERAEXAMPLE_H

#include "CameraExample_global.h"
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLTexture>
#include <QOpenGLWidget>
#include <QElapsedTimer>

class CAMERAEXAMPLE_EXPORT CameraExample : public QOpenGLWidget
{
    Q_OBJECT
public:
    CameraExample(QWidget *parent = nullptr);
    ~CameraExample();

protected:
    // QObject interface
    virtual void timerEvent(QTimerEvent *event) override;
    // QWidget interface
    virtual void enterEvent(QEvent *event) override;
    virtual void leaveEvent(QEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void wheelEvent(QWheelEvent *event) override;
    virtual void keyPressEvent(QKeyEvent *event) override;
    virtual void keyReleaseEvent(QKeyEvent *event) override;
    // QOpenGLWidget interface
    virtual void initializeGL() override;
    virtual void resizeGL(int w, int h) override;
    virtual void paintGL() override;

public:
    QOpenGLShaderProgram shaderProgram;
    QOpenGLBuffer vbo;
    QOpenGLVertexArrayObject vao;
    QOpenGLTexture *texture1;
    QOpenGLTexture *texture2;
    QMatrix4x4 projection;
    QMatrix4x4 view;
    QVector3D cameraPos {QVector3D(0.0f, 0.0f,  3.0f)};
    QVector3D cameraFront {QVector3D(0.0f, 0.0f, -1.0f)};
    QVector3D cameraUp {QVector3D(0.0f, 1.0f,  0.0f)};
    float yaw   = 0.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
    float pitch =  0.0f;
    float fov   =  45.0f;
    QVector<QVector3D> cubePositions;
    QElapsedTimer time;
    struct {
        bool W {false};
        bool S {false};
        bool A {false};
        bool D {false};
    } keys;
    int timerID {0};
};

#endif // CAMERAEXAMPLE_H
