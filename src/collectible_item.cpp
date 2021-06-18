#include "../include/collectible_item.h"

#include "../include/world.h"
#include "../include/math_utilities.h"

c_adv::CollectibleItem::CollectibleItem() {
    m_asset = nullptr;
    m_glowColor = ysMath::Constants::One;
}

c_adv::CollectibleItem::~CollectibleItem() {
    /* void */
}

void c_adv::CollectibleItem::initialize() {
    GameObject::initialize();

    m_collectionTimer.setCooldownPeriod(0.3f);
    m_collectionTimer.disable();

    m_emissionWave.setPeriod(1.0f);
    m_spinTimer.setPeriod(3.0f);
    m_floatWave.setPeriod(5.0f);

    m_rAxis_x.setPeriod(60.0f);
    m_rAxis_z.setPeriod(120.0f);

    RigidBody.SetHint(dphysics::RigidBody::RigidBodyHint::Dynamic);
    RigidBody.SetInverseMass(0.0f);
    RigidBody.SetAlwaysAwake(true);
    RigidBody.SetRequestsInformation(true);

    dphysics::CollisionObject *bounds;
    RigidBody.CollisionGeometry.NewCircleObject(&bounds);
    bounds->SetMode(dphysics::CollisionObject::Mode::Sensor);
    bounds->GetAsCircle()->Position = ysMath::Constants::Zero;
    bounds->GetAsCircle()->Radius = 4.0f;
    
    m_audio = m_world->getAssetManager().GetAudioAsset("Collection::Mysterious");
}

void c_adv::CollectibleItem::render() {
    const ysVector rotationAxis = ysMath::Normalize(ysMath::LoadVector(
        m_rAxis_x.get(),
        1.0f,
        m_rAxis_z.get()
    ));

    ysTransform renderTransform = RigidBody.Transform;
    renderTransform.SetPosition(
        ysMath::Add(
            renderTransform.GetPositionParentSpace(),
            ysMath::LoadVector(0, m_floatWave.get() * 0.2f, 0)
        ));
    renderTransform.SetOrientation(
        ysMath::QuatMultiply(
            renderTransform.GetOrientationParentSpace(),
            ysMath::LoadQuaternion(
                m_spinTimer.get() * ysMath::Constants::TWO_PI, rotationAxis)
        )
    );

    const float collectionProgress = m_collectionTimer.active()
        ? m_collectionTimer.get()
        : 0.0f;

    const float glow = 0.5f * (m_floatWave.get() + 1);
    const float collectionGlow = std::sin(collectionProgress * collectionProgress * ysMath::Constants::PI);

    m_world->getShaders().SetObjectTransform(renderTransform.GetWorldTransform());
    m_world->getShaders().ConfigureModel(0.5f * (1 - collectionProgress * collectionProgress), m_asset);
    m_world->getShaders().SetEmission(ysMath::LoadScalar(collectionGlow));

    m_world->getEngine().DrawModel(
        m_world->getShaders().GetRegularFlags(),
        m_asset);

    Light glowLamp{};
    glowLamp.Active = 1;
    glowLamp.FalloffEnabled = 1;
    glowLamp.Color = ysVector4(glow + collectionGlow, glow + collectionGlow, glow + collectionGlow); glowLamp.Color.Scale(10.0f);
    glowLamp.Position = ysMath::GetVector4(renderTransform.GetWorldPosition());
    glowLamp.FalloffScale = 0.1f;

    m_world->getShaders().AddLight(glowLamp);
}

void c_adv::CollectibleItem::process(float dt) {
    GameObject::process(dt);

    m_collectionTimer.update(dt);

    m_emissionWave.update(dt);
    m_floatWave.update(dt);
    m_spinTimer.update(dt);

    m_rAxis_x.update(dt);
    m_rAxis_z.update(dt);

    if (m_collectionTimer.ready()) {
        setDeletionFlag();
    }
    else if (m_collectionTimer.active()) {
        m_spinTimer.setPeriod(3.0f * (1 - m_collectionTimer.get() + 0.01));
    }

    collidingWithPlayerCheck();
}

void c_adv::CollectibleItem::collidingWithPlayerCheck() {
    if (m_collectionTimer.enabled()) return;

    bool collidingWithPlayer = false;
    const int collisionCount = RigidBody.GetCollisionCount();
    for (int i = 0; i < collisionCount; ++i) {
        dphysics::Collision *col = RigidBody.GetCollision(i);
        if (!getCollidingObject(col)->hasTag(Tag::Player)) continue;

        GameObject *player = getCollidingObject(col);
        if (col->m_sensor && !col->IsGhost()) {
            if (inRange(
                player->RigidBody.Transform.GetWorldPosition(),
                RigidBody.Transform.GetWorldPosition(),
                2.0))
            {
                collidingWithPlayer = true;
                break;
            }
        }
    }

    if (collidingWithPlayer) {
        m_collectionTimer.trigger();
        m_world->getEngine().PlayAudio(m_audio);
    }
}
