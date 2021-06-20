#ifndef CEREAL_ADVENTURE_DEBUG_CAMERA_CONTROLLER_H
#define CEREAL_ADVENTURE_DEBUG_CAMERA_CONTROLLER_H

#include "game_object.h"

#include "spring_connector.h"

namespace c_adv {

    class DebugCameraController : public GameObject {
    public:
        DebugCameraController();
        ~DebugCameraController();

        virtual void initialize();

        virtual void render();
        virtual void process(float dt);

    protected:
        float m_cameraDistance;

        SpringConnector m_smoothCamera;
        SpringConnector m_smoothTarget;

    protected:
        SpringConnector m_smoothPosition;
    };

} /* namespace c_adv */

#endif /* CEREAL_ADVENTURE_DEBUG_CAMERA_CONTROLLER_H */
