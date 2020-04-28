#include "../include/player.h"

#include "../include/world.h"

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
    bounds->GetAsBox()->HalfHeight = 1.5f;
    bounds->GetAsBox()->HalfWidth = 0.2f;
    bounds->GetAsBox()->Orientation = ysMath::Constants::QuatIdentity;
    bounds->GetAsBox()->Position = ysMath::Constants::Zero;

    m_renderSkeleton = m_world->getAssetManager().BuildRenderSkeleton(
        &RigidBody.Transform, CharacterRoot);

    m_renderSkeleton->BindAction(AnimArmsWalk, &m_animArmsWalk);
    m_renderSkeleton->BindAction(AnimLegsWalk, &m_animLegsWalk);
    m_renderSkeleton->BindAction(AnimLegsIdle, &m_animLegsIdle);
    m_renderSkeleton->BindAction(AnimArmsIdle, &m_animArmsIdle);

    m_legsChannel = m_renderSkeleton->AnimationMixer.NewChannel();
    m_armsChannel = m_renderSkeleton->AnimationMixer.NewChannel();
}

void c_adv::Player::process() {
    GameObject::process();

    RigidBody.ClearAccumulators();
    RigidBody.AddForceWorldSpace(
        ysMath::LoadVector(0.0f, -15.0f / RigidBody.GetInverseMass(), 0.0f),
        RigidBody.Transform.GetWorldPosition());

    updateMotion();
    updateAnimation();
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
    console->DrawGeneralText(msg.str().c_str());
}

bool c_adv::Player::isOnSurface() {
    int collisionCount = RigidBody.GetCollisionCount();
    float closestBoxDistance = 0.0f;
    GameObject *closestBox = nullptr;

    for (int i = 0; i < collisionCount; ++i) {
        dphysics::Collision *col = RigidBody.GetCollision(i);
        dphysics::RigidBody *body = col->m_body1 == &RigidBody
            ? col->m_body2
            : col->m_body1;

        if (!col->m_sensor && !col->IsGhost()) {
            ysVector normal = col->m_normal;
            if (std::abs(ysMath::GetScalar(ysMath::Dot(normal, ysMath::Constants::YAxis))) > 0.5f) {
                return true;
            }
        }
    }

    return false;
}

void c_adv::Player::updateMotion() {
    dbasic::DeltaEngine &engine = m_world->getEngine();

    ysVector v = RigidBody.GetVelocity();
    
    if (isOnSurface()) {
        if (engine.ProcessKeyDown(ysKeyboard::KEY_SPACE)) {
            RigidBody.AddImpulseWorldSpace(ysMath::LoadVector(0.0f, 4.0f, 0.0f), RigidBody.Transform.GetWorldPosition());
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
