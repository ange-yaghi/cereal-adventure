#include "../include/player.h"

#include "../include/world.h"
#include "../include/math_utilities.h"

#include <sstream>

ysAnimationAction
    *c_adv::Player::AnimArmsWalk = nullptr,
    *c_adv::Player::AnimLegsWalk = nullptr,
    *c_adv::Player::AnimLegsIdle = nullptr,
    *c_adv::Player::AnimArmsIdle = nullptr,
    *c_adv::Player::AnimTurnBack = nullptr,
    *c_adv::Player::AnimTurnForward = nullptr,
    *c_adv::Player::AnimLegsFalling = nullptr,
    *c_adv::Player::AnimLegsHanging = nullptr,
    *c_adv::Player::AnimArmsHanging = nullptr,
    *c_adv::Player::AnimLegsDamageLanding = nullptr,
    *c_adv::Player::AnimArmsDamageLanding = nullptr,
    *c_adv::Player::AnimLegsFastFalling = nullptr;

dbasic::AudioAsset
    *c_adv::Player::AudioFootstep01 = nullptr,
    *c_adv::Player::AudioFootstep02 = nullptr, 
    *c_adv::Player::AudioFootstep03 = nullptr, 
    *c_adv::Player::AudioFootstep04 = nullptr,
    *c_adv::Player::DamageImpact = nullptr;

dbasic::SceneObjectAsset *c_adv::Player::CharacterRoot = nullptr;

c_adv::Player::Player() {
    m_armsChannel = nullptr;
    m_legsChannel = nullptr;
    m_rotationChannel = nullptr;
    m_renderSkeleton = nullptr;
    m_gripLink = nullptr;
    m_ledge = nullptr;

    m_health = 10.0f;
    m_ledgeGraspDistance = 0.5f;
    m_graspReady = false;

    m_direction = Direction::Forward;
    m_nextDirection = Direction::Forward;
    m_legsState = LegsState::Idle;
    m_armsState = ArmsState::Idle;

    m_impactDamage = false;
    m_lastRunPlayhead = 0.0f;

    m_fallDamageThreshold = 15.0f;
    m_fallDamageMultiplier = 0.3f;

    m_runVelocity = 0.0f;
    m_maxRunVelocity = 4.0f;
}

c_adv::Player::~Player() {
    /* void */
}

void c_adv::Player::initialize() {
    GameObject::initialize();

    addTag(Tag::Dynamic);

    RigidBody.SetHint(dphysics::RigidBody::RigidBodyHint::Dynamic);
    RigidBody.SetInverseMass(1.0f);
    RigidBody.SetAlwaysAwake(true);
    RigidBody.SetRequestsInformation(true);

    m_springConnector.setDampingTensor(ysMath::LoadVector(0.5f, 0.5f, 0.0f));
    m_springConnector.setStiffnessTensor(ysMath::LoadVector(500.0f, 500.0f, 0.0f));
    m_springConnector.setPosition(RigidBody.Transform.GetWorldPosition());

    dphysics::CollisionObject *bounds;
    RigidBody.CollisionGeometry.NewBoxObject(&bounds);
    bounds->SetMode(dphysics::CollisionObject::Mode::Fine);
    bounds->GetAsBox()->HalfHeight = 1.48f;
    bounds->GetAsBox()->HalfWidth = 0.4f;

    RigidBody.CollisionGeometry.NewCircleObject(&bounds);
    bounds->SetMode(dphysics::CollisionObject::Mode::Sensor);
    bounds->GetAsCircle()->Radius = 4.0f;

    m_renderSkeleton = m_world->getAssetManager().BuildRenderSkeleton(
        &m_renderTransform, CharacterRoot);

    m_renderSkeleton->BindAction(AnimArmsWalk, &m_animArmsWalk);
    m_renderSkeleton->BindAction(AnimLegsWalk, &m_animLegsWalk);
    m_renderSkeleton->BindAction(AnimLegsIdle, &m_animLegsIdle);
    m_renderSkeleton->BindAction(AnimArmsIdle, &m_animArmsIdle);
    m_renderSkeleton->BindAction(AnimTurnBack, &m_animLegsTurnBack);
    m_renderSkeleton->BindAction(AnimTurnForward, &m_animLegsTurnForward);
    m_renderSkeleton->BindAction(AnimLegsFalling, &m_animLegsFalling);
    m_renderSkeleton->BindAction(AnimLegsHanging, &m_animLegsHanging);
    m_renderSkeleton->BindAction(AnimArmsHanging, &m_animArmsHanging);
    m_renderSkeleton->BindAction(AnimArmsDamageLanding, &m_animArmsDamageLanding);
    m_renderSkeleton->BindAction(AnimLegsDamageLanding, &m_animLegsDamageLanding);
    m_renderSkeleton->BindAction(AnimLegsFastFalling, &m_animLegsFastFalling);

    m_legsChannel = m_renderSkeleton->AnimationMixer.NewChannel();
    m_armsChannel = m_renderSkeleton->AnimationMixer.NewChannel();
    m_rotationChannel = m_renderSkeleton->AnimationMixer.NewChannel();

    m_gripCooldown.setCooldownPeriod(0.0f);
    m_movementCooldown.setCooldownPeriod(4.0f);
}

