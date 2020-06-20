#ifndef CEREAL_ADVENTURE_OVEN_H
#define CEREAL_ADVENTURE_OVEN_H

#include "game_object.h"

namespace c_adv {

    class Oven : public GameObject {
    public:
        Oven();
        ~Oven();

        virtual void initialize();

        virtual void render();
        virtual void process();

        // Assets ----
    public:
        static void configureAssets(dbasic::AssetManager *am);

    protected:
        static dbasic::ModelAsset *m_ovenAsset;
    };

} /* namespace c_adv */

#endif /* CEREAL_ADVENTURE_OVEN_H */

