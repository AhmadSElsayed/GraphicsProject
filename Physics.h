#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>
#include <BulletCollision/BroadphaseCollision/btBroadphaseInterface.h>
#include <BulletCollision/BroadphaseCollision/btDbvtBroadphase.h>
#include <BulletCollision/CollisionDispatch/btDefaultCollisionConfiguration.h>

#include <BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h>
#include <BulletCollision/CollisionShapes/btShapeHull.h>

#include <vector>
#include "GraphicsObject.h"

struct PhysicsObject
{
    bool dynamic;

    btConvexHullShape* unoptimizedHull;
    btShapeHull* hullOptimizer;
    btConvexHullShape* hull;

    btTriangleMesh* triangleMesh;
    btBvhTriangleMeshShape* triangleMeshShape;

    btCollisionShape* collisionShape;
    btDefaultMotionState* motionState;
    btScalar mass;
    btVector3 inertia;
    btRigidBody* object;
    GraphicsObject* model;
};

class Physics
{
public:
    std::vector<PhysicsObject> physicsObjects;


private:
    btDefaultCollisionConfiguration* collisionConfiguration;
    btBroadphaseInterface* broadphase;
    btCollisionDispatcher* dispatcher;
    btSequentialImpulseConstraintSolver* solver;
    btDiscreteDynamicsWorld* dynamicsWorld;


    void loadStaticCollisionShape(PhysicsObject* physicsObject, const Model &model);
    void loadDynamicCollisionShape(PhysicsObject* physicsObject, const Model &model);
public:

    Physics();
    void update(int deltaTime);
    void add(GraphicsObject* model, GLfloat mass, glm::vec3 initialPosition);
    ~Physics();
};
