#ifndef CEREAL_ADVENTURE_SHELVES_H
#define CEREAL_ADVENTURE_SHELVES_H

#include "game_object.h"

namespace c_adv {

    class Shelves : public GameObject {
    public:
        Shelves();
        ~Shelves();

        virtual void initialize();

        virtual void render();

        // Assets ----
    public:
        void getAssets(dbasic::AssetManager *am);

    protected:
        static dbasic::ModelAsset *m_shelvesAsset;
    };

} /* namespace c_adv */

#endif /* CEREAL_ADVENTURE_SHELVES_H */
