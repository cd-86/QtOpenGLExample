#include "MyOpenGLWidget.h"

#include <QOpenGLFunctions>

MyOpenGLWidget::MyOpenGLWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{}

MyOpenGLWidget::~MyOpenGLWidget() {}

void MyOpenGLWidget::initializeGL()
{
    // 设置用来清空屏幕的颜色 这里设置为黑色
    QOpenGLFunctions *f = context()->functions();
    f->glClearColor(0.f, 0.f, 0.f, 0.f);
}

void MyOpenGLWidget::resizeGL(int w, int h)
{
    // 窗口尺寸发生变化时，设置视口大小， 这里把视口设置与窗口一样大
    QOpenGLFunctions *f = context()->functions();
    f->glViewport(0, 0, w, h);
}

void MyOpenGLWidget::paintGL()
{
    // 每次重绘之前清空屏幕颜色和深度缓冲
    QOpenGLFunctions* f = context()->functions();
    f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
