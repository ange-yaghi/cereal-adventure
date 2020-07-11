#include "../include/walk_component.h"

#include "../include/game_object.h"

c_adv::WalkComponent::WalkComponent() {
    m_currentSurface = nullptr;
    m_object = nullptr;
    m_groundDebounceTimer = FLT_MAX;

    m_walkingRight = false;
    m_walkingLeft = false;
    m_acceleration = 20.0f;
    m_maxRunVelocity = 4.0f;
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

    bool groundCollision = false;
    int collisionCount = rigidBody.GetCollisionCount();

    ysVector v = rigidBody.GetVelocity();

    for (int i = 0; i < collisionCount; ++i) {
        dphysics::Collision *col = rigidBody.GetCollision(i);
        GameObject *object = m_object->getCollidingObject(col);

        if (m_object->getCollidingObject(col)->hasTag(GameObject::Tag::Ledge)) continue;

        if (!col->m_sensor && !col->IsGhost()) {
            ysVector normal = (col->m_body1 == &rigidBody) ? col->m_normal : ysMath::Negate(col->m_normal);
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

    float forceAppliedToSurface = 0.0f;

    if (m_walkingRight) {
        m_runVelocity += m_acceleration * dt;
        if (m_runVelocity > m_maxRunVelocity) m_runVelocity = m_maxRunVelocity;

        forceAppliedToSurface = -m_acceleration / rigidBody.GetInverseMass();
    }
    else if (m_walkingLeft) {
        m_runVelocity -= m_acceleration * dt;
        if (m_runVelocity < -m_maxRunVelocity) m_runVelocity = -m_maxRunVelocity;

        forceAppliedToSurface = m_acceleration / rigidBody.GetInverseMass();
    }

    if (m_walkingLeft || m_walkingRight) {
        rigidBody.SetVelocity(ysMath::LoadVector(m_runVelocity, ysMath::GetY(v), 0.0f));
    }
    else if (isOnSurface()) {
        m_runVelocity = 0.0f;
        if (std::abs(ysMath::GetX(v)) > 0.01f) {
            forceAppliedToSurface = ysMath::GetX(v) * 10.0f;
            rigidBody.AddForceWorldSpace(
                ysMath::LoadVector(-forceAppliedToSurface, 0.0f, 0.0f), 
                rigidBody.Transform.GetWorldPosition());
        }
        else {
            rigidBody.SetVelocity(ysMath::LoadVector(0.0f, ysMath::GetY(v), 0.0f));
        }
    }

    if (m_currentSurface != nullptr) {
        m_currentSurface->RigidBody.AddForceWorldSpace(
            ysMath::LoadVector(forceAppliedToSurface, 0.0f, 0.0f),
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
