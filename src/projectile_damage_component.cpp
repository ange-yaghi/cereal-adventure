#include "../include/projectile_damage_component.h"

#include "../include/player.h"

c_adv::ProjectileDamageComponent::ProjectileDamageComponent() {
    m_player = nullptr;
}

c_adv::ProjectileDamageComponent::~ProjectileDamageComponent() {
    /* void */
}

void c_adv::ProjectileDamageComponent::initialize(Player *player) {
    m_player = player;
}

void c_adv::ProjectileDamageComponent::process(float dt) {
    int collisionCount = m_player->RigidBody.GetCollisionCount();
    for (int i = 0; i < collisionCount; ++i) {
        dphysics::Collision *collision = m_player->RigidBody.GetCollision(i);
        if (collision->m_sensor || collision->IsGhost()) continue;

        GameObject *object = m_player->getCollidingObject(collision);
        if (!object->hasTag(GameObject::Tag::Projectile)) continue;
        if (!object->isDangerous()) continue;

        ysVector collisionVelocity = ysMath::Mask(m_player->getCollisionVelocity(collision), ysMath::Constants::MaskKeepX);
        collisionVelocity = ysMath::Mul(collisionVelocity, ysMath::LoadScalar(1.0f));

        m_player->RigidBody.AddImpulseWorldSpace(
            collisionVelocity,
            m_player->RigidBody.Transform.GetWorldPosition()
        );
        m_player->takeDamage(1.0f);
    }
}
