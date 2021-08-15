#include "../include/player.h"

#include "../include/world.h"
#include "../include/math_utilities.h"
#include "../include/colors.h"

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
    *c_adv::Player::AnimLegsFastFalling = nullptr,
    *c_adv::Player::AnimArmsLaunch = nullptr,
    *c_adv::Player::AnimArmsDie = nullptr,
    *c_adv::Player::AnimLegsDie = nullptr;

dbasic::AudioAsset
    *c_adv::Player::AudioFootstep01 = nullptr,
    *c_adv::Player::AudioFootstep02 = nullptr, 
    *c_adv::Player::AudioFootstep03 = nullptr, 
    *c_adv::Player::AudioFootstep04 = nullptr,
    *c_adv::Player::AudioJumpVocal01 = nullptr,
    *c_adv::Player::AudioJumpVocal02 = nullptr,
    *c_adv::Player::AudioShake01 = nullptr,
    *c_adv::Player::AudioShake02 = nullptr,
    *c_adv::Player::AudioShake03 = nullptr,
    *c_adv::Player::DamageImpact = nullptr,
    *c_adv::Player::AudioDamage01 = nullptr,
    *c_adv::Player::AudioDamage02 = nullptr;

dbasic::SceneObjectAsset *c_adv::Player::CharacterRoot = nullptr;
dbasic::ModelAsset *c_adv::Player::Sphere = nullptr;
dbasic::Material *c_adv::Player::Material = nullptr;

c_adv::Player::Player() {
    m_armsChannel = nullptr;
    m_legsChannel = nullptr;
    m_rotationChannel = nullptr;
    m_renderSkeleton = nullptr;
    m_gripLink = nullptr;
    m_ledge = nullptr;

    m_health = 20.0f;
    m_ledgeGraspDistance = 0.4f;
    m_graspReady = false;
    m_launching = false;

    m_direction = Direction::Forward;
    m_nextDirection = Direction::Forward;
    m_legsState = LegsState::Idle;

    m_lastRunPlayhead = 0.0f;

    m_terminalFallVelocity = 15.0f;
    m_fallDamageThreshold = 13.0f;
    m_fallDamageMultiplier = 0.3f;
    m_landingVelocityThreshold = 2.0f;

    m_lastMissReason = "";

    m_bodyCollider = nullptr;
    m_walkCollider = nullptr;

    m_consoleEnabled = false;
}

c_adv::Player::~Player() {
    /* void */
}

void c_adv::Player::initialize() {
    GameObject::initialize();

    addTag(Tag::Dynamic);
    addTag(Tag::Player);

    m_fireDamageComponent.initialize(this);
    m_walkComponent.initialize(this);
    m_projectileDamageComponent.initialize(this);
    m_armsFsm.initialize(this);
    m_legsFsm.initialize(this);
    m_deathComponent.initialize(this);

    RigidBody.SetHint(dphysics::RigidBody::RigidBodyHint::Dynamic);
    RigidBody.SetInverseMass(1.0f);
    RigidBody.SetAlwaysAwake(true);
    RigidBody.SetRequestsInformation(true);
    RigidBody.SetMaterial(PlayerFrictionMaterial);

    m_springConnector.setDampingTensor(ysMath::LoadVector(0.5f, 0.5f, 0.0f));
    m_springConnector.setStiffnessTensor(ysMath::LoadVector(500.0f, 500.0f, 0.0f));
    m_springConnector.setPosition(RigidBody.Transform.GetWorldPosition());

    RigidBody.CollisionGeometry.NewBoxObject(&m_bodyCollider);
    m_bodyCollider->SetMode(dphysics::CollisionObject::Mode::Fine);
    m_bodyCollider->GetAsBox()->HalfHeight = 1.0f;
    m_bodyCollider->GetAsBox()->HalfWidth = 0.3f;

    RigidBody.CollisionGeometry.NewBoxObject(&m_walkCollider);
    m_walkCollider->SetMode(dphysics::CollisionObject::Mode::Fine);
    m_walkCollider->SetRelativePosition(ysMath::LoadVector(0.0f, -1.15f, 0.0f));
    m_walkCollider->GetAsBox()->HalfHeight = 0.3f;
    m_walkCollider->GetAsBox()->HalfWidth = 0.3f;

    dphysics::CollisionObject *bounds;
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
    m_renderSkeleton->BindAction(AnimArmsLaunch, &m_animArmsLaunch);
    m_renderSkeleton->BindAction(AnimArmsDie, &m_animArmsDie);
    m_renderSkeleton->BindAction(AnimLegsDie, &m_animLegsDie);

    m_legsChannel = m_renderSkeleton->AnimationMixer.NewChannel();
    m_armsChannel = m_renderSkeleton->AnimationMixer.NewChannel();
    m_rotationChannel = m_renderSkeleton->AnimationMixer.NewChannel();

    m_gripCooldown.setCooldownPeriod(0.0f);
    m_movementCooldown.setCooldownPeriod(4.0f);
    m_footstepCooldown.setCooldownPeriod(0.25f);
    m_shakeCooldown.setCooldownPeriod(1.0f);
}

