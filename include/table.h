#ifndef CEREAL_ADVENTURE_TABLE_H
#define CEREAL_ADVENTURE_TABLE_H

#include "game_object.h"

namespace c_adv {

    class Table : public GameObject {
    public:
        Table();
        ~Table();

        virtual void initialize();

        virtual void render();

        // Assets ----
    public:
        void getAssets(dbasic::AssetManager *am);

    protected:
        static dbasic::ModelAsset *m_tableAsset;
    };

} /* namespace c_adv */

#endif /* CEREAL_ADVENTURE_TABLE_H */

