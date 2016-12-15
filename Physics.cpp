
#include "Physics.h"

Physics::Physics()
{
    collisionConfiguration = new btDefaultCollisionConfiguration();
    broadphase = new btDbvtBroadphase();
    dispatcher = new btCollisionDispatcher(collisionConfiguration);
    btGImpactCollisionAlgorithm::registerAlgorithm(dispatcher);
    solver = new btSequentialImpulseConstraintSolver();
    dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
    dynamicsWorld->setGravity(btVector3(0, -9.81f, 0));
}

void Physics::update(GLfloat deltaTime)
{

    dynamicsWorld->stepSimulation(deltaTime,10);
    btTransform trans;
    for(int i =0; i<physicsObjects.size(); i++)
    {
        trans.setIdentity();
        if(physicsObjects[i].dynamic)
            physicsObjects[i].object->getMotionState()->getWorldTransform(trans);
        physicsObjects[i].model->Transform(trans);
    }


}

Physics::~Physics()
{
    for (PhysicsObject p : physicsObjects) {
        dynamicsWorld->removeRigidBody(p.object);
        delete p.object;
        delete p.motionState;
        delete p.collisionShape;

        if (!p.dynamic) {
            delete p.triangleMeshShape;
            delete p.triangleMesh;
        } else {
            delete p.hullOptimizer;
            delete p.unoptimizedHull;
            delete p.hull;
        }
    }

    delete dynamicsWorld;
    delete solver;
    delete dispatcher;
    delete collisionConfiguration;
    delete broadphase;
}

void Physics::add(GraphicsObject *model, GLfloat mass, glm::vec3 initialPosition)
{
    PhysicsObject physicsObject;
    if(mass != 0)
        loadDynamicCollisionShape(&physicsObject, *(model->getModel()));
    else
        loadStaticCollisionShape(&physicsObject, *(model->getModel()));

    physicsObject.motionState = new btDefaultMotionState(btTransform(
            btQuaternion(0, 0, 0, 1),
            btVector3(initialPosition.x,
                      initialPosition.y,
                      initialPosition.z)));
    physicsObject.mass = (btScalar)mass;
    physicsObject.inertia = btVector3(0, 0, 0);
    physicsObject.collisionShape->calculateLocalInertia(physicsObject.mass, physicsObject.inertia);
    btRigidBody::btRigidBodyConstructionInfo rigidBodyConstructionInfo(physicsObject.mass,
                                                               physicsObject.motionState,
                                                               physicsObject.collisionShape,
                                                               physicsObject.inertia);
    physicsObject.object = new btRigidBody(rigidBodyConstructionInfo);
    dynamicsWorld->addRigidBody(physicsObject.object);
    physicsObject.model = model;
    physicsObjects.push_back(physicsObject);
}

void Physics::loadStaticCollisionShape(PhysicsObject *physicsObject, const Model &model)
{
    physicsObject->dynamic = false;
    physicsObject->unoptimizedHull = nullptr;
    physicsObject->hullOptimizer = nullptr;
    physicsObject->hull = nullptr;

    for (int i = 0; i < model.meshes.size(); i++)
    {
        physicsObject->triangleMesh = new btTriangleMesh;
        for (unsigned j = 0; j < model.meshes[i].indices.size(); j += 3) {
            btVector3 vertex_1{
                    model.meshes[i].vertices[model.meshes[i].indices[i]].Position.x,
                    model.meshes[i].vertices[model.meshes[i].indices[i]].Position.y,
                    model.meshes[i].vertices[model.meshes[i].indices[i]].Position.z};
            btVector3 vertex_2{
                    model.meshes[i].vertices[model.meshes[i].indices[i + 1]].Position.x,
                    model.meshes[i].vertices[model.meshes[i].indices[i + 1]].Position.y,
                    model.meshes[i].vertices[model.meshes[i].indices[i + 1]].Position.z};
            btVector3 vertex_3{
                    model.meshes[i].vertices[model.meshes[i].indices[i + 2]].Position.x,
                    model.meshes[i].vertices[model.meshes[i].indices[i + 2]].Position.y,
                    model.meshes[i].vertices[model.meshes[i].indices[i + 2]].Position.z};
            physicsObject->triangleMesh->addTriangle(vertex_1, vertex_2, vertex_3);
        }
        physicsObject->collisionShape = new btBvhTriangleMeshShape{physicsObject->triangleMesh, true};
    }
    //physicsObject->collisionShape = new btBoxShape(btVector3(0,0,0));
}

void Physics::loadDynamicCollisionShape(PhysicsObject* physicsObject, const Model &model)
{
    physicsObject->dynamic = true;
    physicsObject->triangleMesh = nullptr;
    physicsObject->triangleMeshShape = nullptr;

    physicsObject->unoptimizedHull = new btConvexHullShape{};
    for (int j = 0; j < model.meshes.size(); ++j)
    {
        for (GLuint i =0; i<model.meshes[j].indices.size(); i++)
        {
            btVector3 vertex{model.meshes[j].vertices[i].Position.x,
                             model.meshes[j].vertices[i].Position.y, model.meshes[j].vertices[i].Position.z};
            physicsObject->unoptimizedHull->addPoint(vertex);
        }

        physicsObject->hullOptimizer = new btShapeHull{physicsObject->unoptimizedHull};
        btScalar margin{physicsObject->unoptimizedHull->getMargin()};
        physicsObject->hullOptimizer->buildHull(margin);
        physicsObject->hull = new btConvexHullShape{
                (btScalar *) physicsObject->hullOptimizer->getVertexPointer(),
                physicsObject->hullOptimizer->numVertices()};

        physicsObject->collisionShape = new btConvexHullShape{*physicsObject->hull};
    }
    //physicsObject->collisionShape = new btCapsuleShape(0,10);

}
