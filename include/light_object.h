#ifndef CEREAL_ADVENTURE_LIGHT_OBJECT_H
#define CEREAL_ADVENTURE_LIGHT_OBJECT_H

#include "game_object.h"

namespace c_adv {

    class LightObject : public GameObject {
    public:
        LightObject();
        ~LightObject();

        virtual void initialize();

        virtual void render();
        virtual void process(float dt);

        void setAsset(dbasic::SceneObjectAsset *asset) { m_asset = asset; }
        dbasic::SceneObjectAsset *getAsset() const { return m_asset; }

    protected:
        dbasic::SceneObjectAsset *m_asset;
    };

} /* namespace c_adv */

#endif /* CEREAL_ADVENTURE_LIGHT_OBJECT_H */
