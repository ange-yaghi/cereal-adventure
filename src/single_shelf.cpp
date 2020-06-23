#include "../include/single_shelf.h"

#include "../include/world.h"
#include "../include/colors.h"

dbasic::ModelAsset *c_adv::SingleShelf::m_singleShelfAsset = nullptr;

c_adv::SingleShelf::SingleShelf() {
    /* void */
}

c_adv::SingleShelf::~SingleShelf() {
    /* void */
}

void c_adv::SingleShelf::initialize() {
    GameObject::initialize();

    RigidBody.SetHint(dphysics::RigidBody::RigidBodyHint::Dynamic);
    RigidBody.SetInverseMass(0.0f);

    dphysics::CollisionObject *bounds;
    RigidBody.CollisionGeometry.NewBoxObject(&bounds);
    bounds->SetMode(dphysics::CollisionObject::Mode::Fine);
    bounds->GetAsBox()->HalfHeight = 0.1f;
    bounds->GetAsBox()->HalfWidth = 1.0f;
    bounds->GetAsBox()->Orientation = ysMath::Constants::QuatIdentity;
    bounds->GetAsBox()->Position = ysMath::Constants::Zero;
}

void c_adv::SingleShelf::render() {
    m_world->getEngine().ResetBrdfParameters();
    m_world->getEngine().SetBaseColor(CyberYellow);

    m_world->getEngine().SetObjectTransform(RigidBody.Transform.GetWorldTransform());
    m_world->getEngine().DrawModel(m_singleShelfAsset, 1.0f, nullptr);
}

void c_adv::SingleShelf::process() {
    GameObject::process();
}

void c_adv::SingleShelf::configureAssets(dbasic::AssetManager *am) {
    m_singleShelfAsset = am->GetModelAsset("SingleShelf");
}
