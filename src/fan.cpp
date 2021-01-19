#include "../include/fan.h"

#include "../include/world.h"
#include "../include/colors.h"

dbasic::ModelAsset *c_adv::Fan::m_fanAsset = nullptr;

c_adv::Fan::Fan() {
    /* void */
}

c_adv::Fan::~Fan() {
    /* void */
}

void c_adv::Fan::initialize() {
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
    bounds->SetRelativePosition(ysMath::LoadVector(4.0f, 0.0f, 0.0f));

    RigidBody.CollisionGeometry.NewBoxObject(&bounds);
    bounds->SetMode(dphysics::CollisionObject::Mode::Fine);
    bounds->GetAsBox()->HalfWidth = 2.3f / 2;
    bounds->GetAsBox()->HalfHeight = 3.2f / 2;
}

void c_adv::Fan::render() {
    m_world->getShaders().ResetBrdfParameters();
    m_world->getShaders().SetBaseColor(DebugRed);

    m_world->getShaders().SetObjectTransform(RigidBody.Transform.GetWorldTransform());
    m_world->getShaders().ConfigureModel(1.0f);
    m_world->getEngine().DrawModel(m_world->getShaders().GetRegularFlag(), m_fanAsset, nullptr);
}

void c_adv::Fan::process(float dt) {
    GameObject::process(dt);

    int collisionCount = RigidBody.GetCollisionCount();
    for (int i = 0; i < collisionCount; ++i) {
        dphysics::Collision *col = RigidBody.GetCollision(i);

        if (col->m_sensor) {
            GameObject *obj = getCollidingObject(col);

            const ysVector position = RigidBody.Transform.GetWorldPosition();
            const ysVector objPosition = obj->RigidBody.Transform.GetWorldPosition();

            const float obj_x = ysMath::GetX(objPosition);
            const float obj_y = ysMath::GetY(objPosition);
            const float fan_x = ysMath::GetX(position);
            const float fan_y = ysMath::GetY(position);

            if (std::abs(obj_y - fan_y) < 1.5f && obj_x > fan_x) {
                if (!obj->hasTag(GameObject::Tag::Dynamic)) continue;
                if (ysMath::GetX(obj->RigidBody.GetVelocity()) > 15.0f) continue;

                obj->RigidBody.AddForceLocalSpace(
                    ysMath::LoadVector(20.0f, 10.0f, 0.0f), ysMath::Constants::Zero);
            }
        }
    }
}

void c_adv::Fan::configureAssets(dbasic::AssetManager *am) {
    m_fanAsset = am->GetModelAsset("Fan");
}
