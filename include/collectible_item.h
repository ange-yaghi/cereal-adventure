#ifndef CEREAL_ADVENTURE_COLLECTIBLE_ITEM_H
#define CEREAL_ADVENTURE_COLLECTIBLE_ITEM_H

#include "game_object.h"

#include "wave_generator.h"
#include "wrapping_timer.h"
#include "cooldown_timer.h"

namespace c_adv {

    class CollectibleItem : public GameObject {
    public:
        CollectibleItem();
        ~CollectibleItem();

        virtual void initialize();

        virtual void render();
        virtual void process(float dt);

        void setAsset(dbasic::ModelAsset *asset) { m_asset = asset; }
        dbasic::ModelAsset *getAsset() const { return m_asset; }

        void setGlowColor(const ysVector &color) { m_glowColor = color; }
        ysVector getGlowColor() const { return m_glowColor; }

    protected:
        void collidingWithPlayerCheck();

    protected:
        ysVector m_glowColor;

        WaveGenerator m_emissionWave;
        WaveGenerator m_floatWave;
        WaveGenerator m_rAxis_x;
        WaveGenerator m_rAxis_z;
        WrappingTimer m_spinTimer;

        CooldownTimer m_collectionTimer;

        dbasic::ModelAsset *m_asset;
    };

} /* namespace c_adv */

#endif /* CEREAL_ADVENTURE_COLLECTIBLE_ITEM_H */
