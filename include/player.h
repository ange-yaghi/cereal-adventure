#ifndef CEREAL_ADVENTURE_PLAYER_H
#define CEREAL_ADVENTURE_PLAYER_H

#include "game_object.h"

#include "cooldown_timer.h"
#include "spring_connector.h"
#include "fire_damage_component.h"
#include "projectile_damage_component.h"
#include "walk_component.h"
#include "clock.h"
#include "player_arms_fsm.h"

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

    public:
        Player();
        ~Player();

        virtual void initialize();
        virtual void process(float dt);
        virtual void render();

        bool isHurt();
        bool isHanging();
        bool isGraspReady() const { return m_graspReady; }
        bool isReadyToMove() const { return m_movementCooldown.ready(); }
        bool isLaunching() const { return m_launching; }
        bool isCurrentArmActionComplete() const { return m_armsChannel->IsActionComplete(); }

        void updateGrip();
        void attemptGrip();
        GameObject *findGrip();
        void releaseGrip();

        ysVector getGripLocationLocal();
        ysVector getGripLocationWorld();

        void takeDamage(float damage);

        ysAnimationActionBinding *getArmsAction(PlayerArmsFsm::State state);

    protected:
        void updateMotion(float dt);
        void updateAnimation(float dt);
        void legsAnimationFsm();
        void rotationAnimationFsm();
        void armsAnimationFsm();
        void updateSoundEffects();

        void processImpactDamage();

    protected:
        float m_health;
        float m_terminalFallVelocity;
        float m_fallDamageThreshold;
        float m_fallDamageMultiplier;
        
        bool m_graspReady;
        bool m_launching;

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
            m_animLegsFastFalling,
            m_animArmsLaunch;

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

        // Sound effect internals
        float m_lastRunPlayhead;

        // Components
    public:
        FireDamageComponent m_fireDamageComponent;
        ProjectileDamageComponent m_projectileDamageComponent;
        WalkComponent m_walkComponent;

        PlayerArmsFsm m_armsFsm;

        LegsState m_legsState;

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
            *AnimLegsFastFalling,
            *AnimArmsLaunch;
        
        static dbasic::AudioAsset
            *AudioFootstep01,
            *AudioFootstep02,
            *AudioFootstep03,
            *AudioFootstep04,
            *DamageImpact;

        static dbasic::SceneObjectAsset *CharacterRoot;
        static dbasic::ModelAsset *Sphere;
        static dbasic::Material *Material;

        // Debug
    protected:
        std::string m_lastMissReason;
        CooldownTimer m_debugDamageIndicatorCooldown;
        Clock m_debugDamageFlicker;
    };

} /* namespace c_adv */

#endif /* CEREAL_ADVENTURE_PLAYER_H */
