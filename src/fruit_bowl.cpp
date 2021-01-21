#include "../include/fruit_bowl.h"

#include "../include/world.h"
#include "../include/colors.h"
#include "../include/fruit_projectile.h"

dbasic::ModelAsset *c_adv::FruitBowl::m_fruitBowl = nullptr;

c_adv::FruitBowl::FruitBowl() {
    /* void */
}

c_adv::FruitBowl::~FruitBowl() {
    /* void */
}

void c_adv::FruitBowl::initialize() {
    GameObject::initialize();

    RigidBody.SetHint(dphysics::RigidBody::RigidBodyHint::Dynamic);
    RigidBody.SetInverseMass(0.0f);

    dphysics::CollisionObject *bounds;
    RigidBody.CollisionGeometry.NewBoxObject(&bounds);
    bounds->SetMode(dphysics::CollisionObject::Mode::Fine);
    bounds->GetAsBox()->HalfHeight = 0.8f / 2;
    bounds->GetAsBox()->HalfWidth = 1.7f / 2;
    bounds->GetAsBox()->Orientation = ysMath::Constants::QuatIdentity;
    bounds->GetAsBox()->Position = ysMath::Constants::Zero;

    m_clock.setHighTime(1.0f);
    m_clock.setLowTime(10.0f);
}

void c_adv::FruitBowl::render() {
    m_world->getShaders().ResetBrdfParameters();
    m_world->getShaders().SetBaseColor(ObjectColor);

    m_world->getShaders().SetObjectTransform(RigidBody.Transform.GetWorldTransform());
    m_world->getShaders().ConfigureModel(1.0f);
    m_world->getEngine().DrawModel(m_world->getShaders().GetRegularFlags(), m_fruitBowl, nullptr);
}

void c_adv::FruitBowl::process(float dt) {
    m_clock.update(dt);

    if (m_clock.getState()) {
        FruitProjectile *projectile = getRealm()->spawn<FruitProjectile>();
        projectile->RigidBody.Transform.SetPosition(
            ysMath::Add(RigidBody.Transform.GetWorldPosition(), ysMath::LoadVector(0.0f, 1.0f, 0.0f))
        );

        const float angle = ysMath::UniformRandom() * ysMath::Constants::PI;
        const float velocity = ysMath::UniformRandom() * 10.0f + 5.0f;

        projectile->RigidBody.SetVelocity(
            ysMath::LoadVector(cos(angle) * velocity, sin(angle) * velocity, 0.0f));

        m_clock.reset();
    }
}

void c_adv::FruitBowl::configureAssets(dbasic::AssetManager *am) {
    m_fruitBowl = am->GetModelAsset("FruitBowl");
}
