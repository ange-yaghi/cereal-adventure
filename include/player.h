#ifndef CEREAL_ADVENTURE_PLAYER_H
#define CEREAL_ADVENTURE_PLAYER_H

#include "game_object.h"

#include "cooldown_timer.h"

namespace c_adv {

    class Ledge;

    class Player : public GameObject {
    public:
        Player();
        ~Player();

        virtual void initialize();
        virtual void process();
        virtual void render();

        bool isOnSurface();
        bool isHanging();

        void updateGrip();
        void attemptGrip();
        void releaseGrip();

        ysVector getGripLocationLocal();
        ysVector getGripLocationWorld();

    protected:
        void updateMotion();
        void updateAnimation();

        void processImpactDamage();

    protected:
        float m_health;

    protected:
        ysAnimationActionBinding
            m_animLegsWalk,
            m_animArmsWalk,
            m_animLegsIdle,
            m_animArmsIdle;

        dbasic::RenderSkeleton *m_renderSkeleton;

        ysAnimationChannel *m_legsChannel;
        ysAnimationChannel *m_armsChannel;

        dphysics::LedgeLink *m_gripLink;
        GameObject *m_ledge;

        CooldownTimer m_gripCooldown;

        // Movement parameters
    protected:
        float m_ledgeGraspDistance;

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
