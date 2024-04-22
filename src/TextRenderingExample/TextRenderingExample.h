#ifndef TEXTRENDERINGEXAMPLE_H
#define TEXTRENDERINGEXAMPLE_H

#include "TextRenderingExample_global.h"
#include <QOpenGLWidget>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLTexture>

class TEXTRENDERINGEXAMPLE_EXPORT TextRenderingExample : public QOpenGLWidget
{
    Q_OBJECT

public:
    TextRenderingExample(QWidget *parent = nullptr);
    ~TextRenderingExample();

    // QOpenGLWidget interface
protected:
    virtual void initializeGL() override;
    virtual void resizeGL(int w, int h) override;
    virtual void paintGL() override;

public:
    QOpenGLShaderProgram shaderProgram;
    QOpenGLBuffer vbo;
    QOpenGLBuffer ebo {QOpenGLBuffer(QOpenGLBuffer::IndexBuffer)};
    QOpenGLVertexArrayObject vao;
    QOpenGLTexture *texture;
    QMatrix4x4 mat;
    int count;

};

#endif // TEXTRENDERINGEXAMPLE_H
