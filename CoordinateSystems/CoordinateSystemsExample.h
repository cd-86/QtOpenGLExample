#ifndef COORDINATESYSTEMSEXAMPLE_H
#define COORDINATESYSTEMSEXAMPLE_H

#include <QtCore/qglobal.h>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLTexture>
#include <QOpenGLWidget>
#include <QTime>

class Q_DECL_EXPORT CoordinateSystemsExample : public QOpenGLWidget
{
public:
    CoordinateSystemsExample(QWidget *parent = nullptr);
    ~CoordinateSystemsExample();

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
    QOpenGLTexture *texture1;
    QOpenGLTexture *texture2;
    QMatrix4x4 projection;
    QMatrix4x4 view;
    QTime time;
    QVector<QVector3D> cubePositions;

};

#endif // COORDINATESYSTEMSEXAMPLE_H
