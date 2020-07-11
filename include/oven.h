#ifndef CEREAL_ADVENTURE_OVEN_H
#define CEREAL_ADVENTURE_OVEN_H

#include "game_object.h"

#include "clock.h"

namespace c_adv {

    class Oven : public GameObject {
    public:
        static constexpr float HalfHeight = 1.0f;
        static constexpr float HalfWidth = 1.0f;

    public:
        Oven();
        ~Oven();

        virtual void initialize();

        virtual void render();
        virtual void process(float dt);

        bool isHot();
        bool isDangerous(const ysVector &p_world);

    protected:
        Clock m_clock;

        // Assets ----
    public:
        static void configureAssets(dbasic::AssetManager *am);

    protected:
        static dbasic::ModelAsset *m_ovenAsset;
    };

} /* namespace c_adv */

#endif /* CEREAL_ADVENTURE_OVEN_H */
