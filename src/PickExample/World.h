#ifndef WORLD_H
#define WORLD_H

#include "btBulletDynamicsCommon.h"

#include <QMatrix4x4>

class MyContactResultCallback : public btCollisionWorld::ContactResultCallback
{


public:
    MyContactResultCallback();
    ~MyContactResultCallback();
        // ContactResultCallback interface
    btScalar addSingleResult(btManifoldPoint &cp, const btCollisionObjectWrapper *colObj0Wrap, int partId0, int index0, const btCollisionObjectWrapper *colObj1Wrap, int partId1, int index1) override;
public:
    QVector<btCollisionObject*> collisionObjects;
};

class World
{
public:
    struct CollisionMatrix {
        QMatrix4x4 aabb;
        QMatrix4x4 obb;
    };

    World();
    ~World();

    void clearRigidBody();

    void addBoxRigidBody(const QVector3D &pos, const QVector3D &boxHalfExtents, const QQuaternion &qua = QQuaternion(), void *userPointer = nullptr, int userIndex = 0);
    void addRigidBody(btRigidBody *obj);
    void removeRigidBody(btRigidBody *obj);
    void takeRigidBody(btRigidBody *obj);
    QVector<CollisionMatrix> getAllCollisionAabbAndObb(int collisionFilterGrou = btBroadphaseProxy::AllFilter);
    const btCollisionObject* getRayTestObject(const QVector3D &lightPos, const QVector3D &direction) const;
    QVector<btCollisionObject*> getCollisionTestObjects(const QVector3D* points, int numPoints);
    QVector<btCollisionObject*> getCollisionTestObjects(const QVector3D &pos, const QVector3D &boxHalfExtents, const QQuaternion &qua);
    inline int getNumCollisionObjects() const {return m_dynamicsWorld->getNumCollisionObjects();}

public:
    btDefaultCollisionConfiguration* m_collisionConfiguration;
    btCollisionDispatcher* m_dispatcher;
    btBroadphaseInterface* m_overlappingPairCache;
    btSequentialImpulseConstraintSolver* m_solver;
    btDiscreteDynamicsWorld* m_dynamicsWorld;

};

#endif // WORLD_H
