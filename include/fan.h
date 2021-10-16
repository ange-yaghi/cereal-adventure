#ifndef CEREAL_ADVENTURE_FAN_H
#define CEREAL_ADVENTURE_FAN_H

#include "game_object.h"

#include "cooldown_timer.h"
#include "clock.h"

namespace c_adv {

    class Fan : public GameObject {
    public:
        Fan();
        virtual ~Fan();

        virtual void initialize();

        virtual void render();
        virtual void process(float dt);

        // Assets ----
    public:
        void getAssets(dbasic::AssetManager *am);

    protected:
        static dbasic::ModelAsset *m_fanAsset;
    };

} /* namespace c_adv */

#endif /* CEREAL_ADVENTURE_FAN_H */

