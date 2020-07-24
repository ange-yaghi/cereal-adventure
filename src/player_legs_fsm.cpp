#include "../include/player_legs_fsm.h"

#include "../include/player.h"

c_adv::PlayerLegsFsm::PlayerLegsFsm() {
    m_player = nullptr;
    m_currentState = State::Idle;
}

c_adv::PlayerLegsFsm::~PlayerLegsFsm() {
    /* void */
}

void c_adv::PlayerLegsFsm::initialize(Player *player) {
    m_player = player;
}

void c_adv::PlayerLegsFsm::nextState(FsmResults &result) {
    ysVector velocity = m_player->RigidBody.GetVelocity();

    // FSM inputs
    State current = m_currentState;
    bool onSurface = m_player->m_walkComponent.isOnSurface();
    bool isHurt = m_player->isHurt();
    bool isHanging = m_player->isHanging();
    bool isGraspReady = m_player->isGraspReady();
    bool isLaunching = m_player->isLaunching();
    bool isActionComplete = m_player->isCurrentArmActionComplete();
    float horizontalVelocity = ysMath::GetX(velocity);
    float fallSpeed = max(-ysMath::GetY(velocity), 0.0f);
    bool isMoving = std::abs(horizontalVelocity) > 1.0f;
    bool isAlive = m_player->isAlive();

    // FSM Output
    State next = current;
    State queued = State::Undefined;

    float nextFade = 0.0f;
    float nextClip = 0.0f;
    float queuedFade = 0.0f;
    float queuedClip = 0.0f;

    // FSM
    if (current == State::Running) {
        if (onSurface) {
            if (!isAlive) {
                next = State::Dying;
                nextFade = 20.0f;
            }
            else if (isHurt) {
                next = State::ImpactDamage;
                nextFade = 20.0f;
            }
            else if (std::abs(horizontalVelocity) < 1.0f) {
                next = State::Idle;
                nextFade = 20.0f;
            }
            else {
                queued = State::Running;
            }
        }
        else {
            next = State::Idle;
            nextFade = 20.0f;
        }
    }
    else if (current == State::Falling) {
        if (onSurface) {
            if (std::abs(horizontalVelocity) < 1.0f) {
                next = State::Idle;
                nextFade = 20.0f;
            }
            else {
                next = State::Running;
                nextFade = 20.0f;
            }
        }
        else if (!isHanging) {
            if (fallSpeed > 9.0f) {
                next = State::FastFalling;
                nextFade = 40.0f;

                queued = State::FastFalling;
            }
            else {
                queued = State::Falling;
            }
        }
        else {
            next = State::Hanging;
            nextFade = 20.0f;

            queued = State::Hanging;
            queuedFade = 0.0f;
            queuedClip = 63.0f;
        }
    }
    else if (current == State::FastFalling) {
        if (onSurface) {
            if (std::abs(horizontalVelocity) < 1.0f) {
                next = State::Idle;
                nextFade = 20.0f;
            }
            else {
                next = State::Running;
                nextFade = 20.0f;
            }
        }
        else if (!isHanging) {
            if (fallSpeed > 9.0f) {
                queued = State::FastFalling;
            }
            else {
                next = State::Falling;
                nextFade = 20.0f;

                queued = State::Falling;
            }
        }
        else {
            next = State::Hanging;
            nextFade = 20.0f;

            queued = State::Hanging;
            queuedFade = 0.0f;
            queuedClip = 63.0f;
        }
    }
    else if (current == State::Idle) {
        if (onSurface) {
            if (!isAlive) {
                next = State::Dying;
                nextFade = 20.0f;
            }
            else if (isHurt) {
                next = State::ImpactDamage;
                nextFade = 20.0f;
            }
            else if (std::abs(horizontalVelocity) > 1.0f) {
                next = State::Running;
                nextFade = 20.0f;
            }
            else {
                queued = State::Idle;
            }
        }
        else {
            next = State::Falling;
            nextFade = 20.0f;
        }
    }
    else if (current == State::ImpactDamage) {
        if (!isHurt) {
            next = State::Idle;
            nextFade = 20.0f;
        }
    }
    else if (current == State::Hanging) {
        if (!isHanging) {
            if (onSurface) {
                next = State::Idle;
                nextFade = 20.0f;
            }
            else {
                next = State::Falling;
                nextFade = 20.0f;
            }
        }
        else {
            queued = State::Hanging;
            queuedFade = 0.0f;
            queuedClip = 63.0f;
        }
    }

    // Output
    result.next = next;
    result.nextClip = nextClip;
    result.nextFade = nextFade;
    result.queued = queued;
    result.queuedClip = queuedClip;
    result.queuedFade = queuedFade;
}
