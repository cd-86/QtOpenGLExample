#ifndef HELLOTRIANGLEEXAMPLE_H
#define HELLOTRIANGLEEXAMPLE_H

#include <QtCore/qglobal.h>
#include <QOpenGLWidget>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>


class Q_DECL_EXPORT HelloTriangleExample : public QOpenGLWidget
{
    // Q_OBJECT

public:
    HelloTriangleExample(QWidget *parent = nullptr);
    ~HelloTriangleExample();

    // QOpenGLWidget interface
protected:
    virtual void initializeGL() override;
    virtual void resizeGL(int w, int h) override;
    virtual void paintGL() override;

private:
    QOpenGLShaderProgram shaderProgram;
    QOpenGLBuffer vbo;
    QOpenGLBuffer ebo {QOpenGLBuffer(QOpenGLBuffer::IndexBuffer)};
    QOpenGLVertexArrayObject vao;
};
#endif // HELLOTRIANGLEEXAMPLE_H
