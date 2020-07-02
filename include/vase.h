#ifndef CEREAL_ADVENTURE_VASE_H
#define CEREAL_ADVENTURE_VASE_H

#include "game_object.h"

namespace c_adv {

    class Vase : public GameObject {
    public:
        Vase();
        ~Vase();

        virtual void initialize();

        virtual void render();
        virtual void process(float dt);

        // Assets ----
    public:
        static void configureAssets(dbasic::AssetManager *am);

    protected:
        static dbasic::ModelAsset *m_vaseAsset;
    };

} /* namespace c_adv */

#endif /* CEREAL_ADVENTURE_VASE_H */