void c_adv::Player::process() {
    GameObject::process();

    RigidBody.AddForceWorldSpace(
        ysMath::LoadVector(0.0f, -15.0f / RigidBody.GetInverseMass(), 0.0f),
        RigidBody.Transform.GetWorldPosition());

    m_springConnector.setTarget(RigidBody.Transform.GetWorldPosition());
    m_springConnector.update(m_world->getEngine().GetFrameLength());

    m_renderTransform.SetPosition(m_springConnector.getPosition());
    m_renderTransform.SetOrientation(RigidBody.Transform.GetWorldOrientation());

    processImpactDamage();
    updateMotion();
    updateAnimation();

    m_gripCooldown.update(m_world->getEngine().GetFrameLength());
    m_movementCooldown.update(m_world->getEngine().GetFrameLength());

    if (m_world->getEngine().ProcessKeyDown(ysKeyboard::KEY_0)) {
        m_world->getEngine().PlayAudio(AudioFootstep01);
    }

    updateSoundEffects();
}

void c_adv::Player::render() {
    m_world->getEngine().ResetBrdfParameters();
    m_world->getEngine().DrawRenderSkeleton(m_renderSkeleton, 1.0f, (int)Layer::Player);

    dbasic::Console *console = m_world->getEngine().GetConsole();
    console->MoveToOrigin();

    std::stringstream msg;
    ysVector position = RigidBody.Transform.GetWorldPosition();
    msg << "////// Delta Game Engine ///////" << "\n";
    msg << "Pos " << ysMath::GetX(position) << "/" << ysMath::GetY(position) << "          \n";
    msg << "V   " << ysMath::GetX(RigidBody.GetVelocity()) << "/" << ysMath::GetY(RigidBody.GetVelocity()) << "             \n";
    msg << "FPS " << m_world->getEngine().GetAverageFramerate() << "          \n";
    msg << "AO/DO/VI: " << 
        m_realm->getAliveObjectCount() << "/" << 
        m_realm->getDeadObjectCount() << "/" << 
        m_realm->getVisibleObjectCount() << "          \n";
    msg << "Healh " << m_health << "\n";
    console->DrawGeneralText(msg.str().c_str());
}

bool c_adv::Player::isOnSurface() {
    int collisionCount = RigidBody.GetCollisionCount();

    for (int i = 0; i < collisionCount; ++i) {
        dphysics::Collision *col = RigidBody.GetCollision(i);
        if (getCollidingObject(col)->hasTag(Tag::Ledge)) continue;

        if (!col->m_sensor && !col->IsGhost()) {
            ysVector normal = (col->m_body1 == &RigidBody) ? col->m_normal : ysMath::Negate(col->m_normal);
            if (ysMath::GetScalar(ysMath::Dot(normal, ysMath::Constants::YAxis)) > 0.5f) {
                return true;
            }
        }
    }

    return false;
}

