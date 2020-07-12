#ifndef CEREAL_ADVENTURE_FRUIT_BOWL_H
#define CEREAL_ADVENTURE_FRUIT_BOWL_H

#include "game_object.h"

#include "clock.h"

namespace c_adv {

    class FruitBowl : public GameObject {
    public:
        FruitBowl();
        ~FruitBowl();

        virtual void initialize();

        virtual void render();
        virtual void process(float dt);

    protected:
        Clock m_clock;

        // Assets ----
    public:
        static void configureAssets(dbasic::AssetManager *am);

    protected:
        static dbasic::ModelAsset *m_fruitBowl;
    };

} /* namespace c_adv */

#endif /* CEREAL_ADVENTURE_FRUIT_BOWL_H */