void c_adv::Player::process(float dt) {
    GameObject::process(dt);

    m_fireDamageComponent.process(dt);
    m_walkComponent.process(dt);
    m_projectileDamageComponent.process(dt);
    m_deathComponent.process(dt);

    if (m_world->getEngine().ProcessKeyDown(ysKey::Code::T)) {
        takeDamage(1.0f);
    }

    const ysVector v = RigidBody.GetVelocity();
    if (ysMath::GetY(v) > -m_terminalFallVelocity) {
        RigidBody.AddForceWorldSpace(
            ysMath::LoadVector(0.0f, -15.0f / RigidBody.GetInverseMass(), 0.0f),
            RigidBody.Transform.GetWorldPosition());
    }

    m_springConnector.setTarget(RigidBody.Transform.GetWorldPosition());
    m_springConnector.update(dt);

    m_renderTransform.SetPosition(RigidBody.Transform.GetWorldPosition());
    m_renderTransform.SetOrientation(RigidBody.Transform.GetWorldOrientation());

    processImpactDamage();
    updateMotion(dt);
    updateAnimation(dt);

    m_gripCooldown.update(dt);
    m_movementCooldown.update(dt);
    m_footstepCooldown.update(dt);
    m_shakeCooldown.update(dt);

    if (m_world->getEngine().ProcessKeyDown(ysKey::Code::F1)) {
        m_world->getEngine().GetConsole()->Clear();
        m_consoleEnabled = !m_consoleEnabled;
    }

    updateSoundEffects();
    updateCollisionBounds();

    m_world->getUi().setPlayerHealth(m_health / 20.0f);

    m_ledgeGraspDistance = max(std::abs(ysMath::GetY(RigidBody.GetVelocity()) * 0.4f), 0.4f);
}

void c_adv::Player::render() {
    m_world->getShaders().ResetBrdfParameters();
    m_world->getEngine().DrawRenderSkeleton(
        m_world->getShaders().GetRegularFlags(),
        m_renderSkeleton,
        1.0f,
        &m_world->getShaders(),
        (int)Layer::Player);

    m_world->getShaders().ResetBrdfParameters();
    m_world->getShaders().SetLit(false);
    m_world->getShaders().SetBaseColor(DebugRed);
    m_world->getShaders().SetColorReplace(true);

    const ysMatrix mat = ysMath::TranslationTransform(getGripLocationWorld());
    m_world->getShaders().SetObjectTransform(mat);
    m_world->getShaders().SetScale(1, 1, 1);
    bool ready;
    //if (findGrip(ready) != nullptr) m_world->getEngine().DrawModel(m_world->getShaders().GetRegularFlags(), Sphere);
    
    if (m_consoleEnabled) {
        dbasic::Console *console = m_world->getEngine().GetConsole();
        console->Clear();
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
        msg << "Health: " << m_health << "              \n";
        msg << "Last Miss: " << m_lastMissReason << "              \n";
        msg << "Status: ";
        if (isHanging()) msg << "HANGING ";
        if (m_walkComponent.isOnSurface()) msg << "ON SURFACE ";
        msg << "             \n";
        msg << "RUN FORCE V: " << m_walkComponent.getRunVelocity() << "                               \n";

        console->DrawGeneralText(msg.str().c_str());
    }
}

bool c_adv::Player::isAlive() const {
    return m_health > 0.0f;
}

bool c_adv::Player::isHurt() const {
    return !m_movementCooldown.ready();
}

