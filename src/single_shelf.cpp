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
    m_world->getShaders().ResetBrdfParameters();
    m_world->getShaders().SetBaseColor(ObjectColor);

    m_world->getShaders().SetObjectTransform(RigidBody.Transform.GetWorldTransform());
    m_world->getShaders().ConfigureModel(1.0f, m_singleShelfAsset);
    m_world->getEngine().DrawModel(m_world->getShaders().GetRegularFlags(), m_singleShelfAsset);
}

void c_adv::SingleShelf::configureAssets(dbasic::AssetManager *am) {
    m_singleShelfAsset = am->GetModelAsset("SingleShelf");
}
