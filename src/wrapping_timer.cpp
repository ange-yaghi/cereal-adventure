#include "../include/wrapping_timer.h"

c_adv::WrappingTimer::WrappingTimer() {
    m_timer = 0;
    m_period = 0;
}

c_adv::WrappingTimer::~WrappingTimer() {
    /* void */
}

void c_adv::WrappingTimer::update(float dt) {
    m_timer += dt;
    while (m_timer > m_period) m_timer -= m_period;
}
