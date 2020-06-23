#ifndef CEREAL_ADVENTURE_CABINET_H
#define CEREAL_ADVENTURE_CABINET_H

#include "game_object.h"

namespace c_adv {

    class Cabinet : public GameObject {
    public:
        Cabinet();
        ~Cabinet();

        virtual void initialize();

        virtual void render();
        virtual void process();

        // Assets ----
    public:
        static void configureAssets(dbasic::AssetManager *am);

    protected:
        static dbasic::ModelAsset *m_cabinetAsset;
    };

} /* namespace c_adv */

#endif /* CEREAL_ADVENTURE_CABINET_H */
