#ifndef MYOPENGLWIDGET_H
#define MYOPENGLWIDGET_H

#include <QOpenGLWidget>

class MyOpenGLWidget : public QOpenGLWidget
{
    Q_OBJECT

public:
    MyOpenGLWidget(QWidget *parent = nullptr);
    ~MyOpenGLWidget();

    // QOpenGLWidget interface
protected:
    virtual void initializeGL() override;
    virtual void resizeGL(int w, int h) override;
    virtual void paintGL() override;
};
#endif // MYOPENGLWIDGET_H
