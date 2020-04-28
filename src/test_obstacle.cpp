#include "../include/test_obstacle.h"

#include "../include/world.h"

dbasic::ModelAsset *c_adv::TestObstacle::m_obstacleMesh = nullptr;

c_adv::TestObstacle::TestObstacle() {
    /* void */
}

c_adv::TestObstacle::~TestObstacle() {
    /* void */
}

void c_adv::TestObstacle::initialize() {
    RigidBody.SetHint(dphysics::RigidBody::RigidBodyHint::Dynamic);
    RigidBody.SetInverseMass(0.0f);

    dphysics::CollisionObject *bounds;
    RigidBody.CollisionGeometry.NewBoxObject(&bounds);
    bounds->SetMode(dphysics::CollisionObject::Mode::Fine);
    bounds->GetAsBox()->HalfHeight = 1.0f;
    bounds->GetAsBox()->HalfWidth = 1.0f;
    bounds->GetAsBox()->Orientation = ysMath::Constants::QuatIdentity;
    bounds->GetAsBox()->Position = ysMath::Constants::Zero;
}

void c_adv::TestObstacle::render() {
    const int color[] = { 255, 0, 0 };
    m_world->getEngine().SetObjectTransform(RigidBody.Transform.GetWorldTransform());
    m_world->getEngine().DrawModel(m_obstacleMesh, RigidBody.Transform.GetWorldTransform(), 1.0f, nullptr);
}

void c_adv::TestObstacle::process() {
    GameObject::process();
}

void c_adv::TestObstacle::configureAssets(dbasic::AssetManager *am) {
    m_obstacleMesh = am->GetModelAsset("TestObstacle");
}
