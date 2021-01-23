#include "../include/oven.h"

#include "../include/world.h"
#include "../include/colors.h"

dbasic::ModelAsset *c_adv::Oven::m_ovenAsset = nullptr;

c_adv::Oven::Oven() {
    /* void */
}

c_adv::Oven::~Oven() {
    /* void */
}

void c_adv::Oven::initialize() {
    GameObject::initialize();

    addTag(GameObject::Tag::Oven);

    RigidBody.SetHint(dphysics::RigidBody::RigidBodyHint::Dynamic);
    RigidBody.SetInverseMass(0.0f);

    dphysics::CollisionObject *bounds;
    RigidBody.CollisionGeometry.NewBoxObject(&bounds);
    bounds->SetMode(dphysics::CollisionObject::Mode::Fine);
    bounds->GetAsBox()->HalfHeight = 1.0f;
    bounds->GetAsBox()->HalfWidth = 1.0f;
    bounds->GetAsBox()->Orientation = ysMath::Constants::QuatIdentity;
    bounds->GetAsBox()->Position = ysMath::Constants::Zero;

    m_clock.setLowTime(2.0f);
    m_clock.setHighTime(2.0f);
}

void c_adv::Oven::render() {
    GameObject::render();

    m_world->getShaders().ResetBrdfParameters();

    if (isHot()) {
        m_world->getShaders().SetBaseColor(DebugRed);
    }
    else {
        m_world->getShaders().SetBaseColor(ObjectColor);
    }

    m_world->getShaders().SetObjectTransform(RigidBody.Transform.GetWorldTransform());
    m_world->getShaders().ConfigureModel(1.0f, m_ovenAsset);
    m_world->getEngine().DrawModel(m_world->getShaders().GetRegularFlags(), m_ovenAsset);
}

void c_adv::Oven::process(float dt) {
    GameObject::process(dt);

    m_clock.update(dt);
}

bool c_adv::Oven::isHot() {
    return m_clock.getState();
}

bool c_adv::Oven::isDangerous(const ysVector &p_world) {
    ysVector local = RigidBody.Transform.WorldToLocalSpace(p_world);

    return ysMath::GetY(local) > HalfHeight - 0.1f;
}

void c_adv::Oven::configureAssets(dbasic::AssetManager *am) {
    m_ovenAsset = am->GetModelAsset("Oven");
}

