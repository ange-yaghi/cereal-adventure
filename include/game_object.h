#ifndef CEREAL_ADVENTURE_GAME_OBJECT_H
#define CEREAL_ADVENTURE_GAME_OBJECT_H

#include "aabb.h"

#include "delta.h"

namespace c_adv {

    class World;
    class Realm;

    class GameObject {
    public:
        enum class CollisionLayers {
            Main = 0x0,
            Box = 0x1,
            CarriedBoxableItem = 0x2
        };

        enum class Layer {
            Ground,
            Holes,
            Items,
            Mob,
            PlayerCarriedItem,
            Player,
            Wall
        };

        enum class Tag {
            Hole,
            Wall,
            Floor,
            Obstacle,
            Insect,
            Player,
            Carryable,
            Edible,
            Container,
            Beetle,
            Probe,
            Count
        };

    public:
        GameObject();
        ~GameObject();

        dphysics::RigidBody RigidBody;

        void setWorld(World *world) { m_world = world; }
        World *getWorld() const { return m_world; }

        virtual void initialize();
        virtual void render();
        virtual void process();

        virtual void onCarry();
        virtual void onDrop();

        GameObject *getCollidingObject(dphysics::Collision *collision);

        bool colliding();
        bool colliding(GameObject *object);

        bool getDeletionFlag() const { return m_deletionFlag; }
        void setDeletionFlag() { m_deletionFlag = true; }

        void setBeingCarried(bool carried) { m_beingCarried = carried; }
        bool isBeingCarried() const { return m_beingCarried; }

        bool hasTag(Tag tag) const { return m_tags[(int)tag]; }
        void addTag(Tag tag) { m_tags[(int)tag] = true; }
        void removeTag(Tag tag) { m_tags[(int)tag] = false; }

        int getRealmRecordIndex() const { return m_realmRecordIndex; }
        void setRealmRecordIndex(int index) { m_realmRecordIndex = index; }

        void setRealm(Realm *realm) { m_realm = realm; }
        Realm *getRealm() const { return m_realm; }

        virtual void changeRealm(Realm *newRealm) { m_newRealm = newRealm; m_changeRealm = true; }
        Realm *getNewRealm() const { return m_newRealm; }

        void resetRealmChange() { m_changeRealm = false; m_newRealm = nullptr; }
        bool isChangingRealm() const { return m_changeRealm; }

        GameObject *getLastPortal() { return m_lastPortal; }
        void setLastPortal(GameObject *hole) { m_lastPortal = hole; }

        const AABB &getVisualBounds() const { return m_visualBounds; }
        void addVisualBound(const AABB &bound);
        virtual void createVisualBounds();

        void incrementReferenceCount() { ++m_referenceCount; }
        void decrementReferenceCount() { --m_referenceCount; }
        int getReferenceCount() const { return m_referenceCount; }

        virtual ysVector getPickupPointWorld() { return RigidBody.Transform.GetWorldPosition(); }
        virtual float getPickupRadius() const { return 0.0f; }

        void setGraceMode(bool graceMode);
        bool inGraceMode() const { return m_graceMode; }

        virtual void onEnter(GameObject *object) { /* void */ }
        virtual void onExit(GameObject *object) { /* void */ }

        virtual Realm *getTargetRealm() { return nullptr; }
        virtual Realm *generateRealm() { return nullptr; }

        // Get whether this object has been fully registered in its realm
        bool isReal() const { return m_real; }

    protected:
        AABB m_visualBounds;

    protected:
        int m_referenceCount;

    protected:
        World *m_world;
        Realm *m_realm;
        Realm *m_newRealm;
        bool m_changeRealm;
        GameObject *m_lastPortal;

    private:
        bool m_beingCarried;
        bool m_graceMode;
        bool m_real;

    private:
        bool m_deletionFlag;

    private:
        std::vector<bool> m_tags;

    private:
        int m_realmRecordIndex;
    };

} /* namespace c_adv */

#endif /* CEREAL_ADVENTURE_GAME_OBJECT_H */
