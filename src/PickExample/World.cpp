#include "World.h"

MyContactResultCallback::MyContactResultCallback() : btCollisionWorld::ContactResultCallback()
{

}

MyContactResultCallback::~MyContactResultCallback()
{

}

btScalar MyContactResultCallback::addSingleResult(btManifoldPoint &cp, const btCollisionObjectWrapper *colObj0Wrap, int partId0, int index0, const btCollisionObjectWrapper *colObj1Wrap, int partId1, int index1)
{
    collisionObjects << (btCollisionObject*)colObj1Wrap->getCollisionObject();
    return cp.getDistance();
}

World::World()
{
    m_collisionConfiguration = new btDefaultCollisionConfiguration();
    m_dispatcher = new btCollisionDispatcher(m_collisionConfiguration);
    m_overlappingPairCache = new btDbvtBroadphase();
    m_solver = new btSequentialImpulseConstraintSolver;
    m_dynamicsWorld = new btDiscreteDynamicsWorld(m_dispatcher, m_overlappingPairCache, m_solver, m_collisionConfiguration);
    m_dynamicsWorld->setGravity(btVector3(0, -10, 0));
}

World::~World()
{
    clearRigidBody();
    delete m_dynamicsWorld;
    delete m_solver;
    delete m_overlappingPairCache;
    delete m_dispatcher;
    delete m_collisionConfiguration;
}

void World::clearRigidBody()
{
    for (int i = m_dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--)
    {
        btCollisionObject* obj = m_dynamicsWorld->getCollisionObjectArray()[i];
        btRigidBody* body = btRigidBody::upcast(obj);
        if (body)
        {
            //            if (body->getMotionState())
            delete body->getMotionState();
            //            if (body->getCollisionShape())
            delete body->getCollisionShape();
        }
        m_dynamicsWorld->removeCollisionObject(obj);
        delete obj;
    }
}

void World::addBoxRigidBody(const QVector3D &pos, const QVector3D &boxHalfExtents, const QQuaternion &qua, void *userPointer, int userIndex)
{
    btCollisionShape* boxCollisionShape = new btBoxShape(btVector3(boxHalfExtents.x(), boxHalfExtents.y(), boxHalfExtents.z()));
    btDefaultMotionState* motionstate = new btDefaultMotionState(btTransform(
        btQuaternion(qua.x(), qua.y(), qua.z(), qua.scalar()),
        btVector3(pos.x(), pos.y(), pos.z())
        ));

    btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(0, motionstate, boxCollisionShape); // 质量 0
    btRigidBody *rigidBody = new btRigidBody(rigidBodyCI);
    rigidBody->setUserPointer(userPointer);
    rigidBody->setUserIndex(userIndex);

    m_dynamicsWorld->addRigidBody(rigidBody);
}

void World::addRigidBody(btRigidBody *obj)
{
    m_dynamicsWorld->addRigidBody(obj);
}

void World::removeRigidBody(btRigidBody *obj)
{
    if (!obj)
        return;
    //            if (body->getMotionState())
    delete obj->getMotionState();
    //            if (body->getCollisionShape())
    delete obj->getCollisionShape();
    m_dynamicsWorld->removeRigidBody(obj);
    delete obj;
}

void World::takeRigidBody(btRigidBody *obj)
{
    if (!obj)
        return;
    m_dynamicsWorld->removeRigidBody(obj);
}

QVector<World::CollisionMatrix> World::getAllCollisionAabbAndObb(int collisionFilterGrou)
{
    QVector<CollisionMatrix> cms;
    for (int i = m_dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--)
    {
        btCollisionObject* obj = m_dynamicsWorld->getCollisionObjectArray()[i];
        if (collisionFilterGrou != btBroadphaseProxy::AllFilter && obj->getBroadphaseHandle()->m_collisionFilterGroup != collisionFilterGrou){
            continue;
        }
        CollisionMatrix ms;
        btRigidBody* body = btRigidBody::upcast(obj);
        if (body){
            btVector3 min, max;
            body->getAabb(min, max);
            btVector3 vec = max - min;
            auto vec2 = vec/2.0;
            auto vecF = vec2 + min;
            ms.aabb.translate(QVector3D(vecF.x(),vecF.y(),vecF.z()));
            ms.aabb.scale(QVector3D(vec.x(),vec.y(),vec.z()));

            if (body->getCollisionShape()->getShapeType() == BOX_SHAPE_PROXYTYPE) {
                btTransform tf;
                body->getMotionState()->getWorldTransform(tf);
                tf.getOpenGLMatrix(ms.obb.data());

                btBoxShape* box = (btBoxShape*)body->getCollisionShape();
                vec = box->getHalfExtentsWithMargin();
                auto vecX2 = vec * 2;
                ms.obb.scale(QVector3D(vecX2.x(),vecX2.y(),vecX2.z()));
            }
        }
        cms.append(ms);
    }
    return cms;
}

const btCollisionObject* World::getRayTestObject(const QVector3D &lightPos, const QVector3D &direction) const
{
    btCollisionWorld::ClosestRayResultCallback RayCallback(
        btVector3(lightPos.x(), lightPos.y(), lightPos.z()),
        btVector3(direction.x(), direction.y(), direction.z())
        );
    // 据说可以提升射线检测精度
    //    RayCallback.m_flags |= btTriangleRaycastCallback::kF_UseSubSimplexConvexCastRaytest | btTriangleRaycastCallback::kF_UseGjkConvexCastRaytest;
    m_dynamicsWorld->rayTest(
        btVector3(lightPos.x(), lightPos.y(), lightPos.z()),
        btVector3(direction.x(), direction.y(), direction.z()),
        RayCallback
        );

    if(RayCallback.hasHit()) {
        return RayCallback.m_collisionObject;
    }
    return nullptr;
}

QVector<btCollisionObject*> World::getCollisionTestObjects(const QVector3D* points, int numPoints)
{
    btConvexHullShape collisonShape((float*)points, numPoints, sizeof(QVector3D));
    btCollisionObject collisionObject;
    collisionObject.setCollisionShape(&collisonShape);

    MyContactResultCallback callback;
    m_dynamicsWorld->contactTest(&collisionObject, callback);
    return callback.collisionObjects;
}

QVector<btCollisionObject *> World::getCollisionTestObjects(const QVector3D &pos, const QVector3D &boxHalfExtents, const QQuaternion &qua)
{
    btBoxShape collisonShape(btVector3(boxHalfExtents.x(), boxHalfExtents.y(), boxHalfExtents.z()));
    btCollisionObject collisionObject;
    collisionObject.setCollisionShape(&collisonShape);
    collisionObject.setWorldTransform(btTransform(
        btQuaternion(qua.x(), qua.y(), qua.z(), qua.scalar()),
        btVector3(pos.x(), pos.y(), pos.z())
        ));

    MyContactResultCallback callback;
    m_dynamicsWorld->contactTest(&collisionObject, callback);
    return callback.collisionObjects;
}
