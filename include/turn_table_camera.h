#ifndef CEREAL_ADVENTURE_TURN_TABLE_CAMERA_H
#define CEREAL_ADVENTURE_TURN_TABLE_CAMERA_H

#include "game_object.h"

#include "spring_connector.h"

namespace c_adv {

    class TurnTableCamera : public GameObject {
    public:
        TurnTableCamera();
        ~TurnTableCamera();

        virtual void initialize();

        virtual void render();
        virtual void process(float dt);

    protected:
        float m_turnTableAngle;
        float m_verticalAngle;
        float m_distance;
        float m_targetHeight;

    protected:
        SpringConnector m_smoothPosition;
        SpringConnector m_smoothTarget;
    };

} /* namespace c_adv */

#endif /* CEREAL_ADVENTURE_TURN_TABLE_CAMERA_H */
