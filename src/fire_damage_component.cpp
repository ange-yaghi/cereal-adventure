#include "../include/fire_damage_component.h"

#include "../include/player.h"
#include "../include/oven.h"

c_adv::FireDamageComponent::FireDamageComponent() {
    m_player = nullptr;
}

c_adv::FireDamageComponent::~FireDamageComponent() {
    /* void */
}

void c_adv::FireDamageComponent::initialize(Player *player) {
    m_fireDamageCooldown.setCooldownPeriod(1.0f);

    m_player = player;
}

void c_adv::FireDamageComponent::process(float dt) {
    m_fireDamageCooldown.update(dt);

    if (!m_fireDamageCooldown.ready()) return;

    ysVector collisionPosition;
    GameObject *object = getCollidingOven(collisionPosition);

    if (object != nullptr) {
        Oven *oven = static_cast<Oven *>(object);
        bool dangerousPosition = oven->isDangerous(collisionPosition);

        if (dangerousPosition && oven->isHot()) {
            m_player->RigidBody.AddImpulseWorldSpace(
                ysMath::LoadVector(0.0f, 5.0f, 0.0f), m_player->RigidBody.Transform.GetWorldPosition());

            m_player->takeDamage(1.0f);
            m_fireDamageCooldown.trigger();
        }
    }
}

c_adv::GameObject *c_adv::FireDamageComponent::getCollidingOven(ysVector &position) {
    int collisionCount = m_player->RigidBody.GetCollisionCount();
    for (int i = 0; i < collisionCount; ++i) {
        dphysics::Collision *collision = m_player->RigidBody.GetCollision(i);
        if (!collision->m_sensor && !collision->IsGhost()) {
            GameObject *object = m_player->getCollidingObject(collision);
            if (object->hasTag(GameObject::Tag::Oven)) {
                position = collision->m_position;
                return object;
            }
        }
    }

    return nullptr;
}
