#ifndef CEREAL_ADVENTURE_SCENE_LIGHTING_CONTROLLER_H
#define CEREAL_ADVENTURE_SCENE_LIGHTING_CONTROLLER_H

#include "game_object.h"

#include "spring_connector.h"

namespace c_adv {

    class SceneLightingController : public GameObject {
    public:
        SceneLightingController();
        ~SceneLightingController();

        virtual void initialize();

        virtual void render();
        virtual void process(float dt);
    };

} /* namespace c_adv */

#endif /* CEREAL_ADVENTURE_SCENE_LIGHTING_CONTROLLER_H */
