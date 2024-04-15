#include "ShadersExample.h"

#include <QOpenGLFunctions>
#include <QtMath>

ShadersExample::ShadersExample(QWidget *parent)
{
    time.start();
}

ShadersExample::~ShadersExample()
{
    makeCurrent();
    vao.destroy();
    vbo.destroy();
    doneCurrent();
}

void ShadersExample::timerEvent(QTimerEvent *event)
{
    color.setGreen((qSin(time.elapsed() / 500.f) / 2.f + 0.5f) * 255);
    update();
}

void ShadersExample::initializeGL()
{

    // 设置用来清空屏幕的颜色 这里设置为黑色
    QOpenGLFunctions *f = context()->functions();
    f->glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    // 顶点着色器
    shaderProgram.addCacheableShaderFromSourceCode(QOpenGLShader::Vertex, R"(
        #version 330 core
        layout (location = 0) in vec3 aPos; // 位置变量的属性位置值为0

        void main()
        {
            gl_Position = vec4(aPos, 1.0); // 注意我们如何把一个vec3作为vec4的构造器的参数
        }
    )");
    // 片段着色器
    shaderProgram.addCacheableShaderFromSourceCode(QOpenGLShader::Fragment, R"(
        #version 330 core
        out vec4 FragColor;

        uniform vec4 uColor;

        void main()
        {
            FragColor = uColor;
        }
    )");
    // 编译链接
    if(!shaderProgram.link()) {
        qDebug() << shaderProgram.log();
    };
    // 三角形顶点数据
    float vertices[] = {
        0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        0.0f,  0.5f, 0.0f
    };
    vao.create();
    vao.bind();
    vbo.create();
    vbo.bind();
    vbo.allocate(vertices, sizeof(vertices));
    shaderProgram.enableAttributeArray(0);
    shaderProgram.setAttributeBuffer(0, GL_FLOAT, 0, 3, 3 * sizeof(float));
    // 启动计时器
    startTimer(20);
}

void ShadersExample::resizeGL(int w, int h)
{
    // 窗口尺寸发生变化时，设置视口大小， 这里把视口设置与窗口一样大
    QOpenGLFunctions *f = context()->functions();
    f->glViewport(0, 0, w, h);
}

void ShadersExample::paintGL()
{
    // 每次重绘之前清空屏幕颜色和深度缓冲
    QOpenGLFunctions* f = context()->functions();
    f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shaderProgram.bind();
    vao.bind();
    shaderProgram.setUniformValue("uColor", color);
    f->glDrawArrays(GL_TRIANGLES, 0, 3);
}
