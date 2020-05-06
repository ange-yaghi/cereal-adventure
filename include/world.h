#ifndef CEREAL_ADVENTURE_WORLD_H
#define CEREAL_ADVENTURE_WORLD_H

#include "aabb.h"

#include "delta.h"
#include "realm.h"
#include "spring_connector.h"

#include <vector>

namespace c_adv {

    class World {
    public:
        World();
        ~World();

        void initialize(void *instance, ysContextObject::DEVICE_API api);
        void initialSpawn();
        void run();
        void frameTick();

        dbasic::DeltaEngine &getEngine() { return m_engine; }
        dbasic::AssetManager &getAssetManager() { return m_assetManager; }

        AABB getCameraExtents() const;

        void render();
        void process();

        void generateLevel(dbasic::RenderSkeleton *hierarchy);

        template <typename T>
        T *newRealm() {
            void *buffer = _aligned_malloc(sizeof(T), 16);
            T *newObject = new (buffer) T;
            newObject->setWorld(this);

            m_realms.push_back(newObject);

            return newObject;
        }

    protected:
        void updateRealms();

        std::vector<Realm *> m_realms;

        Realm *m_mainRealm;
        GameObject *m_focus;

        SpringConnector m_smoothCamera;
        SpringConnector m_smoothTarget;

    protected:
        dbasic::DeltaEngine m_engine;
        dbasic::AssetManager m_assetManager;

        dbasic::Path m_assetPath;
    };

} /* namespace c_adv */

#endif /* CEREAL_ADVENTURE_GAME_OBJECT_H */
