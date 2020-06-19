#include "../include/stool_1.h"

#include "../include/world.h"

dbasic::ModelAsset *c_adv::Stool_1::m_stoolAsset = nullptr;

c_adv::Stool_1::Stool_1() {
    /* void */
}

c_adv::Stool_1::~Stool_1() {
    /* void */
}

void c_adv::Stool_1::initialize() {
    GameObject::initialize();

    RigidBody.SetHint(dphysics::RigidBody::RigidBodyHint::Dynamic);
    RigidBody.SetInverseMass(0.0f);

    dphysics::CollisionObject *bounds;
    RigidBody.CollisionGeometry.NewBoxObject(&bounds);
    bounds->SetMode(dphysics::CollisionObject::Mode::Fine);
    bounds->GetAsBox()->HalfHeight = 1.7f;
    bounds->GetAsBox()->HalfWidth = 0.3f;
    bounds->GetAsBox()->Orientation = ysMath::Constants::QuatIdentity;
    bounds->GetAsBox()->Position = ysMath::Constants::Zero;
}

void c_adv::Stool_1::render() {
    m_world->getEngine().ResetBrdfParameters();
    m_world->getEngine().SetBaseColor(ysColor::srgbiToLinear(50, 50, 50));

    m_world->getEngine().SetObjectTransform(RigidBody.Transform.GetWorldTransform());
    m_world->getEngine().DrawModel(m_stoolAsset, 1.0f, nullptr);
}

void c_adv::Stool_1::process() {
    GameObject::process();
}

void c_adv::Stool_1::configureAssets(dbasic::AssetManager *am) {
    m_stoolAsset = am->GetModelAsset("Stool_1");
}
