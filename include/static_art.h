#ifndef CEREAL_ADVENTURE_STATIC_ART_H
#define CEREAL_ADVENTURE_STATIC_ART_H

#include "game_object.h"

namespace c_adv {

    class StaticArt : public GameObject {
    public:
        StaticArt();
        ~StaticArt();

        virtual void initialize();

        virtual void render();
        virtual void process();

        void setAsset(dbasic::ModelAsset *asset) { m_asset = asset; }
        dbasic::ModelAsset *getAsset() const { return m_asset; }

    protected:
        dbasic::ModelAsset *m_asset;
    };

} /* namespace c_adv */

#endif /* CEREAL_ADVENTURE_STATIC_ART_H */
