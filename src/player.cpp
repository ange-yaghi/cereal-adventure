#include "../include/player.h"

#include "../include/world.h"
#include "../include/math_utilities.h"

#include <sstream>

ysAnimationAction
    *c_adv::Player::AnimArmsWalk = nullptr,
    *c_adv::Player::AnimLegsWalk = nullptr,
    *c_adv::Player::AnimLegsIdle = nullptr,
    *c_adv::Player::AnimArmsIdle = nullptr;

dbasic::SceneObjectAsset *c_adv::Player::CharacterRoot = nullptr;

c_adv::Player::Player() {
    m_armsChannel = nullptr;
    m_legsChannel = nullptr;
    m_renderSkeleton = nullptr;
    m_gripLink = nullptr;
    m_ledge = nullptr;

    m_health = 10.0f;
    m_ledgeGraspDistance = 3.0f;
}

c_adv::Player::~Player() {
    /* void */
}

void c_adv::Player::initialize() {
    GameObject::initialize();

    RigidBody.SetHint(dphysics::RigidBody::RigidBodyHint::Dynamic);
    RigidBody.SetInverseMass(1.0f);
    RigidBody.SetAlwaysAwake(true);
    RigidBody.SetRequestsInformation(true);

    dphysics::CollisionObject *bounds;
    RigidBody.CollisionGeometry.NewBoxObject(&bounds);
    bounds->SetMode(dphysics::CollisionObject::Mode::Fine);
    bounds->GetAsBox()->HalfHeight = 1.45f;
    bounds->GetAsBox()->HalfWidth = 0.2f;

    RigidBody.CollisionGeometry.NewCircleObject(&bounds);
    bounds->SetMode(dphysics::CollisionObject::Mode::Sensor);
    bounds->GetAsCircle()->Radius = 4.0f;

    m_renderSkeleton = m_world->getAssetManager().BuildRenderSkeleton(
        &RigidBody.Transform, CharacterRoot);

    m_renderSkeleton->BindAction(AnimArmsWalk, &m_animArmsWalk);
    m_renderSkeleton->BindAction(AnimLegsWalk, &m_animLegsWalk);
    m_renderSkeleton->BindAction(AnimLegsIdle, &m_animLegsIdle);
    m_renderSkeleton->BindAction(AnimArmsIdle, &m_animArmsIdle);

    m_legsChannel = m_renderSkeleton->AnimationMixer.NewChannel();
    m_armsChannel = m_renderSkeleton->AnimationMixer.NewChannel();

    m_gripCooldown.setCooldownPeriod(0.0f);
}

void c_adv::Player::process() {
    GameObject::process();

    RigidBody.ClearAccumulators();
    RigidBody.AddForceWorldSpace(
        ysMath::LoadVector(0.0f, -15.0f / RigidBody.GetInverseMass(), 0.0f),
        RigidBody.Transform.GetWorldPosition());

    processImpactDamage();
    updateMotion();
    updateAnimation();

    m_gripCooldown.update(m_world->getEngine().GetFrameLength());
}

void c_adv::Player::render() {
    int color[] = { 0xf1, 0xc4, 0x0f };
    m_world->getEngine().SetObjectTransform(RigidBody.Transform.GetWorldTransform());
    //m_world->getEngine().DrawBox(color, 5.0f, 5.0f, (int)Layer::Player);

    m_world->getEngine().SetMultiplyColor(ysVector4(1.0f, 1.0f, 1.0f, 1.0f));
    m_world->getEngine().DrawRenderSkeleton(m_renderSkeleton, 1.0f, (int)Layer::Player);

    dbasic::Console *console = m_world->getEngine().GetConsole();
    console->MoveToLocation(dbasic::GuiPoint(1, 2));
    
    console->SetFontForeColor(0, 0, 0, 1.0f);
    console->SetFontBackColor(0, 0, 0, 0.0f);

    std::stringstream msg;
    ysVector position = RigidBody.Transform.GetWorldPosition();
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
            ysVector normal = col->m_normal;
            if (std::abs(ysMath::GetScalar(ysMath::Dot(normal, ysMath::Constants::YAxis))) > 0.5f) {
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
        releaseGrip();
    }
}

void c_adv::Player::releaseGrip() {
    if (m_gripLink != nullptr) {
        m_realm->PhysicsSystem.DeleteLink(m_gripLink);
        m_gripLink = nullptr;
        m_ledge = nullptr;
    }
}

ysVector c_adv::Player::getGripLocationLocal() {
    return ysMath::LoadVector(0.2f, 1.2f, 0.0f);
}

ysVector c_adv::Player::getGripLocationWorld() {
    return RigidBody.Transform.LocalToWorldSpace(getGripLocationLocal());
}

void c_adv::Player::processImpactDamage() {
    int collisionCount = RigidBody.GetCollisionCount();

    for (int i = 0; i < collisionCount; ++i) {
        dphysics::Collision *col = RigidBody.GetCollision(i);
        // TODO: check if hanging or not

        if (!col->m_sensor && !col->IsGhost()) {
            ysVector closingVelocity = col->GetContactVelocity();
            float mag = ysMath::GetScalar(ysMath::Magnitude(closingVelocity));
            if (mag > 10.0f) {
                m_health -= (mag - 10.0f);
            }
        }
    }
}