bool c_adv::Player::isHanging() {
    int collisionCount = RigidBody.GetCollisionCount();

    for (int i = 0; i < collisionCount; ++i) {
        dphysics::Collision *col = RigidBody.GetCollision(i);
        if (!getCollidingObject(col)->hasTag(Tag::Ledge)) continue;

        if (!col->m_sensor && !col->IsGhost()) {
            ysVector normal = col->m_normal;
            if (std::abs(ysMath::GetScalar(ysMath::Dot(normal, ysMath::Constants::YAxis))) > 0.5f) {
                return true;
            }
        }
    }

    return false;
}

void c_adv::Player::updateGrip() {
    dbasic::DeltaEngine &engine = m_world->getEngine();

    if (m_ledge != nullptr) {
        if (distance(m_ledge->RigidBody.Transform.GetWorldPosition(), getGripLocationWorld()) > m_ledgeGraspDistance) {
            releaseGrip();
        }
    }

    if (!isOnSurface()) {
        if (engine.IsKeyDown(ysKeyboard::KEY_SHIFT)) {
            if (m_gripCooldown.ready()) {
                m_gripCooldown.trigger();
                attemptGrip();
            }

            m_gripCooldown.lock();
        }
    }

    if (!engine.IsKeyDown(ysKeyboard::KEY_SHIFT)) {
        m_gripCooldown.unlock();
        releaseGrip();
    }
}

void c_adv::Player::attemptGrip() {
    int collisionCount = RigidBody.GetCollisionCount();

    float closestLedgeDistance = m_ledgeGraspDistance;
    GameObject *closestLedge = nullptr;

    ysVector gripLocation = getGripLocationWorld();
    float gy = ysMath::GetY(gripLocation);

    for (int i = 0; i < collisionCount; ++i) {
        dphysics::Collision *col = RigidBody.GetCollision(i);
        if (!getCollidingObject(col)->hasTag(Tag::Ledge)) continue;

        GameObject *ledge = getCollidingObject(col);
        if (col->m_sensor && !col->IsGhost()) {
            ysVector ledgePosition = ledge->RigidBody.Transform.GetWorldPosition();
            float ly = ysMath::GetY(ledgePosition);

            if (ly < gy) {
                float d = distance(ledgePosition, gripLocation);
                if (d < closestLedgeDistance) {
                    closestLedge = ledge;
                    closestLedgeDistance = d;
                }
            }
        }
    }

    if (closestLedge != nullptr) {
        m_ledge = closestLedge;
        m_graspReady = true;

        if (m_gripLink != nullptr) {
            m_gripLink->SetAnchor(closestLedge->RigidBody.Transform.GetWorldPosition());
        }
        else {
            m_gripLink = m_realm->PhysicsSystem.CreateLink<dphysics::LedgeLink>(&this->RigidBody, &closestLedge->RigidBody);
            m_gripLink->SetAnchor(closestLedge->RigidBody.Transform.GetWorldPosition());
            m_gripLink->SetGripLocal(getGripLocationLocal());
        }
    }
    else {
        m_ledge = closestLedge;
        m_graspReady = false;
        releaseGrip();
    }
}

void c_adv::Player::releaseGrip() {
    if (m_gripLink != nullptr) {
        m_realm->PhysicsSystem.DeleteLink(m_gripLink);
        m_gripLink = nullptr;
        m_ledge = nullptr;
        m_graspReady = false;
    }
}

ysVector c_adv::Player::getGripLocationLocal() {
    if (m_direction == Direction::Forward) {
        return ysMath::LoadVector(0.4f, 1.2f, 0.0f);
    }
    else {
        return ysMath::LoadVector(-0.4f, 1.2f, 0.0f);
    }
}

ysVector c_adv::Player::getGripLocationWorld() {
    return RigidBody.Transform.LocalToWorldSpace(getGripLocationLocal());
}

