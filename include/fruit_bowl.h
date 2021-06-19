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

        void setOrientation(const ysQuaternion &quaternion) { m_renderTransform.SetOrientation(quaternion); }

    protected:
        ysTransform m_renderTransform;
        Clock m_clock;

        // Assets ----
    public:
        void getAssets(dbasic::AssetManager *am);

    protected:
        static dbasic::RenderSkeleton *s_fruitBowl;
        static dbasic::ModelAsset *s_apple;
        static dbasic::ModelAsset *s_banana;
        static dbasic::ModelAsset *s_pear;
    };

} /* namespace c_adv */

#endif /* CEREAL_ADVENTURE_FRUIT_BOWL_H */

