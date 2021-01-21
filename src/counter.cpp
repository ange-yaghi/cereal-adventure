#include "../include/counter.h"

#include "../include/world.h"
#include "../include/colors.h"

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
    RigidBody.SetMaterial(GenericFrictionMaterial);

    dphysics::CollisionObject *bounds;
    RigidBody.CollisionGeometry.NewBoxObject(&bounds);
    bounds->SetMode(dphysics::CollisionObject::Mode::Fine);
    bounds->GetAsBox()->HalfHeight = 1.0f;
    bounds->GetAsBox()->HalfWidth = 1.0f;
    bounds->GetAsBox()->Orientation = ysMath::Constants::QuatIdentity;
    bounds->GetAsBox()->Position = ysMath::Constants::Zero;
}

void c_adv::Counter::render() {
    m_world->getShaders().ResetBrdfParameters();
    m_world->getShaders().SetBaseColor(ObjectColor);

    m_world->getShaders().SetObjectTransform(RigidBody.Transform.GetWorldTransform());
    m_world->getShaders().ConfigureModel(1.0f);
    m_world->getEngine().DrawModel(m_world->getShaders().GetRegularFlags(), m_counterAsset, nullptr);
}

void c_adv::Counter::configureAssets(dbasic::AssetManager *am) {
    m_counterAsset = am->GetModelAsset("Counter_1");
}
