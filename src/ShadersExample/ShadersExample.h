#ifndef SHADERSEXAMPLE_H
#define SHADERSEXAMPLE_H

#include "ShadersExample_global.h"
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLWidget>
#include <QElapsedTimer>

class SHADERSEXAMPLE_EXPORT ShadersExample : public QOpenGLWidget
{
    Q_OBJECT
public:
    ShadersExample(QWidget *parent = nullptr);
    ~ShadersExample();

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
    QOpenGLVertexArrayObject vao;
    QColor color;
    QElapsedTimer time;
};

#endif // SHADERSEXAMPLE_H