void c_adv::Player::processImpactDamage() {
    // Clear the impact damage flag
    m_impactDamage = false;

    int collisionCount = RigidBody.GetCollisionCount();

    for (int i = 0; i < collisionCount; ++i) {
        dphysics::Collision *col = RigidBody.GetCollision(i);
        // TODO: check if hanging or not

        if (!col->m_sensor && !col->IsGhost()) {
            ysVector closingVelocity = col->GetContactVelocityWorld();
            float mag = (&RigidBody == col->m_body1)
                ? ysMath::GetY(closingVelocity)
                : -ysMath::GetY(closingVelocity);

            if (mag < -m_fallDamageThreshold) {
                m_health -= (abs(mag) - m_fallDamageThreshold);

                m_movementCooldown.trigger();
                m_impactDamage = true;
            }
        }
    }
}

void c_adv::Player::updateMotion() {
    dbasic::DeltaEngine &engine = m_world->getEngine();

    updateGrip();

    ysVector v = RigidBody.GetVelocity();
    m_runVelocity = ysMath::GetX(v);
    
    if (isOnSurface()) {
        bool brake = true;
        if (m_movementCooldown.ready()) {
            if (engine.ProcessKeyDown(ysKeyboard::KEY_SPACE)) {
                RigidBody.AddImpulseWorldSpace(ysMath::LoadVector(0.0f, 8.0f, 0.0f), RigidBody.Transform.GetWorldPosition());
            }

            if (engine.IsKeyDown(ysKeyboard::KEY_D)) {
                m_nextDirection = Direction::Forward;

                m_runVelocity += 20.0f * engine.GetFrameLength();
                if (m_runVelocity > m_maxRunVelocity) m_runVelocity = m_maxRunVelocity;

                brake = false;
            }
            else if (engine.IsKeyDown(ysKeyboard::KEY_A)) {
                m_nextDirection = Direction::Back;

                m_runVelocity -= 20.0f * engine.GetFrameLength();
                if (m_runVelocity < -m_maxRunVelocity) m_runVelocity = -m_maxRunVelocity;

                brake = false;
            }

            if (!brake) {
                RigidBody.SetVelocity(ysMath::LoadVector(m_runVelocity, ysMath::GetY(v), 0.0f));
            }
        }

        if (brake) {
            if (std::abs(ysMath::GetX(v)) > 0.01f) {
                RigidBody.AddForceWorldSpace(ysMath::LoadVector(-ysMath::GetX(v) * 10, 0.0f, 0.0f), RigidBody.Transform.GetWorldPosition());
            }
            else {
                RigidBody.SetVelocity(ysMath::LoadVector(0.0f, ysMath::GetY(v), 0.0f));
            }
        }
    }
    else if (isHanging()) {
        if (engine.ProcessKeyDown(ysKeyboard::KEY_SPACE)) {
            RigidBody.AddImpulseWorldSpace(ysMath::LoadVector(0.0f, 10.0f, 0.0f), RigidBody.Transform.GetWorldPosition());
        }
    }
    else {
        if (engine.IsKeyDown(ysKeyboard::KEY_D) && ysMath::GetX(v) < 2.0f) {
            RigidBody.AddForceWorldSpace(ysMath::LoadVector(10.0f, 0.0f, 0.0f), RigidBody.Transform.GetWorldPosition());
            m_nextDirection = Direction::Forward;
        }
        else if (engine.IsKeyDown(ysKeyboard::KEY_A) && ysMath::GetX(v) > -2.0f) {
            RigidBody.AddForceWorldSpace(ysMath::LoadVector(-10.0f, 0.0f, 0.0f), RigidBody.Transform.GetWorldPosition());
            m_nextDirection = Direction::Back;
        }
    }
}

void c_adv::Player::updateAnimation() {
    legsAnimationFsm();
    rotationAnimationFsm();
    armsAnimationFsm();

    m_renderSkeleton->UpdateAnimation(m_world->getEngine().GetFrameLength() * 60.0f);
}

