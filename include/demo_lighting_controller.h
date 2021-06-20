#ifndef CEREAL_ADVENTURE_DEMO_LIGHTING_CONTROLLER_H
#define CEREAL_ADVENTURE_DEMO_LIGHTING_CONTROLLER_H

#include "game_object.h"

#include "spring_connector.h"

namespace c_adv {

    class DemoLightingController : public GameObject {
    public:
        DemoLightingController();
        ~DemoLightingController();

        virtual void initialize();

        virtual void render();
        virtual void process(float dt);
    };

} /* namespace c_adv */

#endif /* CEREAL_ADVENTURE_DEMO_LIGHTING_CONTROLLER_H */
