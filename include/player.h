#ifndef CEREAL_ADVENTURE_PLAYER_H
#define CEREAL_ADVENTURE_PLAYER_H

#include "game_object.h"

#include "cooldown_timer.h"
#include "spring_connector.h"

namespace c_adv {

    class Ledge;

    class Player : public GameObject {
    public:
        enum class Direction {
            Forward,
            Back
        };

        enum class LegsState {
            Running,
            Idle,
            Falling,
            FastFalling,
            Hanging,
            ImpactDamage,
            Undefined
        };

        enum class ArmsState {
            Idle,
            Running,
            Hanging,
            ImpactDamage,
            Undefined
        };

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
        void legsAnimationFsm();
        void rotationAnimationFsm();
        void armsAnimationFsm();
        void updateSoundEffects();

        void processImpactDamage();

    protected:
        float m_health;
        float m_fallDamageThreshold;
        float m_fallDamageMultiplier;
        bool m_graspReady;

    protected:
        ysAnimationActionBinding
            m_animLegsWalk,
            m_animArmsWalk,
            m_animLegsIdle,
            m_animArmsIdle,
            m_animLegsTurnBack,
            m_animLegsTurnForward,
            m_animLegsFalling,
            m_animLegsHanging,
            m_animArmsHanging,
            m_animArmsDamageLanding,
            m_animLegsDamageLanding,
            m_animLegsFastFalling;

        dbasic::RenderSkeleton *m_renderSkeleton;
        SpringConnector m_springConnector;
        ysTransform m_renderTransform;

        ysAnimationChannel *m_legsChannel;
        ysAnimationChannel *m_armsChannel;
        ysAnimationChannel *m_rotationChannel;

        dphysics::LedgeLink *m_gripLink;
        GameObject *m_ledge;

        CooldownTimer m_gripCooldown;
        CooldownTimer m_movementCooldown;

        Direction m_direction;
        Direction m_nextDirection;
        LegsState m_legsState;
        ArmsState m_armsState;

        // Whether impact damage was taken in the last frame
        bool m_impactDamage;

        // Sound effect internals
        float m_lastRunPlayhead;

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
            *AnimArmsIdle,
            *AnimTurnBack,
            *AnimTurnForward,
            *AnimLegsFalling,
            *AnimLegsHanging,
            *AnimArmsHanging,
            *AnimArmsDamageLanding,
            *AnimLegsDamageLanding,
            *AnimLegsFastFalling;
        
        static dbasic::AudioAsset
            *AudioFootstep01,
            *AudioFootstep02,
            *AudioFootstep03,
            *AudioFootstep04,
            *DamageImpact;

        static dbasic::SceneObjectAsset *CharacterRoot;
    };


} /* namespace c_adv */

#endif /* CEREAL_ADVENTURE_PLAYER_H */
