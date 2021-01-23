#include "../include/toast_projectile.h"

#include "../include/world.h"
#include "../include/colors.h"

dbasic::ModelAsset *c_adv::ToastProjectile::m_toastAsset = nullptr;

c_adv::ToastProjectile::ToastProjectile() {
    m_age = 0.0f;
    m_lifespan = 3.0f;
    m_timeAsGhost = 0.1f;
    m_dangerous = true;
}

c_adv::ToastProjectile::~ToastProjectile() {
    /* void */
}

void c_adv::ToastProjectile::initialize() {
    GameObject::initialize();

    addTag(Tag::Dynamic);
    addTag(Tag::Projectile);

    RigidBody.SetHint(dphysics::RigidBody::RigidBodyHint::Dynamic);
    RigidBody.SetInverseMass(1.0f / 0.1f);
    RigidBody.SetInverseInertiaTensor(RigidBody.GetRectangleTensor(0.07f, 0.442f));
    RigidBody.SetRequestsInformation(true);
    RigidBody.SetMaterial(GenericFrictionMaterial);

    dphysics::CollisionObject *bounds;
    RigidBody.CollisionGeometry.NewBoxObject(&bounds);
    bounds->SetMode(dphysics::CollisionObject::Mode::Fine);
    bounds->GetAsBox()->HalfHeight = 0.442f / 2;
    bounds->GetAsBox()->HalfWidth = 0.07f / 2;
    bounds->SetLayer(ProjectileCollisionLayer);
    bounds->SetCollidesWith(EmitterCollisionLayer, false);

    m_collisionPrimitive = bounds;

    m_positionDamper.setDampingTensor(ysMath::LoadVector(0.5f, 0.5f, 0.0f));
    m_positionDamper.setStiffnessTensor(ysMath::LoadVector(500.0f, 500.0f, 0.0f));
    m_positionDamper.setPosition(RigidBody.Transform.GetWorldPosition());

    m_rotationDamper.setDampingTensor(ysMath::LoadScalar(0.5f));
    m_rotationDamper.setStiffnessTensor(ysMath::LoadScalar(1000.0f));
    m_rotationDamper.setPosition(RigidBody.Transform.GetWorldOrientation());
}

void c_adv::ToastProjectile::render() {
    m_world->getShaders().ResetBrdfParameters();
    m_world->getShaders().SetBaseColor(DebugRed);

    m_world->getShaders().SetObjectTransform(m_renderTransform.GetWorldTransform());
    m_world->getShaders().ConfigureModel(1.0f, m_toastAsset);
    m_world->getEngine().DrawModel(m_world->getShaders().GetRegularFlags(), m_toastAsset);
}

void c_adv::ToastProjectile::process(float dt) {
    GameObject::process(dt);

    m_positionDamper.setTarget(RigidBody.Transform.GetWorldPosition());
    m_positionDamper.update(dt);

    m_rotationDamper.setTarget(RigidBody.Transform.GetWorldOrientation());
    m_rotationDamper.update(dt);

    m_renderTransform.SetPosition(m_positionDamper.getPosition());
    m_renderTransform.SetOrientation(m_rotationDamper.getPosition());

    RigidBody.AddForceWorldSpace(
        ysMath::LoadVector(0.0f, -15.0f / RigidBody.GetInverseMass(), 0.0f),
        RigidBody.Transform.GetWorldPosition());

    m_age += dt;
    m_collisionPrimitive->SetCollidesWith(EmitterCollisionLayer, m_age > m_timeAsGhost);

    checkDespawn();
}

void c_adv::ToastProjectile::checkDespawn() {
    ysVector position = RigidBody.Transform.GetWorldPosition();
    if (abs(ysMath::GetX(position)) > 500.0f) setDeletionFlag();
    if (abs(ysMath::GetY(position)) > 60.0f) setDeletionFlag();

    if (m_age >= m_lifespan) setDeletionFlag();
    checkHitObstacle();
}

void c_adv::ToastProjectile::checkHitObstacle() {
    int collisionCount = RigidBody.GetCollisionCount();
    for (int i = 0; i < collisionCount; ++i) {
        dphysics::Collision *col = RigidBody.GetCollision(i);
        GameObject *object = getCollidingObject(col);

        if (!col->m_sensor && !col->IsGhost()) {
            if (object->hasTag(Tag::Player)) {
                setDeletionFlag();
            }
            else {
                m_dangerous = false;
            }
        }
    }
}

void c_adv::ToastProjectile::configureAssets(dbasic::AssetManager *am) {
    m_toastAsset = am->GetModelAsset("Toast");
}
