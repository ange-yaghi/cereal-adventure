#ifndef CEREAL_ADVENTURE_WINDOW_LIGHT_SOURCE_H
#define CEREAL_ADVENTURE_WINDOW_LIGHT_SOURCE_H

#include "game_object.h"

namespace c_adv {

    class WindowLightSource : public GameObject {
    public:
        WindowLightSource();
        ~WindowLightSource();

        virtual void initialize();

        virtual void render();
    };

} /* namespace c_adv */

#endif /* CEREAL_ADVENTURE_WINDOW_LIGHT_SOURCE_H */
