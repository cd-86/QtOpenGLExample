#ifndef CAMERA_H
#define CAMERA_H

#include <QVector3D>
#include <QMatrix4x4>
#include <QtMath>


class Camera
{
public:
    QVector3D Position;
    QVector3D Front;
    QVector3D Up;
    QVector3D Right;
    QVector3D WorldUp;
    float Yaw {0};
    float Pitch {45.0f};
    float MouseSensitivity {0.1f};
    float Zoom {50.0f};

    Camera() : WorldUp(QVector3D(0.0f, 1.0f, 0.0f))
    {
        updateCameraVectors();
    }

    QMatrix4x4 GetViewMatrix()
    {
        QMatrix4x4 mat;
        mat.lookAt(Position, Position + Front, Up);
        return mat;
    }

    void ProcessMouseMovement(float xoffset, float yoffset)
    {
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        Yaw   += xoffset;
        Pitch += yoffset;

        if (Pitch > 89.0f)
            Pitch = 89.0f;
        if (Pitch < -89.0f)
            Pitch = -89.0f;

        updateCameraVectors();
    }

    void ProcessMouseScroll(float yoffset)
    {
        Zoom *= (float)yoffset;
        if (Zoom < 0.001f)
            Zoom = 0.001f;
        if (Zoom > 1000.0f)
            Zoom = 1000.0f;
        updateCameraVectors();
    }

private:
    void updateCameraVectors()
    {
        float yaw = qDegreesToRadians(Yaw);
        float pitch = qDegreesToRadians(Pitch);
        Position.setX(Zoom * qSin(yaw) * qCos(pitch));
        Position.setY(Zoom * qSin(pitch));
        Position.setZ(Zoom * qCos(yaw) * qCos(pitch));

        Front = -Position.normalized();
        Right = QVector3D::crossProduct(Front, WorldUp).normalized();
        Up = QVector3D::crossProduct(Right, Front).normalized();
    }
};

#endif // CAMERA_H
