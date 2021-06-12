#ifndef CEREAL_ADVENTURE_TOASTER_H
#define CEREAL_ADVENTURE_TOASTER_H

#include "game_object.h"

#include "clock.h"
#include "cooldown_timer.h"

namespace c_adv {

    class Toaster : public GameObject {
    public:
        static const float ToastSpread;

    public:
        Toaster();
        ~Toaster();

        virtual void initialize();

        virtual void render();
        virtual void process(float dt);

    protected:
        Clock m_clock;
        CooldownTimer m_warmupTimer;

        // Assets ----
    public:
        void getAssets(dbasic::AssetManager *am);

    protected:
        static dbasic::ModelAsset *m_toasterAsset;
        static dbasic::AudioAsset *m_launchAudio;
    };

} /* namespace c_adv */

#endif /* CEREAL_ADVENTURE_TOASTER_H */
