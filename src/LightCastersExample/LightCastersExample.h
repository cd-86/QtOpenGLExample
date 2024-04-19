﻿#ifndef LIGHTCASTERSEXAMPLE_H
#define LIGHTCASTERSEXAMPLE_H

#include "LightCastersExample_global.h"
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLWidget>
#include <QElapsedTimer>
#include <QOpenGLTexture>

#include "Camera.h"

class LIGHTCASTERSEXAMPLE_EXPORT LightCastersExample : public QOpenGLWidget
{
    Q_OBJECT
public:
    explicit LightCastersExample(QWidget *parent = nullptr);
    ~LightCastersExample();

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
    QOpenGLShaderProgram lightingShader;
    QOpenGLShaderProgram lightCubeShader;
    QOpenGLBuffer vbo;
    QOpenGLVertexArrayObject cubeVao;
    QOpenGLVertexArrayObject lightVao;
    QMatrix4x4 projection;
    QMatrix4x4 view;
    Camera camera {Camera(QVector3D(0.0f, 0.0f, 3.0f))};
    QVector3D lightPos {QVector3D(1.2f, 1.0f, 2.0f)};
    QOpenGLTexture *diffuseMap;
    QOpenGLTexture *specularMap;
    QVector<QVector3D> cubePositions;
    QElapsedTimer time;
    struct {
        bool W {false};
        bool S {false};
        bool A {false};
        bool D {false};
    } keys;
};

#endif // LIGHTCASTERSEXAMPLE_H
