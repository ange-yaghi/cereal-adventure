#include "../include/stove_hood.h"

#include "../include/world.h"
#include "../include/colors.h"

dbasic::ModelAsset *c_adv::StoveHood::m_stoveHoodAsset = nullptr;

c_adv::StoveHood::StoveHood() {
    m_clock.setLowTime(10.0f);
    m_clock.setHighTime(10.0f);

    m_currentPower = 0.0f;
    m_maxPower = 30.0f;
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

    ysVector color = ysMath::Lerp(ObjectColor, DebugRed, m_currentPower / m_maxPower);
    m_world->getEngine().SetBaseColor(color);

    m_world->getEngine().SetObjectTransform(RigidBody.Transform.GetWorldTransform());
    m_world->getEngine().DrawModel(m_stoveHoodAsset, 1.0f, nullptr);
}

void c_adv::StoveHood::process(float dt) {
    GameObject::process(dt);

    m_clock.update(dt);

    if (m_clock.getState()) {
        m_currentPower += dt * 7.0f;
        m_currentPower = min(m_currentPower, m_maxPower);
    }
    else {
        m_currentPower -= dt * 15.0f;
        m_currentPower = max(m_currentPower, 0.0f);
    }

    int collisionCount = RigidBody.GetCollisionCount();
    for (int i = 0; i < collisionCount; ++i) {
        dphysics::Collision *col = RigidBody.GetCollision(i);

        if (col->m_sensor) {
            GameObject *obj = getCollidingObject(col);

            float obj_x = ysMath::GetX(obj->RigidBody.Transform.GetWorldPosition());
            float hood_x = ysMath::GetX(RigidBody.Transform.GetWorldPosition());

            if (std::abs(obj_x - hood_x) < 1.5f) {
                if (obj->hasTag(GameObject::Tag::Dynamic)) {
                    obj->RigidBody.AddForceLocalSpace(
                        ysMath::LoadVector(0.0f, m_currentPower, 0.0f), ysMath::Constants::Zero);
                }
            }
        }
    }
}

void c_adv::StoveHood::configureAssets(dbasic::AssetManager *am) {
    m_stoveHoodAsset = am->GetModelAsset("StoveHood");
}
