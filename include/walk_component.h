#ifndef CEREAL_ADVENTURE_WALK_COMPONENT_H
#define CEREAL_ADVENTURE_WALK_COMPONENT_H

#include "../include/delta.h"

namespace c_adv {

    class GameObject;

    class WalkComponent {
        friend GameObject;

    public:
        WalkComponent();
        ~WalkComponent();

        void initialize(GameObject *object);

        void process(float dt);

        bool isOnSurface();

        void setMaxRunVelocity(float maxRunVelocity) { m_maxRunVelocity = maxRunVelocity; }
        float getMaxRunVelocity() const { return m_maxRunVelocity; }

        float getRunVelocity() const { return m_runVelocity; }

        void setAcceleration(float acceleration) { m_acceleration = acceleration; }
        float getAcceleration() const { return m_acceleration; }

        void setWalkingLeft(bool walkingLeft) { m_walkingLeft = walkingLeft; }
        bool getWalkingLeft() const { return m_walkingLeft; }

        void setWalkingRight(bool walkingRight) { m_walkingRight = walkingRight; }
        bool getWalkingRight() const { return m_walkingRight; }

        // Inputs
    protected:
        GameObject *m_object;
        float m_maxRunVelocity;
        float m_acceleration;
        bool m_walkingLeft;
        bool m_walkingRight;

        // State
    protected:
        float m_groundDebounceTimer;
        GameObject *m_currentSurface;
        ysVector m_contactPoint;
        float m_runVelocity;
        float m_startVelocity;

        void checkSurfaceExists();
        void changeSurface(GameObject *surface);
    };

} /* namespace c_adv */

#endif /* CEREAL_ADVENTURE_WALK_COMPONENT_H */
