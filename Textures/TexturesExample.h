#ifndef TEXTURESEXAMPLE_H
#define TEXTURESEXAMPLE_H

#include <QtCore/qglobal.h>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLTexture>
#include <QOpenGLWidget>

class Q_DECL_EXPORT TexturesExample : public QOpenGLWidget
{
public:
    TexturesExample(QWidget *parent = nullptr);
    ~TexturesExample();

protected:
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

};

#endif // TEXTURESEXAMPLE_H
