#include "../include/test_obstacle.h"

#include "../include/world.h"

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
    RigidBody.CollisionGeometry.NewCircleObject(&bounds);
    bounds->SetMode(dphysics::CollisionObject::Mode::Fine);
    bounds->GetAsCircle()->Radius = 1.0f;
}

void c_adv::TestObstacle::render() {
    const int color[] = { 255, 0, 0 };
    m_world->getEngine().SetObjectTransform(RigidBody.Transform.GetWorldTransform());
    //m_world->getEngine().DrawBox(color, 2.0f, 2.0f);
}

void c_adv::TestObstacle::process() {
    GameObject::process();
}

void c_adv::TestObstacle::configureAssets(dbasic::AssetManager *am) {
    /* void */
}
