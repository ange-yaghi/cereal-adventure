#ifndef CEREAL_ADVENTURE_FRIDGE_H
#define CEREAL_ADVENTURE_FRIDGE_H

#include "game_object.h"

namespace c_adv {

    class Fridge : public GameObject {
    public:
        Fridge();
        ~Fridge();

        virtual void initialize();

        virtual void render();

        // Assets ----
    public:
        void getAssets(dbasic::AssetManager *am);

    protected:
        static dbasic::ModelAsset *m_fridgeAsset;
    };

} /* namespace c_adv */

#endif /* CEREAL_ADVENTURE_FRIDGE_H */