void c_adv::Player::updateMotion() {
    dbasic::DeltaEngine &engine = m_world->getEngine();

    updateGrip();

    ysVector v = RigidBody.GetVelocity();
    
    if (isOnSurface()) {
        if (engine.ProcessKeyDown(ysKeyboard::KEY_SPACE)) {
            RigidBody.AddImpulseWorldSpace(ysMath::LoadVector(0.0f, 8.0f, 0.0f), RigidBody.Transform.GetWorldPosition());
        }

        if (engine.IsKeyDown(ysKeyboard::KEY_D)) {
            RigidBody.SetVelocity(ysMath::LoadVector(3.0f, ysMath::GetY(v), 0.0f));
        }
        else if (std::abs(ysMath::GetX(v)) > 0.01f) {
            RigidBody.AddForceWorldSpace(ysMath::LoadVector(-ysMath::GetX(v) * 10, 0.0f, 0.0f), RigidBody.Transform.GetWorldPosition());
        }
        else {
            RigidBody.SetVelocity(ysMath::LoadVector(0.0f, ysMath::GetY(v), 0.0f));
        }
    }
    else if (isHanging()) {
        if (engine.ProcessKeyDown(ysKeyboard::KEY_SPACE)) {
            RigidBody.AddImpulseWorldSpace(ysMath::LoadVector(0.0f, 10.0f, 0.0f), RigidBody.Transform.GetWorldPosition());
        }
    }
    else {
        if (engine.IsKeyDown(ysKeyboard::KEY_D) && ysMath::GetX(v) < 0.5f) {
            RigidBody.AddForceWorldSpace(ysMath::LoadVector(2.0f, 0.0f, 0.0f), RigidBody.Transform.GetWorldPosition());
        }
        else if (engine.IsKeyDown(ysKeyboard::KEY_A) && ysMath::GetX(v) > -0.5f) {
            RigidBody.AddForceWorldSpace(ysMath::LoadVector(-2.0f, 0.0f, 0.0f), RigidBody.Transform.GetWorldPosition());
        }
    }
}

void c_adv::Player::updateAnimation() {
    ysVector velocity = RigidBody.GetVelocity();
    float hMag = ysMath::GetScalar(ysMath::Dot(velocity, ysMath::Constants::XAxis));

    if (hMag > 1.0f && isOnSurface()) {
        ysAnimationChannel::ActionSettings smooth;
        smooth.FadeIn = 20.0f;
        smooth.Speed = 1.0f;

        ysAnimationChannel::ActionSettings immediate;
        immediate.FadeIn = 0.0f;
        immediate.Speed = 1.0f;

        if (m_legsChannel->GetCurrentAction() != &m_animLegsWalk) {
            m_legsChannel->AddSegment(&m_animLegsWalk, smooth);
            m_legsChannel->ClearQueue();
        }
        else if (!m_legsChannel->HasQueuedSegments()) {
            m_legsChannel->QueueSegment(&m_animLegsWalk, immediate);
        }

        if (m_armsChannel->GetCurrentAction() != &m_animArmsWalk) {
            m_armsChannel->AddSegment(&m_animArmsWalk, smooth);
            m_armsChannel->ClearQueue();
        }
        else if (!m_armsChannel->HasQueuedSegments()) {
            m_armsChannel->QueueSegment(&m_animArmsWalk, immediate);
        }
    }
    else {
        ysAnimationChannel::ActionSettings smooth;
        smooth.FadeIn = 20.0f;
        smooth.Speed = 1.0f;

        ysAnimationChannel::ActionSettings immediate;
        immediate.FadeIn = 0.0f;
        immediate.Speed = 1.0f;

        if (m_legsChannel->GetCurrentAction() != &m_animLegsIdle) {
            m_legsChannel->AddSegment(&m_animLegsIdle, smooth);
            m_legsChannel->ClearQueue();
        }
        else if (!m_legsChannel->HasQueuedSegments()) {
            m_legsChannel->QueueSegment(&m_animLegsIdle, immediate);
        }

        if (m_armsChannel->GetCurrentAction() != &m_animArmsIdle) {
            m_armsChannel->AddSegment(&m_animArmsIdle, smooth);
            m_armsChannel->ClearQueue();
        }
        else if (!m_armsChannel->HasQueuedSegments()) {
            m_armsChannel->QueueSegment(&m_animArmsIdle, immediate);
        }
    }

    m_renderSkeleton->UpdateAnimation(m_world->getEngine().GetFrameLength() * 60.0f);
}

void c_adv::Player::configureAssets(dbasic::AssetManager *am) {
    AnimLegsWalk = am->GetAction("LegsRun");
    AnimArmsWalk = am->GetAction("ArmsRun");
    AnimLegsIdle = am->GetAction("LegsIdle");
    AnimArmsIdle = am->GetAction("ArmsIdle");

    AnimLegsWalk->SetLength(39.0f);
    AnimArmsWalk->SetLength(39.0f);
    AnimLegsIdle->SetLength(100.0f);
    AnimArmsIdle->SetLength(100.0f);

    CharacterRoot = am->GetSceneObject("CerealArmature");
}
