#ifndef CREATEOPENGLWINDOWEXAMPLE_H
#define CREATEOPENGLWINDOWEXAMPLE_H

#include <QtCore/qglobal.h>
#include <QOpenGLWidget>

class Q_DECL_EXPORT CreateOpenGLWindowExample : public QOpenGLWidget
{
    // Q_OBJECT

public:
    CreateOpenGLWindowExample(QWidget *parent = nullptr);
    ~CreateOpenGLWindowExample();

    // QOpenGLWidget interface
protected:
    virtual void initializeGL() override;
    virtual void resizeGL(int w, int h) override;
    virtual void paintGL() override;
};
#endif // CREATEOPENGLWINDOWEXAMPLE_H
