#include "../include/wave_generator.h"

#include "../include/delta.h"

#include <cmath>

c_adv::WaveGenerator::WaveGenerator() {
    m_timer = 0;
    m_f = 0;
    m_period = 0;
}

c_adv::WaveGenerator::~WaveGenerator() {
    /* void */
}

void c_adv::WaveGenerator::update(float dt) {
    m_timer += dt;
    while (m_timer > m_period) m_timer -= m_period;

    m_f = std::sin((m_timer / m_period) * ysMath::Constants::TWO_PI);
}
