#include "HelloTriangleExample.h"

#include <QOpenGLFunctions>

HelloTriangleExample::HelloTriangleExample(QWidget *parent)
    : QOpenGLWidget(parent)
{
}

HelloTriangleExample::~HelloTriangleExample()
{
    makeCurrent();
    vao.destroy();
    vbo.destroy();
    ebo.destroy();
    doneCurrent();
}

void HelloTriangleExample::initializeGL()
{
    // 设置用来清空屏幕的颜色 这里设置为黑色
    QOpenGLFunctions *f = context()->functions();
    f->glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    // 顶点着色器
    shaderProgram.addCacheableShaderFromSourceCode(QOpenGLShader::Vertex, R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;

        void main()
        {
            gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
        }
    )");
    // 片段着色器
    shaderProgram.addCacheableShaderFromSourceCode(QOpenGLShader::Fragment, R"(
        #version 330 core
        out vec4 FragColor;

        void main()
        {
            FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
        }
    )");
    // 编译链接
    if(!shaderProgram.link()) {
        qDebug() << shaderProgram.log();
    };
    // 顶点数据
    float vertices[] = {
        0.5f, 0.5f, 0.0f,   // 右上角
        0.5f, -0.5f, 0.0f,  // 右下角
        -0.5f, -0.5f, 0.0f, // 左下角
        -0.5f, 0.5f, 0.0f   // 左上角
    };
    // 顶点索引
    uint indices[] = {
        // 注意索引从0开始!
        // 此例的索引(0,1,2,3)就是顶点数组vertices的下标，
        // 这样可以由下标代表顶点组合成矩形

        0, 1, 3, // 第一个三角形
        1, 2, 3  // 第二个三角形
    };
    // 创建VAO VBO EBO
    vao.create();
    vao.bind();
    vbo.create();
    vbo.bind();
    ebo.create();
    ebo.bind();
    vbo.allocate(vertices, sizeof(vertices));
    ebo.allocate(indices, sizeof(indices));
    shaderProgram.enableAttributeArray(0);
    shaderProgram.setAttributeBuffer(0, GL_FLOAT, 0, 3, 3 * sizeof(float));

}

void HelloTriangleExample::resizeGL(int w, int h)
{
    // 窗口尺寸发生变化时，设置视口大小， 这里把视口设置与窗口一样大
    QOpenGLFunctions *f = context()->functions();
    f->glViewport(0, 0, w, h);
}

void HelloTriangleExample::paintGL()
{
    // 每次重绘之前清空屏幕颜色和深度缓冲
    QOpenGLFunctions* f = context()->functions();
    f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shaderProgram.bind();
    vao.bind();
    f->glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
