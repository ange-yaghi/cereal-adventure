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
    RigidBody.SetInverseMass(1.0f / 0.8f);
    RigidBody.SetInverseInertiaTensor(RigidBody.GetRectangleTensor(2.24f, 0.963f));
    RigidBody.SetRequestsInformation(true);
    RigidBody.SetMaterial(GenericFrictionMaterial);

    dphysics::CollisionObject *bounds;
    RigidBody.CollisionGeometry.NewBoxObject(&bounds);
    bounds->SetMode(dphysics::CollisionObject::Mode::Fine);
    bounds->GetAsBox()->HalfHeight = 2.24f / 2;
    bounds->GetAsBox()->HalfWidth = 0.963f / 2;

    m_positionDamper.setDampingTensor(ysMath::LoadVector(0.5f, 0.5f, 0.0f));
    m_positionDamper.setStiffnessTensor(ysMath::LoadVector(500.0f, 500.0f, 0.0f));
    m_positionDamper.setPosition(RigidBody.Transform.GetWorldPosition());

    m_rotationDamper.setDampingTensor(ysMath::LoadScalar(0.5f));
    m_rotationDamper.setStiffnessTensor(ysMath::LoadScalar(1000.0f));
    m_rotationDamper.setPosition(RigidBody.Transform.GetWorldOrientation());
}

void c_adv::Vase::render() {
    m_world->getEngine().ResetBrdfParameters();
    m_world->getEngine().SetBaseColor(DebugRed);

    m_world->getEngine().SetObjectTransform(m_renderTransform.GetWorldTransform());
    m_world->getEngine().DrawModel(m_vaseAsset, 1.0f, nullptr);
}

void c_adv::Vase::process(float dt) {
    GameObject::process(dt);

    m_positionDamper.setTarget(RigidBody.Transform.GetWorldPosition());
    m_positionDamper.update(dt);

    m_rotationDamper.setTarget(RigidBody.Transform.GetWorldOrientation());
    m_rotationDamper.update(dt);

    m_renderTransform.SetPosition(m_positionDamper.getPosition());
    m_renderTransform.SetOrientation(m_rotationDamper.getPosition());

    RigidBody.AddForceWorldSpace(
        ysMath::LoadVector(0.0f, -15.0f / RigidBody.GetInverseMass(), 0.0f),
        RigidBody.Transform.GetWorldPosition());
}

void c_adv::Vase::configureAssets(dbasic::AssetManager *am) {
    m_vaseAsset = am->GetModelAsset("Vase");
}
