#include "../include/microwave.h"

#include "../include/world.h"
#include "../include/colors.h"

dbasic::ModelAsset *c_adv::Microwave::m_microwaveAsset = nullptr;

c_adv::Microwave::Microwave() {
    /* void */
}

c_adv::Microwave::~Microwave() {
    /* void */
}

void c_adv::Microwave::initialize() {
    GameObject::initialize();

    RigidBody.SetHint(dphysics::RigidBody::RigidBodyHint::Dynamic);
    RigidBody.SetInverseMass(0.0f);

    dphysics::CollisionObject *bounds;
    RigidBody.CollisionGeometry.NewBoxObject(&bounds);
    bounds->SetMode(dphysics::CollisionObject::Mode::Fine);
    bounds->GetAsBox()->HalfHeight = 0.6f;
    bounds->GetAsBox()->HalfWidth = 1.0f;
    bounds->GetAsBox()->Orientation = ysMath::Constants::QuatIdentity;
    bounds->GetAsBox()->Position = ysMath::Constants::Zero;
}

void c_adv::Microwave::render() {
    m_world->getEngine().ResetBrdfParameters();
    m_world->getEngine().SetBaseColor(CyberYellow);

    m_world->getEngine().SetObjectTransform(RigidBody.Transform.GetWorldTransform());
    m_world->getEngine().DrawModel(m_microwaveAsset, 1.0f, nullptr);
}

void c_adv::Microwave::process() {
    GameObject::process();
}

void c_adv::Microwave::configureAssets(dbasic::AssetManager *am) {
    m_microwaveAsset = am->GetModelAsset("Microwave");
}
