#include "../include/fruit_bowl.h"

#include "../include/world.h"
#include "../include/colors.h"
#include "../include/fruit_projectile.h"

dbasic::RenderSkeleton *c_adv::FruitBowl::s_fruitBowl = nullptr;
dbasic::ModelAsset *c_adv::FruitBowl::s_apple = nullptr;
dbasic::ModelAsset *c_adv::FruitBowl::s_banana = nullptr;
dbasic::ModelAsset *c_adv::FruitBowl::s_pear = nullptr;

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

    m_renderTransform.SetParent(&RigidBody.Transform);

    m_clock.setHighTime(1.0f);
    m_clock.setLowTime(10.0f);
}

void c_adv::FruitBowl::render() {
    m_world->getShaders().ResetBrdfParameters();
    m_world->getShaders().SetBaseColor(ObjectColor);

    m_world->getEngine().DrawRenderSkeleton(
        m_world->getShaders().GetRegularFlags(),
        s_fruitBowl,
        1.0f,
        &m_world->getShaders(),
        (int)Layer::Items);
}

void c_adv::FruitBowl::process(float dt) {
    m_clock.update(dt);

    if (m_clock.getState()) {
        dbasic::ModelAsset *types[] = { s_apple, s_banana, s_pear };
        dbasic::ModelAsset *projectileType = types[ysMath::UniformRandomInt(3)];

        FruitProjectile *projectile = getRealm()->spawn<FruitProjectile>();
        projectile->setAsset(projectileType);
        projectile->RigidBody.Transform.SetPosition(
            ysMath::Add(RigidBody.Transform.GetWorldPosition(), ysMath::LoadVector(0.0f, 1.0f, 0.0f))
        );
        projectile->RigidBody.SetAngularVelocity(ysMath::LoadVector(0.0f, 0.0f, ysMath::UniformRandom(20.0f)));

        const float angle = ysMath::UniformRandom() * ysMath::Constants::PI;
        const float velocity = ysMath::UniformRandom() * 10.0f + 5.0f;

        projectile->RigidBody.SetVelocity(
            ysMath::LoadVector(cos(angle) * velocity, sin(angle) * velocity, 0.0f));

        m_clock.reset();
    }
}

void c_adv::FruitBowl::getAssets(dbasic::AssetManager *am) {
    s_fruitBowl = am->BuildRenderSkeleton(
        &m_renderTransform, am->GetSceneObject("FruitBowl", ysObjectData::ObjectType::Empty));
    s_apple = am->GetModelAsset("Apple");
    s_banana = am->GetModelAsset("Banana");
    s_pear = am->GetModelAsset("Pear");
}