void c_adv::Player::legsAnimationFsm() {
    ysVector velocity = RigidBody.GetVelocity();
    float hMag = ysMath::GetX(velocity);
    float fallSpeed = max(-ysMath::GetY(velocity), 0.0f);

    LegsState current = m_legsState;
    LegsState next = m_legsState;
    LegsState queued = LegsState::Undefined;

    float nextFade = 0.0f;
    float queuedFade = 0.0f;
    float queuedClip = 0.0f;
    float nextOffset = 0.0f;

    if (isOnSurface()) {
        if (current == LegsState::Running) {
            if (std::abs(hMag) < 1.0f) {
                next = LegsState::Idle;
                nextFade = 20.0f;
            }
            else {
                next = LegsState::Running;
                queued = LegsState::Running;
                queuedFade = 0.0f;
            }
        }
        else if (current == LegsState::Falling || current == LegsState::FastFalling) {
            if (!m_impactDamage) {
                if (std::abs(hMag) < 1.0f) {
                    next = LegsState::Idle;
                    nextFade = 20.0f;
                }
                else {
                    next = LegsState::Running;
                    nextFade = 20.0f;
                }
            }
            else {
                next = LegsState::ImpactDamage;
                nextFade = 10.0f;
            }
        }
        else if (current == LegsState::Idle) {
            if (std::abs(hMag) > 1.0f) {
                next = LegsState::Running;
                nextFade = 20.0f;
            }
            else {
                queued = LegsState::Idle;
                queuedFade = 0.0f;
            }
        }
        else if (current == LegsState::ImpactDamage) {
            if (m_movementCooldown.ready()) {
                next = LegsState::Idle;
                nextFade = 20.0f;
            }
            else {
                next = LegsState::ImpactDamage;
            }
        }
    }
    else if (!isHanging()) {
        if (fallSpeed > 9.0f) {
            next = LegsState::FastFalling;
            nextFade = 40.0f;

            queued = LegsState::FastFalling;
            queuedFade = 0.0f;
        }
        else {
            next = LegsState::Falling;
            nextFade = 20.0f;

            queued = LegsState::Falling;
            queuedFade = 0.0f;
        }
    }
    else {
        next = LegsState::Hanging;
        nextFade = 20.0f;

        queued = LegsState::Hanging;
        queuedFade = 0.0f;
        queuedClip = 63.0f;
    }

    m_legsChannel->ClearQueue();

    if (next != current) {
        ysAnimationActionBinding *nextAnimation = nullptr;
        if (next == LegsState::Idle) {
            nextAnimation = &m_animLegsIdle;
        }
        else if (next == LegsState::Running) {
            nextAnimation = &m_animLegsWalk;
        }
        else if (next == LegsState::Falling) {
            nextAnimation = &m_animLegsFalling;
        }
        else if (next == LegsState::Hanging) {
            nextAnimation = &m_animLegsHanging;
        }
        else if (next == LegsState::ImpactDamage) {
            nextAnimation = &m_animLegsDamageLanding;
        }
        else if (next == LegsState::FastFalling) {
            nextAnimation = &m_animLegsFastFalling;
        }

        ysAnimationChannel::ActionSettings settings;
        settings.FadeIn = nextFade;
        settings.Speed = 1.0f;
        if (nextOffset == 0) {
            m_legsChannel->AddSegment(nextAnimation, settings);
        }
        else {
            m_legsChannel->AddSegmentAtOffset(nextAnimation, nextOffset, settings);
        }
    }

    if (queued != LegsState::Undefined) {
        ysAnimationActionBinding *queuedAnimation = nullptr;
        if (queued == LegsState::Idle) {
            queuedAnimation = &m_animLegsIdle;
        }
        else if (queued == LegsState::Running) {
            queuedAnimation = &m_animLegsWalk;
        }
        else if (queued == LegsState::Falling) {
            queuedAnimation = &m_animLegsFalling;
        }
        else if (queued == LegsState::Hanging) {
            queuedAnimation = &m_animLegsHanging;
        }
        else if (next == LegsState::FastFalling) {
            queuedAnimation = &m_animLegsFastFalling;
        }

        ysAnimationChannel::ActionSettings settings;
        settings.FadeIn = queuedFade;
        settings.Speed = 1.0f;
        settings.LeftClip = queuedClip;
        settings.RightClip = queuedAnimation->GetAction()->GetLength();
        settings.Clip = true;
        m_legsChannel->QueueSegment(queuedAnimation, settings);
    }

    m_legsState = next;
}

