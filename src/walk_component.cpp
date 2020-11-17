#include "../include/walk_component.h"

#include "../include/game_object.h"

c_adv::WalkComponent::WalkComponent() {
    m_currentSurface = nullptr;
    m_object = nullptr;
    m_groundDebounceTimer = FLT_MAX;

    m_walkingRight = false;
    m_walkingLeft = false;
    m_acceleration = 9.0f;
    m_maxRunVelocity = 5.0f;
    m_startVelocity = 1.0f;
    m_runVelocity = 0.0f;
    m_contactPoint = ysMath::Constants::Zero;
}

c_adv::WalkComponent::~WalkComponent() {
    /* void */
}

void c_adv::WalkComponent::initialize(GameObject *object) {
    m_object = object;
}

void c_adv::WalkComponent::process(float dt) {
    dphysics::RigidBody &rigidBody = m_object->RigidBody;

    checkSurfaceExists();

    bool groundCollision = false;
    const int collisionCount = rigidBody.GetCollisionCount();

    const ysVector velocity = rigidBody.GetVelocity();

    for (int i = 0; i < collisionCount; ++i) {
        dphysics::Collision *col = rigidBody.GetCollision(i);
        GameObject *object = m_object->getCollidingObject(col);

        if (m_object->getCollidingObject(col)->hasTag(GameObject::Tag::Ledge)) continue;

        if (!col->m_sensor && !col->IsGhost()) {
            const ysVector normal = (col->m_body1 == &rigidBody) ? col->m_normal : ysMath::Negate(col->m_normal);
            if (ysMath::GetScalar(ysMath::Dot(normal, ysMath::Constants::YAxis)) > 0.5f) {
                groundCollision = true;
                m_currentSurface = object;
                m_contactPoint = col->m_position;
                break;
            }
        }
    }

    if (groundCollision) {
        m_groundDebounceTimer = 0.0f;
    }
    else if (m_groundDebounceTimer != FLT_MAX) {
        m_groundDebounceTimer += dt;
    }

    if (!isOnSurface()) {
        m_currentSurface = nullptr;
    }

    const float velocity_x = ysMath::GetX(velocity);
    float velocity0 = velocity_x;
    float velocity1 = velocity0;
    if (m_walkingRight) {
        velocity0 = m_runVelocity;
        m_runVelocity += m_acceleration * dt;
        if (m_runVelocity > m_maxRunVelocity) m_runVelocity = m_maxRunVelocity;      
    }
    else if (m_walkingLeft) {
        velocity0 = m_runVelocity;
        m_runVelocity -= m_acceleration * dt;
        if (m_runVelocity < -m_maxRunVelocity) m_runVelocity = -m_maxRunVelocity;
    }
    else {
        m_runVelocity = velocity_x;
    }

    float forceAppliedToSurface = 0.0f;

    if (isOnSurface()) {
        rigidBody.AddImpulseLocalSpace(
            ysMath::LoadVector(m_runVelocity - velocity_x, 0.0f, 0.0f), ysMath::Constants::Zero);

        if (m_walkingLeft || m_walkingRight) {
            velocity1 = m_runVelocity;
        }
        else {
            if (std::abs(velocity0) > 0.01f) {
                forceAppliedToSurface = velocity0 * 10.0f;
                rigidBody.AddForceWorldSpace(
                    ysMath::LoadVector(-forceAppliedToSurface, 0.0f, 0.0f),
                    rigidBody.Transform.GetWorldPosition());
                velocity1 = m_runVelocity;
            }
            else {
                rigidBody.SetVelocity(ysMath::LoadVector(0.0f, ysMath::GetY(velocity), 0.0f));
                velocity1 = 0.0f;
            }
        }
    }
    else {
        m_runVelocity = velocity_x;
    }

    const float effectiveAcceleration = (velocity1 - velocity0) / dt;
    const float impulseAppliedToSurface = -(effectiveAcceleration / rigidBody.GetInverseMass()) * dt;

    if (m_currentSurface != nullptr) {
        m_currentSurface->RigidBody.AddForceWorldSpace(
            ysMath::LoadVector(forceAppliedToSurface, 0.0f, 0.0f),
            m_contactPoint
        );

        m_currentSurface->RigidBody.AddImpulseWorldSpace(
            ysMath::LoadVector(impulseAppliedToSurface, 0.0f, 0.0f),
            m_contactPoint
        );
    }
}

bool c_adv::WalkComponent::isOnSurface() {
    constexpr float VelocityThreshold = 1.0f;
    constexpr float DebouncePeriod = 1 / 10.0f;

    if (std::abs(ysMath::GetY(m_object->RigidBody.GetVelocity())) > VelocityThreshold) {
        return false;
    }

    return (m_groundDebounceTimer < DebouncePeriod);
}

void c_adv::WalkComponent::checkSurfaceExists() {
    if (m_currentSurface != nullptr && m_currentSurface->isDead()) {
        changeSurface(nullptr);
    }
}

void c_adv::WalkComponent::changeSurface(GameObject *surface) {
    if (m_currentSurface != nullptr) m_currentSurface->decrementReferenceCount();
    
    m_currentSurface = surface;
    if (m_currentSurface == nullptr) return;
    else m_currentSurface->incrementReferenceCount();
}
