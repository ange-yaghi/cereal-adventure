#ifndef CEREAL_ADVENTURE_CEILING_LIGHT_SOURCE_H
#define CEREAL_ADVENTURE_CEILING_LIGHT_SOURCE_H

#include "game_object.h"

namespace c_adv {

    class CeilingLightSource : public GameObject {
    public:
        CeilingLightSource();
        ~CeilingLightSource();

        virtual void initialize();

        virtual void render();
        virtual void process();
    };

} /* namespace c_adv */

#endif /* CEREAL_ADVENTURE_CEILING_LIGHT_SOURCE_H */