void c_adv::Player::rotationAnimationFsm() {
    if (m_nextDirection != m_direction) {
        ysAnimationChannel::ActionSettings settings;
        settings.FadeIn = 20.0f;
        settings.Speed = 1.0f;

        if (m_nextDirection == Direction::Back) {
            m_direction = Direction::Back;
            m_rotationChannel->AddSegment(&m_animLegsTurnBack, settings);
        }
        else if (m_nextDirection == Direction::Forward) {
            m_direction = Direction::Forward;
            m_rotationChannel->AddSegment(&m_animLegsTurnForward, settings);
        }
    }
}

void c_adv::Player::armsAnimationFsm() {
    ysVector velocity = RigidBody.GetVelocity();
    float hMag = ysMath::GetScalar(ysMath::Dot(velocity, ysMath::Constants::XAxis));

    ArmsState current = m_armsState;
    ArmsState next = m_armsState;
    ArmsState queued = ArmsState::Undefined;

    float nextFade = 0.0f;
    float nextClip = 0.0f;
    float queuedFade = 0.0f;
    float queuedClip = 0.0f;

    if (isOnSurface()) {
        if (m_impactDamage) {
            next = ArmsState::ImpactDamage;
            nextFade = 20.0f;
        }
        else if (current == ArmsState::Running) {
            if (std::abs(hMag) < 1.0f) {
                next = ArmsState::Idle;
                nextFade = 20.0f;
            }
            else {
                next = ArmsState::Running;
                queued = ArmsState::Running;
                queuedFade = 0.0f;
            }
        }
        else if (current == ArmsState::Idle) {
            if (std::abs(hMag) > 1.0f) {
                next = ArmsState::Running;
                nextFade = 20.0f;
            }
            else {
                queued = ArmsState::Idle;
                queuedFade = 0.0f;
            }
        }
        else if (current == ArmsState::Hanging) {
            if (std::abs(hMag) < 1.0f) {
                next = ArmsState::Idle;
                nextFade = 20.0f;
            }
            else {
                next = ArmsState::Running;
                nextFade = 20.0f;
            }
        }
        else if (current == ArmsState::ImpactDamage) {
            if (m_movementCooldown.ready()) {
                next = ArmsState::Idle;
                nextFade = 20.0f;
            }
        }
    }
    else if (isHanging()) {
        next = ArmsState::Hanging;
        nextFade = 2.0f;
        nextClip = 30.0f;

        queued = ArmsState::Hanging;
        queuedFade = 20.0f;
        queuedClip = 30.0f;
    }
    else if (m_graspReady) {
        next = ArmsState::Hanging;
        nextFade = 10.0f;
        nextClip = 10.0f;

        queued = ArmsState::Hanging;
        queuedFade = 20.0f;
        queuedClip = 30.0f;
    }
    else {
        if (current == ArmsState::Hanging) {
            next = ArmsState::Idle;
            nextFade = 40.0f;
        }
        else if (current == ArmsState::Running) {
            next = ArmsState::Idle;
            nextFade = 20.0f;

            queued = ArmsState::Idle;
            queuedFade = 0.0f;
        }
        else if (current == ArmsState::Idle) {
            next = ArmsState::Idle;
            nextFade = 20.0f;

            queued = ArmsState::Idle;
            queuedFade = 0.0f;
        }
    }

    m_armsChannel->ClearQueue();

    if (next != current) {
        ysAnimationActionBinding *nextAnimation = nullptr;
        if (next == ArmsState::Idle) {
            nextAnimation = &m_animArmsIdle;
        }
        else if (next == ArmsState::Running) {
            nextAnimation = &m_animArmsWalk;
        }
        else if (next == ArmsState::Hanging) {
            nextAnimation = &m_animArmsHanging;
        }
        else if (next == ArmsState::ImpactDamage) {
            nextAnimation = &m_animArmsDamageLanding;
            m_world->getEngine().PlayAudio(DamageImpact);
        }

        ysAnimationChannel::ActionSettings settings;
        settings.FadeIn = nextFade;
        settings.Speed = 1.0f;
        settings.Clip = true;
        settings.LeftClip = nextClip;
        settings.RightClip = nextAnimation->GetAction()->GetLength();
        m_armsChannel->AddSegment(nextAnimation, settings);
    }

    if (queued != ArmsState::Undefined) {
        ysAnimationActionBinding *queuedAnimation = nullptr;
        if (queued == ArmsState::Idle) {
            queuedAnimation = &m_animArmsIdle;
        }
        else if (queued == ArmsState::Running) {
            queuedAnimation = &m_animArmsWalk;
        }
        else if (queued == ArmsState::Hanging) {
            queuedAnimation = &m_animArmsHanging;
        }

        ysAnimationChannel::ActionSettings settings;
        settings.FadeIn = queuedFade;
        settings.Speed = 1.0f;
        settings.LeftClip = queuedClip;
        settings.RightClip = queuedAnimation->GetAction()->GetLength();
        settings.Clip = true;
        m_armsChannel->QueueSegment(queuedAnimation, settings);
    }

    m_armsState = next;
}

