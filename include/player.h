#ifndef CEREAL_ADVENTURE_PLAYER_H
#define CEREAL_ADVENTURE_PLAYER_H

#include "game_object.h"

namespace c_adv {

    class Player : public GameObject {
    public:
        Player();
        ~Player();

        virtual void initialize();
        virtual void process();
        virtual void render();

    protected:
        void updateMotion();
        void updateAnimation();

    protected:
        ysAnimationActionBinding
            m_animLegsWalk,
            m_animArmsWalk,
            m_animLegsIdle,
            m_animArmsIdle;

        dbasic::RenderSkeleton *m_renderSkeleton;

        ysAnimationChannel *m_legsChannel;
        ysAnimationChannel *m_armsChannel;

        // Assets ----
    public:
        static void configureAssets(dbasic::AssetManager *am);

    protected:
        static ysAnimationAction
            *AnimLegsWalk,
            *AnimArmsWalk,
            *AnimLegsIdle,
            *AnimArmsIdle;

        static dbasic::SceneObjectAsset *CharacterRoot;
    };


} /* namespace c_adv */

#endif /* CEREAL_ADVENTURE_PLAYER_H */