bool c_adv::Player::isHanging() {
    const int collisionCount = RigidBody.GetCollisionCount();
    for (int i = 0; i < collisionCount; ++i) {
        dphysics::Collision *col = RigidBody.GetCollision(i);
        if (!getCollidingObject(col)->hasTag(Tag::Ledge)) continue;

        if (!col->m_sensor && !col->IsGhost()) {
            ysVector normal = col->m_normal;
            if (std::abs(
                ysMath::GetScalar(ysMath::Dot(normal, ysMath::Constants::YAxis))) > 0.5f)
            {
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

    if (!m_walkComponent.isOnSurface()) {
        if (engine.IsKeyDown(ysKey::Code::Shift)) {
            bool ready = false;
            if (m_gripCooldown.ready()) {
                ready = attemptGrip();

                if (!ready || isGraspReady()) {
                    m_gripCooldown.trigger();
                }
            }

            if (!ready) {
                m_gripCooldown.disable();
            }
        }
    }

    if (!engine.IsKeyDown(ysKey::Code::Shift)) {
        m_gripCooldown.enable();
        releaseGrip();
    }
}

bool c_adv::Player::attemptGrip() {
    bool ready = false;
    GameObject *closestLedge = findGrip(ready);

    if (closestLedge != nullptr) {
        if (ready) {
            m_ledge = closestLedge;
            m_graspReady = true;

            if (m_gripLink == nullptr) {
                m_gripLink = m_realm->PhysicsSystem.CreateLink<dphysics::LedgeLink>(&this->RigidBody, &closestLedge->RigidBody);
                m_gripLink->SetAnchor(closestLedge->RigidBody.Transform.GetWorldPosition());
                m_gripLink->SetGripLocal(getGripLocationLocal());
            }

            m_gripLink->SetAnchor(closestLedge->RigidBody.Transform.GetWorldPosition());

            return false;
        }
        else return true;
    }
    else {
        m_ledge = nullptr;
        m_graspReady = false;
        releaseGrip();

        return false;
    }
}

c_adv::GameObject *c_adv::Player::findGrip(bool &ready) {
    const int collisionCount = RigidBody.GetCollisionCount();

    float closestLedgeDistance = FLT_MAX;
    GameObject *closestLedge = nullptr;

    ysVector gripLocation = getGripLocationWorld();
    const float gy = ysMath::GetY(gripLocation);
    const float gx = ysMath::GetX(gripLocation);

    ready = false;

    for (int i = 0; i < collisionCount; ++i) {
        dphysics::Collision *col = RigidBody.GetCollision(i);
        if (!getCollidingObject(col)->hasTag(Tag::Ledge)) continue;

        GameObject *ledge = getCollidingObject(col);
        if (col->m_sensor && !col->IsGhost()) {
            ysVector ledgePosition = ledge->RigidBody.Transform.GetWorldPosition();
            const float ly = ysMath::GetY(ledgePosition);
            const float lx = ysMath::GetX(ledgePosition);

            if (ly < gy) {
                const float d = distance(gripLocation, ledgePosition);
                if (d < closestLedgeDistance && d < m_ledgeGraspDistance && d > m_ledgeGraspDistance * 0.2f) {
                    ready = (std::abs(gx - lx) < 0.1f);
                    closestLedge = ledge;
                    closestLedgeDistance = d;
                }
            }
        }
    }

    return closestLedge;
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
        return ysMath::LoadVector(0.4f, 1.3f, 0.0f);
    }
    else {
        return ysMath::LoadVector(-0.4f, 1.3f, 0.0f);
    }
}

ysVector c_adv::Player::getGripLocationWorld() {
    return RigidBody.Transform.LocalToWorldSpace(getGripLocationLocal());
}

void c_adv::Player::processImpactDamage() {
    const float VerticalThreshold = ysMath::Constants::SQRT_2 / 2;

    const int collisionCount = RigidBody.GetCollisionCount();
    for (int i = 0; i < collisionCount; ++i) {
        dphysics::Collision *col = RigidBody.GetCollision(i);
        // TODO: check if hanging or not

        if (!col->m_sensor && !col->IsGhost()) {
            if (std::abs(ysMath::GetY(col->m_normal)) < VerticalThreshold) continue;

            GameObject *object = getCollidingObject(col);
            dphysics::RigidBody &other = object->RigidBody;

            const ysVector closingVelocity = col->GetContactVelocityWorld();
            const float mag = (&RigidBody == col->m_body1)
                ? ysMath::GetY(closingVelocity)
                : -ysMath::GetY(closingVelocity);

            if (mag < -m_fallDamageThreshold && other.GetInverseMass() < RigidBody.GetInverseMass()) {
                takeDamage(abs(mag) - m_fallDamageThreshold);
                m_movementCooldown.trigger();

                playShakeSound();
            }

            if (mag < -m_landingVelocityThreshold) {
                playShakeSound();
                if (!object->hasTag(GameObject::Tag::Ledge)) {
                    onLand();
                }
            }
        }
    }
}

void c_adv::Player::updateCollisionBounds() {
    ysTransform *prevParent = m_renderSkeleton->GetRoot()->Transform.GetParent();
    m_renderSkeleton->GetRoot()->Transform.SetParent(&RigidBody.Transform);

    ysTransform *transform = &m_renderSkeleton->GetNode("Body")->Transform;
    const ysQuaternion worldOrientation = transform->GetWorldOrientation();

    const ysVector worldPosition = ysMath::Mask(transform->GetWorldPosition(), ysMath::Constants::MaskOffZ);
    const ysVector offset = transform->LocalToWorldDirection(ysMath::LoadVector(0.0f, 0.3f, 0.0f));
    const ysVector localPosition = RigidBody.Transform.WorldToLocalSpace(ysMath::Add(worldPosition, offset));

    const ysVector dir = ysMath::Constants::YAxis;
    const ysVector t = ysMath::QuatTransform(worldOrientation, dir);

    const float cosTheta = ysMath::GetScalar(ysMath::Dot(dir, t));
    const float x_com = ysMath::GetX(t);
    const float angle = (x_com > 0)
        ? -std::acos(cosTheta)
        : std::acos(cosTheta);
    
    ysQuaternion orientation = ysMath::LoadQuaternion(
        angle,
        ysMath::Constants::ZAxis
    );

    const ysQuaternion localOrientation = RigidBody.Transform.WorldToLocalOrientation(orientation);

    m_bodyCollider->SetRelativeOrientation(localOrientation);
    m_bodyCollider->SetRelativePosition(localPosition);

    m_renderSkeleton->GetRoot()->Transform.SetParent(prevParent);
}

void c_adv::Player::takeDamage(float damage) {
    m_world->getUi().triggerDamage(min(20.0f, damage) / 20.0f);

    releaseGrip();
    m_health -= damage;

    dbasic::AudioAsset *const DamageEffects[] = {
        AudioDamage01,
        AudioDamage02
    };

    const int randomIndex = ysMath::UniformRandomInt(2);
    m_world->getEngine().PlayAudio(DamageEffects[randomIndex]);
}

ysAnimationActionBinding *c_adv::Player::getArmsAction(PlayerArmsFsm::State state) {
    switch (state) {
    case PlayerArmsFsm::State::Hanging:
        return &m_animArmsHanging;
    case PlayerArmsFsm::State::Idle:
        return &m_animArmsIdle;
    case PlayerArmsFsm::State::ImpactDamage:
        return &m_animArmsDamageLanding;
    case PlayerArmsFsm::State::Running:
        return &m_animArmsWalk;
    case PlayerArmsFsm::State::Launching:
        return &m_animArmsLaunch;
    case PlayerArmsFsm::State::Dying:
        return &m_animArmsDie;
    default:
        return nullptr;
    }
}

ysAnimationActionBinding *c_adv::Player::getLegsAction(PlayerLegsFsm::State state) {
    switch (state) {
    case PlayerLegsFsm::State::Hanging:
        return &m_animLegsHanging;
    case PlayerLegsFsm::State::Idle:
        return &m_animLegsIdle;
    case PlayerLegsFsm::State::ImpactDamage:
        return &m_animLegsDamageLanding;
    case PlayerLegsFsm::State::Running:
        return &m_animLegsWalk;
    case PlayerLegsFsm::State::Dying:
        return &m_animLegsDie;
    case PlayerLegsFsm::State::Falling:
        return &m_animLegsFalling;
    case PlayerLegsFsm::State::FastFalling:
        return &m_animLegsFastFalling;
    default:
        return nullptr;
    }
}

void c_adv::Player::updateMotion(float dt) {
    dbasic::DeltaEngine &engine = m_world->getEngine();
    ysVector v = RigidBody.GetVelocity();

    updateGrip();

    m_walkComponent.setWalkingRight(false);
    m_walkComponent.setWalkingLeft(false);

    m_launching = false;

    if (isAlive()) {
        if (m_movementCooldown.ready()) {
            if (engine.IsKeyDown(ysKey::Code::D)) {
                m_nextDirection = Direction::Forward;
                m_walkComponent.setWalkingRight(true);
            }
            else if (engine.IsKeyDown(ysKey::Code::A)) {
                m_nextDirection = Direction::Back;
                m_walkComponent.setWalkingLeft(true);
            }
        }

        if (m_walkComponent.isOnSurface()) {
            if (m_movementCooldown.ready()) {
                if (engine.ProcessKeyDown(ysKey::Code::Space)) {
                    onJump();
                    if (engine.IsKeyDown(ysKey::Code::Control)) {
                        RigidBody.AddImpulseWorldSpace(
                            ysMath::LoadVector(0.0f, 8.0f, 0.0f), 
                            RigidBody.Transform.GetWorldPosition());
                    }
                    else {
                        RigidBody.AddImpulseWorldSpace(
                            ysMath::LoadVector(0.0f, 6.0f, 0.0f), 
                            RigidBody.Transform.GetWorldPosition());
                    }
                }
            }
        }
        else if (isHanging()) {
            if (engine.ProcessKeyDown(ysKey::Code::Space)) {
                onJump();
                RigidBody.AddImpulseWorldSpace(
                    ysMath::LoadVector(0.0f, 10.0f, 0.0f), 
                    RigidBody.Transform.GetWorldPosition());
                m_launching = true;
            }
        }
        else {
            if (engine.IsKeyDown(ysKey::Code::D) && ysMath::GetX(v) < 2.0f) {
                RigidBody.AddForceWorldSpace(
                    ysMath::LoadVector(10.0f, 0.0f, 0.0f), 
                    RigidBody.Transform.GetWorldPosition());
                m_nextDirection = Direction::Forward;
            }
            else if (engine.IsKeyDown(ysKey::Code::A) && ysMath::GetX(v) > -2.0f) {
                RigidBody.AddForceWorldSpace(
                    ysMath::LoadVector(-10.0f, 0.0f, 0.0f), 
                    RigidBody.Transform.GetWorldPosition());
                m_nextDirection = Direction::Back;
            }
        }
    }
}

void c_adv::Player::updateAnimation(float dt) {
    legsAnimationFsm();
    rotationAnimationFsm();
    armsAnimationFsm();

    m_renderSkeleton->UpdateAnimation(dt * 60.0f);
}

void c_adv::Player::legsAnimationFsm() {
    PlayerLegsFsm::State current = m_legsFsm.getState();
    PlayerLegsFsm::FsmResults next;
    m_legsFsm.nextState(next);

    m_legsChannel->ClearQueue();

    if (next.next != current) {
        ysAnimationActionBinding *nextAnimation = getLegsAction(next.next);

        ysAnimationChannel::ActionSettings settings;
        settings.FadeIn = next.nextFade;
        settings.Speed = 1.0f;
        settings.Clip = true;
        settings.LeftClip = next.nextClip;
        settings.RightClip = nextAnimation->GetAction()->GetLength();
        m_legsChannel->AddSegment(nextAnimation, settings);
    }

    if (next.queued != PlayerLegsFsm::State::Undefined) {
        ysAnimationActionBinding *queuedAnimation = getLegsAction(next.queued);

        ysAnimationChannel::ActionSettings settings;
        settings.FadeIn = next.queuedFade;
        settings.Speed = 1.0f;
        settings.LeftClip = next.queuedClip;
        settings.RightClip = queuedAnimation->GetAction()->GetLength();
        settings.Clip = true;
        m_legsChannel->QueueSegment(queuedAnimation, settings);
    }

    m_legsFsm.updateState(next.next);
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

        playShakeSound();
    }
}

void c_adv::Player::armsAnimationFsm() {
    PlayerArmsFsm::State current = m_armsFsm.getState();
    PlayerArmsFsm::FsmResults next;
    m_armsFsm.nextState(next);

    m_armsChannel->ClearQueue();

    if (next.next != current) {
        ysAnimationActionBinding *nextAnimation = getArmsAction(next.next);

        ysAnimationChannel::ActionSettings settings;
        settings.FadeIn = next.nextFade;
        settings.Speed = next.speed;
        settings.Clip = true;
        settings.LeftClip = next.nextClip;
        settings.RightClip = nextAnimation->GetAction()->GetLength();
        m_armsChannel->AddSegment(nextAnimation, settings);
    }

    if (next.queued != PlayerArmsFsm::State::Undefined) {
        ysAnimationActionBinding *queuedAnimation = getArmsAction(next.queued);

        ysAnimationChannel::ActionSettings settings;
        settings.FadeIn = next.queuedFade;
        settings.Speed = next.speed;
        settings.LeftClip = next.queuedClip;
        settings.RightClip = queuedAnimation->GetAction()->GetLength();
        settings.Clip = true;
        m_armsChannel->QueueSegment(queuedAnimation, settings);
    }

    m_armsFsm.updateState(next.next);
}

void c_adv::Player::updateSoundEffects() {
    constexpr float Step0 = 5.0f;
    constexpr float Step1 = 22.0f;

    if (m_legsChannel->GetCurrentAction() == &m_animLegsWalk) {
        const float playhead = m_legsChannel->GetPlayhead();
        bool playFootstep = false;

        if (playhead >= Step0 && m_lastRunPlayhead < Step0) {
            playFootstep = true;
        }
        else if (playhead >= Step1 && m_lastRunPlayhead < Step1) {
            playFootstep = true;
        }

        if (playFootstep) {
            playFootstepSound();
        }

        m_lastRunPlayhead = playhead;
    }
    else {
        m_lastRunPlayhead = 0.0f;
    }
}

void c_adv::Player::onJump() {
    playShakeSound();

    if (ysMath::UniformRandom() > 0.75f) {
        dbasic::AudioAsset *const JumpEffects[] = {
            AudioJumpVocal01,
            AudioJumpVocal02
        };

        const int randomIndex = ysMath::UniformRandomInt(sizeof(JumpEffects) / sizeof(dbasic::AudioAsset *));
        m_world->getEngine().PlayAudio(JumpEffects[randomIndex]);
    }
}

void c_adv::Player::onLand() {
    playFootstepSound();
}

void c_adv::Player::playFootstepSound() {
    if (m_footstepCooldown.ready()) {
        m_footstepCooldown.trigger();

        dbasic::AudioAsset *const FootstepEffects[] = {
            AudioFootstep01,
            AudioFootstep02,
            AudioFootstep03,
            AudioFootstep04
        };

        const int randomIndex = ysMath::UniformRandomInt(4);
        dbasic::AudioAsset *randomFootstep = FootstepEffects[randomIndex];

        m_world->getEngine().PlayAudio(randomFootstep);
    }
}

void c_adv::Player::playShakeSound() {
    if (ysMath::UniformRandom() < 0.5f) return;

    if (m_shakeCooldown.ready()) {
        m_shakeCooldown.trigger();

        dbasic::AudioAsset *const ShakeEffect[] = {
            AudioShake01,
            AudioShake02,
            AudioShake03
        };

        const int randomIndex = ysMath::UniformRandomInt(3);
        m_world->getEngine().PlayAudio(ShakeEffect[randomIndex]);
    }
}

void c_adv::Player::getAssets(dbasic::AssetManager *am) {
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
    AnimArmsLaunch = am->GetAction("ArmsLaunch");
    AnimArmsDie = am->GetAction("ArmsDie");
    AnimLegsDie = am->GetAction("LegsDie");

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
    AnimArmsLaunch->SetLength(25.0f);
    AnimArmsDie->SetLength(340.0f);
    AnimLegsDie->SetLength(340.0f);

    CharacterRoot = am->GetSceneObject("CerealArmature", ysObjectData::ObjectType::Empty);

    AudioFootstep01 = am->GetAudioAsset("CerealBox::Footstep01");
    AudioFootstep02 = am->GetAudioAsset("CerealBox::Footstep02");
    AudioFootstep03 = am->GetAudioAsset("CerealBox::Footstep03");
    AudioFootstep04 = am->GetAudioAsset("CerealBox::Footstep04");
    AudioJumpVocal01 = am->GetAudioAsset("CerealBox::JumpVocal01");
    AudioJumpVocal02 = am->GetAudioAsset("CerealBox::JumpVocal02");
    AudioShake01 = am->GetAudioAsset("CerealBox::Shake01");
    AudioShake02 = am->GetAudioAsset("CerealBox::Shake02");
    AudioShake03 = am->GetAudioAsset("CerealBox::Shake03");
    AudioDamage01 = am->GetAudioAsset("CerealBox::Damage01");
    AudioDamage02 = am->GetAudioAsset("CerealBox::Damage02");
    DamageImpact = am->GetAudioAsset("CerealBox::DamageImpact");

    Sphere = am->GetModelAsset("Sphere");

    Material = am->FindMaterial("PlayerMaterial");
}