void c_adv::Player::updateSoundEffects() {
    constexpr float Step0 = 5.0f;
    constexpr float Step1 = 22.0f;

    if (m_legsChannel->GetCurrentAction() == &m_animLegsWalk) {
        float playhead = m_legsChannel->GetPlayhead();
        bool playFootstep = false;

        if (playhead >= Step0 && m_lastRunPlayhead < Step0) {
            playFootstep = true;
        }
        else if (playhead >= Step1 && m_lastRunPlayhead < Step1) {
            playFootstep = true;
        }

        dbasic::AudioAsset *const FootstepEffects[] = { AudioFootstep01, AudioFootstep02, AudioFootstep03, AudioFootstep04 };
        dbasic::AudioAsset *randomFootstep = nullptr;
        if (playFootstep) {
            int randomIndex = ysMath::UniformRandomInt(4);
            randomFootstep = FootstepEffects[randomIndex];

            m_world->getEngine().PlayAudio(randomFootstep);
        }

        m_lastRunPlayhead = playhead;
    }
    else {
        m_lastRunPlayhead = 0.0f;
    }
}

void c_adv::Player::configureAssets(dbasic::AssetManager *am) {
    AnimLegsWalk = am->GetAction("LegsRun");
    AnimArmsWalk = am->GetAction("ArmsRun");
    AnimLegsIdle = am->GetAction("LegsIdle");
    AnimArmsIdle = am->GetAction("ArmsIdle");
    AnimTurnBack = am->GetAction("TurnBack");
    AnimTurnForward = am->GetAction("TurnForward");
    AnimLegsFalling = am->GetAction("LegsFalling");
    AnimLegsHanging = am->GetAction("LegsHanging");
    AnimArmsHanging = am->GetAction("ArmsHanging");
    AnimLegsDamageLanding = am->GetAction("LegsDamageLanding");
    AnimArmsDamageLanding = am->GetAction("ArmsDamageLanding");
    AnimLegsFastFalling = am->GetAction("LegsHighFalling");

    AnimLegsWalk->SetLength(39.0f);
    AnimArmsWalk->SetLength(39.0f);
    AnimLegsIdle->SetLength(100.0f);
    AnimArmsIdle->SetLength(100.0f);
    AnimTurnBack->SetLength(20.0f);
    AnimLegsFalling->SetLength(100.0f);
    AnimLegsHanging->SetLength(200.0f);
    AnimArmsHanging->SetLength(60.0f);
    AnimLegsDamageLanding->SetLength(250.0f);
    AnimArmsDamageLanding->SetLength(250.0f);
    AnimLegsFastFalling->SetLength(100.0f);

    CharacterRoot = am->GetSceneObject("CerealArmature");

    AudioFootstep01 = am->GetAudioAsset("CerealBox::Footstep01");
    AudioFootstep02 = am->GetAudioAsset("CerealBox::Footstep02");
    AudioFootstep03 = am->GetAudioAsset("CerealBox::Footstep03");
    AudioFootstep04 = am->GetAudioAsset("CerealBox::Footstep04");
    DamageImpact = am->GetAudioAsset("CerealBox::DamageImpact");
}
