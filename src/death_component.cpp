#include "../include/death_component.h"

#include "../include/player.h"

const float c_adv::DeathComponent::AfterDeathSequenceLength = 5.0f;

c_adv::DeathComponent::DeathComponent() {
    m_player = nullptr;
}

c_adv::DeathComponent::~DeathComponent() {
    /* void */
}

void c_adv::DeathComponent::initialize(Player *player) {
    m_player = player;

    m_afterDeathCooldown.setCooldownPeriod(AfterDeathSequenceLength);
    m_afterDeathCooldown.disable();
}

void c_adv::DeathComponent::process(float dt) {
    m_afterDeathCooldown.update(dt);

    if (m_afterDeathCooldown.ready()) {
        m_afterDeathCooldown.disable();

        m_player->setDeletionFlag();
    }

    if (!m_player->isAlive() && !m_player->getDeletionFlag()) {
        m_afterDeathCooldown.trigger();
    }

    if (ysMath::GetY(m_player->RigidBody.Transform.GetWorldPosition()) < -5.0f) {
        m_player->setDeletionFlag();
    }
}
