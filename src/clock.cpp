#include "../include/clock.h"

c_adv::Clock::Clock() {
    m_t = 0.0f;

    m_hT = 0.0f;
    m_lT = 0.0f;

    m_enabled = true;
    m_state = false;
    m_inverted = false;
}

c_adv::Clock::~Clock() {
    /* void */
}

void c_adv::Clock::reset() {
    m_t = 0.0f;
    m_state = false;
}

void c_adv::Clock::update(float dt) {
    if (m_enabled) {
        m_t += dt;
    }

    if (m_t >= m_hT + m_lT) {
        m_state = false;
        m_t -= (m_hT + m_lT);
    }
    else if (m_t >= m_lT) {
        m_state = true;
    }
    else {
        m_state = false;
    }
}
