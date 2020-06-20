#ifndef CEREAL_ADVENTURE_SINGLE_SHELF_H
#define CEREAL_ADVENTURE_SINGLE_SHELF_H

#include "game_object.h"

namespace c_adv {

    class SingleShelf : public GameObject {
    public:
        SingleShelf();
        ~SingleShelf();

        virtual void initialize();

        virtual void render();
        virtual void process();

        // Assets ----
    public:
        static void configureAssets(dbasic::AssetManager *am);

    protected:
        static dbasic::ModelAsset *m_singleShelfAsset;
    };

} /* namespace c_adv */

#endif /* CEREAL_ADVENTURE_SINGLE_SHELF_H */
