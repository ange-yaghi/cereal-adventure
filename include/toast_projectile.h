#ifndef CEREAL_ADVENTURE_TOAST_PROJECTILE_H
#define CEREAL_ADVENTURE_TOAST_PROJECTILE_H

#include "game_object.h"

#include "spring_connector.h"

namespace c_adv {

    class ToastProjectile : public GameObject {
    public:
        ToastProjectile();
        ~ToastProjectile();

        virtual void initialize();

        virtual void render();
        virtual void process(float dt);

        void setAge(float age) { m_age = age; }
        float getAge() const { return m_age; }

        void setLifespan(float lifespan) { m_lifespan = lifespan; }
        float getLifespan() const { return m_lifespan; }

        virtual bool isDangerous() { return m_dangerous; }

    protected:
        ysTransform m_renderTransform;

        SpringConnector m_positionDamper;
        SpringConnector m_rotationDamper;

        void checkDespawn();
        void checkHitObstacle();

    protected:
        float m_age;
        float m_lifespan;
        float m_timeAsGhost;
        bool m_dangerous;

        dphysics::CollisionObject *m_collisionPrimitive;

        // Assets ----
    public:
        static void configureAssets(dbasic::AssetManager *am);

    protected:
        static dbasic::ModelAsset *m_toastAsset;
    };

} /* namespace c_adv */

#endif /* CEREAL_ADVENTURE_TOAST_PROJECTILE_H */


