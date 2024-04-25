#ifndef GEOMETRYSHADEREXAMPLE_H
#define GEOMETRYSHADEREXAMPLE_H

#include "GeometryShaderExample_global.h"
#include <QOpenGLShaderProgram>
#include <QOpenGLWidget>
#include <QCheckBox>

#include "Camera.h"
#include "Model.h"

class GEOMETRYSHADEREXAMPLE_EXPORT GeometryShaderExample : public QOpenGLWidget
{
    Q_OBJECT
public:
    explicit GeometryShaderExample(QWidget *parent = nullptr);
    ~GeometryShaderExample();

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
    QCheckBox *normalChechBox;
    QCheckBox *meshCheckBox;

    QOpenGLShaderProgram modelShader;
    QOpenGLShaderProgram normalShader;
    QOpenGLShaderProgram meshShader;
    Model *ourModel;
    QMatrix4x4 projection;
    QMatrix4x4 view;
    Camera camera;
    QPoint previousPos;
};

#endif // GEOMETRYSHADEREXAMPLE_H
