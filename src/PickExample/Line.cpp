#include "Line.h"

#include <QOpenGLFunctions>

Line::Line() {
    float d[] = {
        // 前面
        - 0.5f, + 0.5f, + 0.5f,
        + 0.5f, + 0.5f, + 0.5f,

        - 0.5f, - 0.5f, + 0.5f,
        + 0.5f, - 0.5f, + 0.5f,

        - 0.5f, + 0.5f, + 0.5f,
        - 0.5f, - 0.5f, + 0.5f,

        + 0.5f, + 0.5f, + 0.5f,
        + 0.5f, - 0.5f, + 0.5f,

        // 后面
        - 0.5f, + 0.5f, - 0.5f,
        + 0.5f, + 0.5f, - 0.5f,

        - 0.5f, - 0.5f, - 0.5f,
        + 0.5f, - 0.5f, - 0.5f,

        - 0.5f, + 0.5f, - 0.5f,
        - 0.5f, - 0.5f, - 0.5f,

        + 0.5f, + 0.5f, - 0.5f,
        + 0.5f, - 0.5f, - 0.5f,

        // 左面
        - 0.5f, + 0.5f, + 0.5f,
        - 0.5f, + 0.5f, - 0.5f,

        - 0.5f, - 0.5f, + 0.5f,
        - 0.5f, - 0.5f, - 0.5f,

        // 右面
        + 0.5f, + 0.5f, + 0.5f,
        + 0.5f, + 0.5f, - 0.5f,

        + 0.5f, - 0.5f, + 0.5f,
        + 0.5f, - 0.5f, - 0.5f,
    };

    vbo.setUsagePattern(QOpenGLBuffer::DynamicDraw);
    vao.create();
    vbo.create();
    vao.bind();
    vbo.bind();

    // 0 -> 12 * 2 * 3 为单位1 立方体;
    // 下面 会频繁写入
    // 12 * 2 * 3 -> 24 * 2 * 3 为平截头体
    // 12 * 2 * 3 -> 16 * 2 * 3 可作为矩形
    // 12 * 2 * 3 -> 13 * 2 * 3 可作为射线;
    vbo.allocate(144 * sizeof(float));
    vbo.write(0, d, sizeof(d));
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    f->glEnableVertexAttribArray(0);
    f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, 3 * sizeof(float), 0);

}

Line::~Line()
{
    vao.destroy();
    vbo.destroy();
}

void Line::setRay(void *data)
{
    vbo.bind();
    vbo.write(72 * sizeof(float), data, 6 * sizeof(float));
}

void Line::setRect(void *data)
{
    vbo.bind();
    vbo.write(72 * sizeof(float), data, 24 * sizeof(float));
}

void Line::setFrustum(void *data)
{
    vbo.bind();
    vbo.write(72 * sizeof(float), data, 72 * sizeof(float));
}

void Line::drawRay()
{
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    f->glDrawArrays(GL_LINES, 24, 2);
}

void Line::drawRect()
{
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    f->glDrawArrays(GL_LINES, 24, 8);
}

void Line::drawFrustum()
{
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    f->glDrawArrays(GL_LINES, 24, 24);
}

void Line::drawCube()
{
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    f->glDrawArrays(GL_LINES, 0, 24);
}
