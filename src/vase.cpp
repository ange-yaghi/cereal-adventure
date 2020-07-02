#include "../include/vase.h"

#include "../include/world.h"
#include "../include/colors.h"

dbasic::ModelAsset *c_adv::Vase::m_vaseAsset = nullptr;

c_adv::Vase::Vase() {
    /* void */
}

c_adv::Vase::~Vase() {
    /* void */
}

void c_adv::Vase::initialize() {
    GameObject::initialize();

    RigidBody.SetHint(dphysics::RigidBody::RigidBodyHint::Dynamic);
    RigidBody.SetInverseMass(1 / 1.0f);
    RigidBody.SetInverseInertiaTensor(RigidBody.GetRectangleTensor(2.24f, 0.963f));
    RigidBody.SetRequestsInformation(true);

    dphysics::CollisionObject *bounds;
    RigidBody.CollisionGeometry.NewBoxObject(&bounds);
    bounds->SetMode(dphysics::CollisionObject::Mode::Fine);
    bounds->GetAsBox()->HalfHeight = 2.24f / 2;
    bounds->GetAsBox()->HalfWidth = 0.963f / 2;
}

void c_adv::Vase::render() {
    m_world->getEngine().ResetBrdfParameters();
    m_world->getEngine().SetBaseColor(CyberYellow);

    m_world->getEngine().SetObjectTransform(RigidBody.Transform.GetWorldTransform());
    m_world->getEngine().DrawModel(m_vaseAsset, 1.0f, nullptr);
}

void c_adv::Vase::process(float dt) {
    GameObject::process(dt);

    RigidBody.ClearAccumulators();
    RigidBody.AddForceWorldSpace(
        ysMath::LoadVector(0.0f, -15.0f / RigidBody.GetInverseMass(), 0.0f),
        RigidBody.Transform.GetWorldPosition());
}

void c_adv::Vase::configureAssets(dbasic::AssetManager *am) {
    m_vaseAsset = am->GetModelAsset("Vase");
}
