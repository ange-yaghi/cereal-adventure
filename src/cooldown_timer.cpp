#include "../include/cooldown_timer.h"

c_adv::CooldownTimer::CooldownTimer() {
    m_cooldownPeriod = 0.0f;
    m_triggered = false;
    m_timer = 0.0f;
    m_locked = false;
}

c_adv::CooldownTimer::~CooldownTimer() {
    /* void */
}

void c_adv::CooldownTimer::trigger() {
    m_triggered = true;
    m_timer = 0.0f;
}

void c_adv::CooldownTimer::update(float dt) {
    if (!m_triggered) return;

    m_timer += dt;

    if (m_timer >= m_cooldownPeriod) m_triggered = false;
}
