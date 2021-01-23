#include "../include/toaster.h"

#include "../include/world.h"
#include "../include/colors.h"
#include "../include/toast_projectile.h"

dbasic::ModelAsset *c_adv::Toaster::m_toasterAsset = nullptr;

const float c_adv::Toaster::ToastSpread = 0.03f;

c_adv::Toaster::Toaster() {
    /* void */
}

c_adv::Toaster::~Toaster() {
    /* void */
}

void c_adv::Toaster::initialize() {
    GameObject::initialize();

    RigidBody.SetHint(dphysics::RigidBody::RigidBodyHint::Dynamic);
    RigidBody.SetInverseMass(0.0f);

    dphysics::CollisionObject *bounds;
    RigidBody.CollisionGeometry.NewBoxObject(&bounds);
    bounds->SetMode(dphysics::CollisionObject::Mode::Fine);
    bounds->GetAsBox()->HalfHeight = 0.56f / 2;
    bounds->GetAsBox()->HalfWidth = 0.56f / 2;
    bounds->GetAsBox()->Orientation = ysMath::Constants::QuatIdentity;
    bounds->GetAsBox()->Position = ysMath::Constants::Zero;
    bounds->SetLayer(EmitterCollisionLayer);
    bounds->SetCollidesWith(ProjectileCollisionLayer, false);

    m_clock.setHighTime(1.0f);
    m_clock.setLowTime(10.0f);
}

void c_adv::Toaster::render() {
    m_world->getShaders().ResetBrdfParameters();
    m_world->getShaders().SetBaseColor(ObjectColor);

    m_world->getShaders().SetObjectTransform(RigidBody.Transform.GetWorldTransform());
    m_world->getShaders().ConfigureModel(1.0f, m_toasterAsset);
    m_world->getEngine().DrawModel(m_world->getShaders().GetRegularFlags(), m_toasterAsset);
}

void c_adv::Toaster::process(float dt) {
    m_clock.update(dt);

    if (m_clock.getState()) {
        ToastProjectile *projectile = getRealm()->spawn<ToastProjectile>();
        projectile->RigidBody.Transform.SetPosition(
            ysMath::Add(RigidBody.Transform.GetWorldPosition(), ysMath::LoadVector(0.1f, 0.0f, 0.0f))
        );

        const float angle = ToastSpread * (0.5f - ysMath::UniformRandom()) * ysMath::Constants::PI + ysMath::Constants::PI / 2;
        const float velocity = ysMath::UniformRandom() * 10.0f + 5.0f;
        const float angularVelocity = (0.5f - ysMath::UniformRandom()) * 5.0f;

        projectile->RigidBody.SetVelocity(
            ysMath::LoadVector(cos(angle) * velocity, sin(angle) * velocity, 0.0f));
        projectile->RigidBody.SetAngularVelocity(
            ysMath::LoadVector(0.0f, 0.0f, angularVelocity)
        );

        m_clock.reset();
    }
}

void c_adv::Toaster::configureAssets(dbasic::AssetManager *am) {
    m_toasterAsset = am->GetModelAsset("Toaster");
}
