#include "../include/oven.h"

#include "../include/world.h"

dbasic::ModelAsset *c_adv::Oven::m_ovenAsset = nullptr;

c_adv::Oven::Oven() {
    /* void */
}

c_adv::Oven::~Oven() {
    /* void */
}

void c_adv::Oven::initialize() {
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

void c_adv::Oven::render() {
    m_world->getEngine().ResetBrdfParameters();
    m_world->getEngine().SetBaseColor(ysColor::srgbiToLinear(50, 50, 50));

    m_world->getEngine().SetObjectTransform(RigidBody.Transform.GetWorldTransform());
    m_world->getEngine().DrawModel(m_ovenAsset, 1.0f, nullptr);
}

void c_adv::Oven::process() {
    GameObject::process();
}

void c_adv::Oven::configureAssets(dbasic::AssetManager *am) {
    m_ovenAsset = am->GetModelAsset("Oven");
}

