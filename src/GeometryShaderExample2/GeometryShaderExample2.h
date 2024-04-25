#ifndef GEOMETRYSHADEREXAMPLE2_H
#define GEOMETRYSHADEREXAMPLE2_H

#include "GeometryShaderExample2_global.h"
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLTexture>
#include <QOpenGLWidget>

#include "Camera.h"

class GEOMETRYSHADEREXAMPLE2_EXPORT GeometryShaderExample2 : public QOpenGLWidget
{
    Q_OBJECT
public:
    explicit GeometryShaderExample2(QWidget *parent = nullptr);
    ~GeometryShaderExample2();

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
    QOpenGLTexture *texture;
    QOpenGLShaderProgram shader;
    QOpenGLVertexArrayObject vao;
    QOpenGLBuffer vbo;
    QMatrix4x4 projection;
    QMatrix4x4 view;
    Camera camera;
    QPoint previousPos;
};

#endif // GEOMETRYSHADEREXAMPLE2_H
