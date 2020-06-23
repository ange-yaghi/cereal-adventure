#include "../include/stove_hood.h"

#include "../include/world.h"
#include "../include/colors.h"

dbasic::ModelAsset *c_adv::StoveHood::m_stoveHoodAsset = nullptr;

c_adv::StoveHood::StoveHood() {
    /* void */
}

c_adv::StoveHood::~StoveHood() {
    /* void */
}

void c_adv::StoveHood::initialize() {
    GameObject::initialize();

    RigidBody.SetHint(dphysics::RigidBody::RigidBodyHint::Dynamic);
    RigidBody.SetInverseMass(0.0f);
    RigidBody.SetAlwaysAwake(true);
    RigidBody.SetRequestsInformation(true);

    dphysics::CollisionObject *bounds;
    RigidBody.CollisionGeometry.NewCircleObject(&bounds);
    bounds->SetMode(dphysics::CollisionObject::Mode::Sensor);
    bounds->GetAsCircle()->Radius = 4.0f;
    bounds->GetAsCircle()->Position = ysMath::Constants::Zero;
    bounds->SetRelativePosition(ysMath::LoadVector(0.0f, -10.0f, 0.0f));
}

void c_adv::StoveHood::render() {
    m_world->getEngine().ResetBrdfParameters();
    m_world->getEngine().SetBaseColor(CyberYellow);

    m_world->getEngine().SetObjectTransform(RigidBody.Transform.GetWorldTransform());
    m_world->getEngine().DrawModel(m_stoveHoodAsset, 1.0f, nullptr);
}

void c_adv::StoveHood::process() {
    GameObject::process();

    int collisionCount = RigidBody.GetCollisionCount();
    for (int i = 0; i < collisionCount; ++i) {
        dphysics::Collision *col = RigidBody.GetCollision(i);

        if (col->m_sensor) {
            if (getCollidingObject(col)->hasTag(GameObject::Tag::Dynamic)) {
                getCollidingObject(col)->RigidBody.AddForceLocalSpace(
                    ysMath::LoadVector(0.0f, 50.0f, 0.0f), ysMath::Constants::Zero);
            }
        }
    }
}

void c_adv::StoveHood::configureAssets(dbasic::AssetManager *am) {
    m_stoveHoodAsset = am->GetModelAsset("StoveHood");
}
