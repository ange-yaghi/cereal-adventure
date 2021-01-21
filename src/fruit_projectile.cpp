#include "../include/fruit_projectile.h"

#include "../include/world.h"
#include "../include/colors.h"

dbasic::ModelAsset *c_adv::FruitProjectile::m_fruitAsset = nullptr;

c_adv::FruitProjectile::FruitProjectile() {
    m_radius = 0.57f / 2;
    m_age = 0.0f;
    m_lifespan = 10.0f;
}

c_adv::FruitProjectile::~FruitProjectile() {
    /* void */
}

void c_adv::FruitProjectile::initialize() {
    GameObject::initialize();
    
    addTag(Tag::Dynamic);
    addTag(Tag::Projectile);

    RigidBody.SetHint(dphysics::RigidBody::RigidBodyHint::Dynamic);
    RigidBody.SetInverseMass(1.0f / 0.01f);
    RigidBody.SetInverseInertiaTensor(RigidBody.GetRectangleTensor(m_radius * 2, m_radius * 2));
    RigidBody.SetRequestsInformation(true);
    RigidBody.SetMaterial(GenericFrictionMaterial);

    dphysics::CollisionObject *bounds;
    RigidBody.CollisionGeometry.NewCircleObject(&bounds);
    bounds->SetMode(dphysics::CollisionObject::Mode::Fine);
    bounds->GetAsCircle()->Radius = m_radius;

    m_positionDamper.setDampingTensor(ysMath::LoadVector(0.5f, 0.5f, 0.0f));
    m_positionDamper.setStiffnessTensor(ysMath::LoadVector(500.0f, 500.0f, 0.0f));
    m_positionDamper.setPosition(RigidBody.Transform.GetWorldPosition());

    m_rotationDamper.setDampingTensor(ysMath::LoadScalar(0.5f));
    m_rotationDamper.setStiffnessTensor(ysMath::LoadScalar(1000.0f));
    m_rotationDamper.setPosition(RigidBody.Transform.GetWorldOrientation());
}

void c_adv::FruitProjectile::render() {
    m_world->getShaders().ResetBrdfParameters();
    m_world->getShaders().SetBaseColor(Black);

    m_world->getShaders().SetObjectTransform(m_renderTransform.GetWorldTransform());
    m_world->getShaders().ConfigureModel(1.0f);
    m_world->getEngine().DrawModel(m_world->getShaders().GetRegularFlags(), m_fruitAsset, nullptr);
}

void c_adv::FruitProjectile::process(float dt) {
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

    checkDespawn();
}

void c_adv::FruitProjectile::checkDespawn() {
    ysVector position = RigidBody.Transform.GetWorldPosition();
    if (abs(ysMath::GetX(position)) > 500.0f) setDeletionFlag();
    if (abs(ysMath::GetY(position)) > 60.0f) setDeletionFlag();

    if (m_age >= m_lifespan) setDeletionFlag();
    if (checkHitObstacle()) setDeletionFlag();
}

bool c_adv::FruitProjectile::checkHitObstacle() {
    int collisionCount = RigidBody.GetCollisionCount();
    for (int i = 0; i < collisionCount; ++i) {
        dphysics::Collision *col = RigidBody.GetCollision(i);
        GameObject *object = getCollidingObject(col);

        if (!col->m_sensor && !col->IsGhost()) {
            if (!object->hasTag(Tag::Projectile)) {
                return true;
            }
        }
    }

    return false;
}

void c_adv::FruitProjectile::configureAssets(dbasic::AssetManager *am) {
    m_fruitAsset = am->GetModelAsset("FruitProjectile");
}
