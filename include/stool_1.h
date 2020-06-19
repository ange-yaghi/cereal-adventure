#ifndef CEREAL_ADVENTURE_STOOL_1_H
#define CEREAL_ADVENTURE_STOOL_1_H

#include "game_object.h"

namespace c_adv {

    class Stool_1 : public GameObject {
    public:
        Stool_1();
        ~Stool_1();

        virtual void initialize();

        virtual void render();
        virtual void process();

        // Assets ----
    public:
        static void configureAssets(dbasic::AssetManager *am);

    protected:
        static dbasic::ModelAsset *m_stoolAsset;
    };

} /* namespace c_adv */

#endif /* CEREAL_ADVENTURE_STOOL_1_H */

