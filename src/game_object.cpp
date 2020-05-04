#include "../include/game_object.h"

#include "../include/world.h"

#include <float.h>

c_adv::GameObject::GameObject() {
    m_world = nullptr;
    m_deletionFlag = false;

    m_beingCarried = false;

    m_tags = std::vector<bool>((int)Tag::Count, false);
    m_realmRecordIndex = -1;

    m_realm = nullptr;
    m_newRealm = nullptr;
    m_changeRealm = false;
    m_lastPortal = nullptr;
    m_graceMode = false;
    m_real = false;

    m_referenceCount = 0;

    m_visualBounds.maxPoint = ysMath::LoadVector(FLT_MAX, FLT_MAX, 0.0f, 1.0f);
    m_visualBounds.minPoint = ysMath::LoadVector(FLT_MIN, FLT_MIN, 0.0f, 1.0f);
}

c_adv::GameObject::~GameObject() {
    /* void */
}

void c_adv::GameObject::process() {
    if (inGraceMode() && !colliding()) {
        setGraceMode(false);
    }
}

void c_adv::GameObject::onCarry() {
    /* void */
}

void c_adv::GameObject::onDrop() {
    /* void */
}

c_adv::GameObject *c_adv::GameObject::getCollidingObject(dphysics::Collision *collision) {
    dphysics::RigidBody *body = collision->m_body1 == &RigidBody
        ? collision->m_body2
        : collision->m_body1;

    return reinterpret_cast<c_adv::GameObject *>(body->GetOwner());
}

bool c_adv::GameObject::colliding() {
    int collisionCount = RigidBody.GetCollisionCount();
    for (int i = 0; i < collisionCount; ++i) {
        dphysics::Collision *col = RigidBody.GetCollision(i);

        if (col->m_sensor) continue;
        else return true;
    }

    return false;
}

bool c_adv::GameObject::colliding(GameObject *object) {
    int collisionCount = RigidBody.GetCollisionCount();
    for (int i = 0; i < collisionCount; ++i) {
        dphysics::Collision *col = RigidBody.GetCollision(i);
        if (col->m_sensor) continue;
        else if (getCollidingObject(col) == object) return true;
    }

    return false;
}

void c_adv::GameObject::addVisualBound(const AABB &bound) {
    m_visualBounds.minPoint = ysMath::ComponentMax(bound.minPoint, m_visualBounds.minPoint);
    m_visualBounds.maxPoint = ysMath::ComponentMin(bound.maxPoint, m_visualBounds.maxPoint);
}

void c_adv::GameObject::createVisualBounds() {
    m_visualBounds.maxPoint = ysMath::LoadVector(FLT_MAX, FLT_MAX, 0.0f, 1.0f);
    m_visualBounds.minPoint = ysMath::LoadVector(-FLT_MAX, -FLT_MAX, 0.0f, 1.0f);

    int objects = RigidBody.CollisionGeometry.GetNumObjects();
    for (int i = 0; i < objects; ++i) {
        dphysics::CollisionObject *object = RigidBody.CollisionGeometry.GetCollisionObject(i);
        ysVector minPoint, maxPoint;
        object->GetBounds(minPoint, maxPoint);
        addVisualBound({ minPoint, maxPoint });
    }
}

void c_adv::GameObject::setGraceMode(bool graceMode) {
    m_graceMode = graceMode;
    RigidBody.SetGhost(graceMode);
}

void c_adv::GameObject::initialize() {
    RigidBody.SetOwner((void *)this);
    m_real = true;
}

void c_adv::GameObject::destroy() {
    /* void */
}

void c_adv::GameObject::render() {
    /* void */
}
