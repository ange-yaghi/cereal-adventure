#ifndef CEREAL_ADVENTURE_TOASTER_H
#define CEREAL_ADVENTURE_TOASTER_H

#include "game_object.h"

#include "clock.h"

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

        // Assets ----
    public:
        static void configureAssets(dbasic::AssetManager *am);

    protected:
        static dbasic::ModelAsset *m_toasterAsset;
    };

} /* namespace c_adv */

#endif /* CEREAL_ADVENTURE_TOASTER_H */
