#ifndef CEREAL_ADVENTURE_COUNTER_H
#define CEREAL_ADVENTURE_COUNTER_H

#include "game_object.h"

namespace c_adv {

    class Counter : public GameObject {
    public:
        Counter();
        ~Counter();

        virtual void initialize();

        virtual void render();

        // Assets ----
    public:
        void getAssets(dbasic::AssetManager *am);

    protected:
        static dbasic::ModelAsset *m_counterAsset; 
    };

} /* namespace c_adv */

#endif /* CEREAL_ADVENTURE_COUNTER_H */
