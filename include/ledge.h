#ifndef CEREAL_ADVENTURE_LEDGE_H
#define CEREAL_ADVENTURE_LEDGE_H

#include "game_object.h"

namespace c_adv {

    class Ledge : public GameObject {
    public:
        Ledge();
        ~Ledge();

        virtual void initialize();

        virtual void render();

        // Assets ----
    public:
        void getAssets(dbasic::AssetManager *am);

    protected:
        static dbasic::ModelAsset *m_ledgeAsset;
    };

} /* namespace c_adv */

#endif /* CEREAL_ADVENTURE_LEDGE_H */
