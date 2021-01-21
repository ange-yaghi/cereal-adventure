#include "../include/fridge.h"

#include "../include/world.h"
#include "../include/colors.h"

dbasic::ModelAsset *c_adv::Fridge::m_fridgeAsset = nullptr;

c_adv::Fridge::Fridge() {
    /* void */
}

c_adv::Fridge::~Fridge() {
    /* void */
}

void c_adv::Fridge::initialize() {
    GameObject::initialize();

    RigidBody.SetHint(dphysics::RigidBody::RigidBodyHint::Dynamic);
    RigidBody.SetInverseMass(0.0f);

    dphysics::CollisionObject *bounds;
    RigidBody.CollisionGeometry.NewBoxObject(&bounds);
    bounds->SetMode(dphysics::CollisionObject::Mode::Fine);
    bounds->GetAsBox()->HalfHeight = 3.0f;
    bounds->GetAsBox()->HalfWidth = 1.0f;
    bounds->GetAsBox()->Orientation = ysMath::Constants::QuatIdentity;
    bounds->GetAsBox()->Position = ysMath::Constants::Zero;
}

void c_adv::Fridge::render() {
    m_world->getShaders().ResetBrdfParameters();
    m_world->getShaders().SetBaseColor(ObjectColor);

    m_world->getShaders().SetObjectTransform(RigidBody.Transform.GetWorldTransform());
    m_world->getShaders().ConfigureModel(1.0f);
    m_world->getEngine().DrawModel(m_world->getShaders().GetRegularFlags(), m_fridgeAsset, nullptr);
}

void c_adv::Fridge::configureAssets(dbasic::AssetManager *am) {
    m_fridgeAsset = am->GetModelAsset("Fridge");
}
