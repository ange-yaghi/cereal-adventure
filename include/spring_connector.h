#ifndef CEREAL_ADVENTURE_SPRING_CONNECTOR_H
#define CEREAL_ADVENTURE_SPRING_CONNECTOR_H

#include "delta.h"

namespace c_adv {

    class SpringConnector {
    public:
        SpringConnector();
        ~SpringConnector();

        void update(float dt);

        ysVector getPosition() const { return m_position; }
        void setPosition(const ysVector &position) { m_position = position; }

        ysVector getTarget() const { return m_target; }
        void setTarget(const ysVector &target) { m_target = target; }

        void setStiffnessTensor(const ysVector &stiffnessTensor) { m_stiffnessTensor = stiffnessTensor; }
        ysVector getStiffnessTensor() const { return m_stiffnessTensor; }

        void setDampingTensor(const ysVector &damping) { m_dampingTensor = damping; }
        ysVector getDampingTensor() const { return m_dampingTensor; }

    protected:
        ysVector m_position;
        ysVector m_target;
        ysVector m_stiffnessTensor;
        ysVector m_dampingTensor;

        // Internals
    protected:
        ysVector m_force;
        ysVector m_acceleration;
        ysVector m_velocity;
    };

} /* namespace c_adv */

#endif /* CEREAL_ADVENTURE_SPRING_CONNECTOR_H */
