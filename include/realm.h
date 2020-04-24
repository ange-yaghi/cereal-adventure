#ifndef CEREAL_ADVENTURE_REALM_H
#define CEREAL_ADVENTURE_REALM_H

#include "delta.h"

#include <vector>
#include <queue>

namespace c_adv {

    class GameObject;
    class World;
    class Hole;

    class Realm {
    public:
        Realm();
        ~Realm();

        dphysics::RigidBodySystem PhysicsSystem;

        void registerGameObject(GameObject *object);
        void unregisterGameObject(GameObject *object);

        void updateRealms();

        void process();
        void render();

        bool isIndoor() const { return m_indoor; }
        void setIndoor(bool indoor) { m_indoor = indoor; }

        void spawnObjects();
        void respawnObjects();

        dbasic::DeltaEngine &getEngine();

        void setWorld(World *world) { m_world = world; }
        World *getWorld() const { return m_world; }

        template <typename T>
        T *spawn() {
            void *buffer = _aligned_malloc(sizeof(T), 16);
            T *newObject = new (buffer) T;
            newObject->setWorld(m_world);
            newObject->setRealm(this);
            addToSpawnQueue(newObject);

            return newObject;
        }

        void unload(GameObject *object);
        void respawn(GameObject *object);

        void setExitPortal(GameObject *portal) { m_exitPortal = portal; }
        GameObject *getExitPortal() const { return m_exitPortal; }

        int getAliveObjectCount() const { return (int)m_gameObjects.size(); }
        int getDeadObjectCount() const { return (int)m_deadObjects.size(); }
        int getVisibleObjectCount() const { return m_visibleObjectCount; }

    protected:
        void addToSpawnQueue(GameObject *object);
        void cleanObjectList();
        void destroyObject(GameObject *object);

    protected:
        std::queue<GameObject *> m_unloadQueue;
        std::queue<GameObject *> m_spawnQueue;
        std::queue<GameObject *> m_respawnQueue;
        std::vector<GameObject *> m_gameObjects;
        std::vector<GameObject *> m_deadObjects;

    protected:
        World *m_world;
        GameObject *m_exitPortal;

        int m_visibleObjectCount;
        bool m_indoor;
    };

} /* namespace c_adv */

#endif /* CEREAL_ADVENTURE_REALM_H */

