#include "../include/toaster.h"

#include "../include/world.h"

dbasic::ModelAsset *c_adv::Toaster::m_toasterAsset = nullptr;

c_adv::Toaster::Toaster() {
    /* void */
}

c_adv::Toaster::~Toaster() {
    /* void */
}

void c_adv::Toaster::initialize() {
    GameObject::initialize();

    RigidBody.SetHint(dphysics::RigidBody::RigidBodyHint::Dynamic);
    RigidBody.SetInverseMass(0.0f);

    dphysics::CollisionObject *bounds;
    RigidBody.CollisionGeometry.NewBoxObject(&bounds);
    bounds->SetMode(dphysics::CollisionObject::Mode::Fine);
    bounds->GetAsBox()->HalfHeight = 0.56f / 2;
    bounds->GetAsBox()->HalfWidth = 0.56f / 2;
    bounds->GetAsBox()->Orientation = ysMath::Constants::QuatIdentity;
    bounds->GetAsBox()->Position = ysMath::Constants::Zero;
}

void c_adv::Toaster::render() {
    m_world->getEngine().SetObjectTransform(RigidBody.Transform.GetWorldTransform());
    m_world->getEngine().SetMultiplyColor(ysVector4(0.1f, 0.1f, 0.1f, 1.0f));
    m_world->getEngine().DrawModel(m_toasterAsset, RigidBody.Transform.GetWorldTransform(), 1.0f, nullptr);
}

void c_adv::Toaster::process() {
    GameObject::process();
}

void c_adv::Toaster::configureAssets(dbasic::AssetManager *am) {
    m_toasterAsset = am->GetModelAsset("Toaster");
}
