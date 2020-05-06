#include "../include/counter.h"

#include "../include/world.h"

dbasic::ModelAsset *c_adv::Counter::m_counterAsset = nullptr;

c_adv::Counter::Counter() {
    /* void */
}

c_adv::Counter::~Counter() {
    /* void */
}

void c_adv::Counter::initialize() {
    GameObject::initialize();

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

void c_adv::Counter::render() {
    m_world->getEngine().SetObjectTransform(RigidBody.Transform.GetWorldTransform());
    m_world->getEngine().SetMultiplyColor(ysVector4(0x84 / 255.0f, 0x3b / 255.0f, 0x62 / 255.0f, 1.0f));
    m_world->getEngine().DrawModel(m_counterAsset, RigidBody.Transform.GetWorldTransform(), 1.0f, nullptr);
}

void c_adv::Counter::process() {
    GameObject::process();
}

void c_adv::Counter::configureAssets(dbasic::AssetManager *am) {
    m_counterAsset = am->GetModelAsset("Counter_1");
}
