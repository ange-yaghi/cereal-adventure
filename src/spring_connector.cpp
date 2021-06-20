#include "../include/spring_connector.h"

c_adv::SpringConnector::SpringConnector() {
    m_stiffnessTensor = ysMath::Constants::Zero;
    m_dampingTensor = ysMath::Constants::Zero;
    m_position = ysMath::Constants::Zero;
    m_target = ysMath::Constants::Zero;

    m_acceleration = ysMath::Constants::Zero;
    m_velocity = ysMath::Constants::Zero;
}

c_adv::SpringConnector::~SpringConnector() {
    /* void */
}

void c_adv::SpringConnector::update(float dt) {
    const ysVector d = ysMath::Sub(m_target, m_position);
    const ysVector f = ysMath::Mul(d, m_stiffnessTensor);
    const ysVector dt_v = ysMath::LoadScalar(dt);

    m_acceleration = f;

    m_velocity = ysMath::Add(m_velocity, ysMath::Mul(dt_v, m_acceleration));
    m_position = ysMath::Add(m_position, ysMath::Mul(dt_v, m_velocity));

    m_velocity = ysMath::Mul(m_velocity, ysMath::Sub(ysMath::Constants::One, m_dampingTensor));
}
