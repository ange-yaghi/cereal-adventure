#include "../include/ledge.h"

#include "../include/world.h"

dbasic::ModelAsset *c_adv::Ledge::m_ledgeAsset = nullptr;

c_adv::Ledge::Ledge() {
    /* bool */
}

c_adv::Ledge::~Ledge() {
    /* bool */
}

void c_adv::Ledge::initialize() {
    GameObject::initialize();

    addTag(Tag::Ledge);

    RigidBody.SetHint(dphysics::RigidBody::RigidBodyHint::Dynamic);
    RigidBody.SetInverseMass(0.0f);
    RigidBody.SetAlwaysAwake(false);
    RigidBody.SetRequestsInformation(false);

    dphysics::CollisionObject *sensor;
    RigidBody.CollisionGeometry.NewCircleObject(&sensor);
    sensor->SetMode(dphysics::CollisionObject::Mode::Sensor);
    sensor->GetAsCircle()->Radius = 1.0f;
}

void c_adv::Ledge::render() {
    m_world->getShaders().ResetBrdfParameters();
    m_world->getShaders().SetBaseColor(ysColor::srgbiToLinear(255, 255, 0));
    m_world->getShaders().SetObjectTransform(RigidBody.Transform.GetWorldTransform());
    m_world->getShaders().SetLit(false);
    m_world->getShaders().ConfigureModel(1.0f);
    m_world->getEngine().DrawModel(m_world->getShaders().GetRegularFlags(), m_ledgeAsset, nullptr, 0);
}

void c_adv::Ledge::configureAssets(dbasic::AssetManager *am) {
    m_ledgeAsset = am->GetModelAsset("LedgeDebug");
}
