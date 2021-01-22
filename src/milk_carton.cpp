#include "../include/milk_carton.h"

#include "../include/world.h"
#include "../include/colors.h"

dbasic::ModelAsset *c_adv::MilkCarton::m_placeholderAsset = nullptr;

c_adv::MilkCarton::MilkCarton() {
    /* void */
}

c_adv::MilkCarton::~MilkCarton() {
    /* void */
}

void c_adv::MilkCarton::initialize() {
    GameObject::initialize();

    RigidBody.SetHint(dphysics::RigidBody::RigidBodyHint::Dynamic);
    RigidBody.SetInverseMass(1 / 1.0f);
    RigidBody.SetInverseInertiaTensor(RigidBody.GetRectangleTensor(0.443f * 5, 1.53f)); 
    RigidBody.SetRequestsInformation(true);

    dphysics::CollisionObject *bounds;
    RigidBody.CollisionGeometry.NewBoxObject(&bounds);
    bounds->SetMode(dphysics::CollisionObject::Mode::Fine);
    bounds->GetAsBox()->HalfHeight = 1.53f / 2;
    bounds->GetAsBox()->HalfWidth = 0.443f / 2;
}

void c_adv::MilkCarton::render() {
    m_world->getShaders().ResetBrdfParameters();
    m_world->getShaders().SetBaseColor(ObjectColor);

    m_world->getShaders().SetObjectTransform(RigidBody.Transform.GetWorldTransform());
    m_world->getShaders().ConfigureModel(1.0f);
    m_world->getEngine().DrawModel(m_world->getShaders().GetRegularFlags(), m_placeholderAsset);
}

void c_adv::MilkCarton::process(float dt) {
    GameObject::process(dt);

    RigidBody.ClearAccumulators();
    RigidBody.AddForceWorldSpace(
        ysMath::LoadVector(0.0f, -15.0f / RigidBody.GetInverseMass(), 0.0f),
        RigidBody.Transform.GetWorldPosition());
}

void c_adv::MilkCarton::configureAssets(dbasic::AssetManager *am) {
    m_placeholderAsset = am->GetModelAsset("MilkCarton");
}
