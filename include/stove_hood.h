#ifndef CEREAL_ADVENTURE_STOVE_HOOD_H
#define CEREAL_ADVENTURE_STOVE_HOOD_H

#include "game_object.h"

#include "cooldown_timer.h"
#include "clock.h"

namespace c_adv {

    class StoveHood : public GameObject {
    public:
        StoveHood();
        ~StoveHood();

        virtual void initialize();

        virtual void render();
        virtual void process();

    protected:
        Clock m_clock;
        float m_currentPower;
        float m_maxPower;

        // Assets ----
    public:
        static void configureAssets(dbasic::AssetManager *am);

    protected:
        static dbasic::ModelAsset *m_stoveHoodAsset;
    };

} /* namespace c_adv */

#endif /* CEREAL_ADVENTURE_STOVE_HOOD_H */
