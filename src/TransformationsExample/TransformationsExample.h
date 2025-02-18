#ifndef TRANSFORMATIONSEXAMPLE_H
#define TRANSFORMATIONSEXAMPLE_H

#include "TransformationsExample_global.h"
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLTexture>
#include <QOpenGLWidget>
#include <QElapsedTimer>

class TRANSFORMATIONSEXAMPLE_EXPORT TransformationsExample : public QOpenGLWidget
{
    Q_OBJECT
public:
    TransformationsExample(QWidget *parent = nullptr);
    ~TransformationsExample();

protected:
    // QObject interface
    virtual void timerEvent(QTimerEvent *event) override;
    // QOpenGLWidget interface
    virtual void initializeGL() override;
    virtual void resizeGL(int w, int h) override;
    virtual void paintGL() override;

public:
    QOpenGLShaderProgram shaderProgram;
    QOpenGLBuffer vbo;
    QOpenGLBuffer ebo {QOpenGLBuffer(QOpenGLBuffer::IndexBuffer)};
    QOpenGLVertexArrayObject vao;
    QOpenGLTexture *texture1;
    QOpenGLTexture *texture2;
    QMatrix4x4 trans;
    QElapsedTimer time;

};

#endif // TRANSFORMATIONSEXAMPLE_H
