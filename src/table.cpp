#include "../include/table.h"

#include "../include/world.h"
#include "../include/colors.h"

dbasic::ModelAsset *c_adv::Table::m_tableAsset = nullptr;

c_adv::Table::Table() {
    /* void */
}

c_adv::Table::~Table() {
    /* void */
}

void c_adv::Table::initialize() {
    GameObject::initialize();

    RigidBody.SetHint(dphysics::RigidBody::RigidBodyHint::Dynamic);
    RigidBody.SetInverseMass(0.0f);

    dphysics::CollisionObject *bounds;
    RigidBody.CollisionGeometry.NewBoxObject(&bounds);
    bounds->SetMode(dphysics::CollisionObject::Mode::Fine);
    bounds->GetAsBox()->HalfHeight = 0.14f / 2;
    bounds->GetAsBox()->HalfWidth = 3.0f / 2;
    bounds->GetAsBox()->Orientation = ysMath::Constants::QuatIdentity;
    bounds->GetAsBox()->Position = ysMath::Constants::Zero;
}

void c_adv::Table::render() {
    m_world->getShaders().ResetBrdfParameters();
    m_world->getShaders().SetBaseColor(ObjectColor);

    m_world->getShaders().SetObjectTransform(RigidBody.Transform.GetWorldTransform());
    m_world->getShaders().ConfigureModel(1.0f);
    m_world->getEngine().DrawModel(m_world->getShaders().GetRegularFlags(), m_tableAsset, nullptr);
}

void c_adv::Table::configureAssets(dbasic::AssetManager *am) {
    m_tableAsset = am->GetModelAsset("Table");
}

