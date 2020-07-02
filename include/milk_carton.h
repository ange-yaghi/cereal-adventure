#ifndef CEREAL_ADVENTURE_MILK_CARTON_H
#define CEREAL_ADVENTURE_MILK_CARTON_H

#include "game_object.h"

namespace c_adv {

    class MilkCarton : public GameObject {
    public:
        MilkCarton();
        ~MilkCarton();

        virtual void initialize();

        virtual void render();
        virtual void process(float dt);

        // Assets ----
    public:
        static void configureAssets(dbasic::AssetManager *am);

    protected:
        static dbasic::ModelAsset *m_placeholderAsset;
    };

} /* namespace c_adv */

#endif /* CEREAL_ADVENTURE_MILK_CARTON_H */
