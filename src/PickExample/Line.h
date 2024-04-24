#ifndef LINE_H
#define LINE_H

#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>

class Line
{
public:
    Line();
    ~Line();
    void setRay(void *data);
    void setRect(void *data);
    void setFrustum(void *data);

    void drawRay();
    void drawRect();
    void drawFrustum();
    void drawCube();

    inline void bind(){vao.bind();}

public:
    QOpenGLVertexArrayObject vao;
    QOpenGLBuffer vbo;
};

#endif // LINE_H
