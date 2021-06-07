#include "../include/cabinet.h"

#include "../include/colors.h"
#include "../include/world.h"

dbasic::ModelAsset *c_adv::Cabinet::m_cabinetAsset = nullptr;

c_adv::Cabinet::Cabinet() {
    /* void */
}

c_adv::Cabinet::~Cabinet() {
    /* void */
}

void c_adv::Cabinet::initialize() {
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

void c_adv::Cabinet::render() {
    m_world->getShaders().ResetBrdfParameters();
    m_world->getShaders().SetBaseColor(ObjectColor);

    m_world->getShaders().SetObjectTransform(RigidBody.Transform.GetWorldTransform());
    m_world->getShaders().ConfigureModel(1.0f, m_cabinetAsset);
    m_world->getEngine().DrawModel(m_world->getShaders().GetRegularFlags(), m_cabinetAsset);
}

void c_adv::Cabinet::getAssets(dbasic::AssetManager *am) {
    m_cabinetAsset = am->GetModelAsset("Cabinet");
}
