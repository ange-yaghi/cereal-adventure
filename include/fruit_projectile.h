#ifndef CEREAL_ADVENTURE_FRUIT_PROJECTILE_H
#define CEREAL_ADVENTURE_FRUIT_PROJECTILE_H

#include "game_object.h"

#include "spring_connector.h"

namespace c_adv {

    class FruitProjectile : public GameObject {
    public:
        FruitProjectile();
        ~FruitProjectile();

        virtual void initialize();

        virtual void render();
        virtual void process(float dt);

        void setRadius(float radius) { m_radius = radius; }
        float getRadius() const { return m_radius; }

        void setAge(float age) { m_age = age; }
        float getAge() const { return m_age; }

        void setLifespan(float lifespan) { m_lifespan = lifespan; }
        float getLifespan() const { return m_lifespan; }

        void setAsset(dbasic::ModelAsset *asset) { m_asset = asset; }
        dbasic::ModelAsset *getAsset() const { return m_asset; }

        virtual bool isDangerous() { return true; }

    protected:
        ysTransform m_renderTransform;

        SpringConnector m_positionDamper;
        SpringConnector m_rotationDamper;

        void checkDespawn();
        bool checkHitObstacle();

    protected:
        float m_radius;
        float m_age;
        float m_lifespan;

        dbasic::ModelAsset *m_asset;

        // Assets ----
    public:
        void getAssets(dbasic::AssetManager *am);

    protected:
        static dbasic::ModelAsset *m_fruitAsset;
    };

} /* namespace c_adv */

#endif /* CEREAL_ADVENTURE_FRUIT_PROJECTILE_H */

