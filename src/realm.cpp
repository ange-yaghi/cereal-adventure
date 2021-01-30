#include "../include/realm.h"

#include "../include/game_object.h"
#include "../include/world.h"
#include "../include/colors.h"

c_adv::Realm::Realm() {
    m_exitPortal = nullptr;
    m_world = nullptr;
    m_indoor = false;

    m_visibleObjectCount = 0;

    initializeFrictionTable();
}

c_adv::Realm::~Realm() {
    /* void */
}

void c_adv::Realm::registerGameObject(GameObject *object) {
    assert(static_cast<GameObject *>(object->RigidBody.GetOwner()) == object);

    object->setRealm(this);
    object->setRealmRecordIndex((int)m_gameObjects.size());
    m_gameObjects.push_back(object);

    PhysicsSystem.RegisterRigidBody(&object->RigidBody);
}

void c_adv::Realm::unregisterGameObject(GameObject *object) {
    int index = object->getRealmRecordIndex();

    object->setRealmRecordIndex(-1);
    PhysicsSystem.RemoveRigidBody(&object->RigidBody);

    m_gameObjects[index] = m_gameObjects.back();
    m_gameObjects[index]->setRealmRecordIndex(index);
    m_gameObjects.pop_back();
}

void c_adv::Realm::process(float dt) {
    spawnObjects();
    respawnObjects();

    for (GameObject *g : m_gameObjects) {
        g->resetAccumulators();
    }

    for (GameObject *g : m_gameObjects) {
        g->createVisualBounds();
        g->process(dt);
    }

    cleanObjectList();

    m_world->getEngine().GetBreakdownTimer().StartMeasurement(World::PhysicsTimer);
    {
        PhysicsSystem.Update(dt);
    }
    m_world->getEngine().GetBreakdownTimer().EndMeasurement(World::PhysicsTimer);
}
 
void c_adv::Realm::render() {
    m_world->getEngine().SetClearColor(ysColor::linearToSrgb(SkyBlue));

    ysVector4 ambient = WallColor;
    ambient.Scale(0.1f);
    m_world->getShaders().SetAmbientLight(ambient);

    const int shadowMap = m_world->getShaders().AddPerspectiveShadowMap(
        ysMath::LoadVector(-5.0f, 0.0f, 10.0f),
        ysMath::Constants::Zero,
        ysMath::Constants::YAxis,
        ysMath::Constants::PI / 3,
        1.0f, 2.0f, 400.0f);
    
    Light light{};
    light.Active = 1;
    light.Color = ysColor::srgbiToLinear(255, 197, 143);
    light.Color.Scale(1.0f);

    light.Attenuation0 = -1.0f;
    light.Attenuation1 = 0.4f;
    light.Direction = ysVector4(0.0f, 0.0f, 1.0f, 0.0f);
    light.FalloffEnabled = 1;
    light.FalloffStart = 2.0f;
    light.FalloffScale = 5.0f;
    light.Position = ysMath::GetVector4(ysMath::LoadVector(-5.0f, 0.0f, 10.0f));
    light.ShadowMap = shadowMap;

    m_world->getShaders().AddLight(light);

    AABB cameraExtents = m_world->getCameraExtents();
    int visibleObjects = 0;
    for (GameObject *g : m_gameObjects) {
        if (g->getDeletionFlag()) continue;
        AABB extents = g->getVisualBounds();

        if (extents.intersects2d(cameraExtents) || true) {
            g->render();
            ++visibleObjects;
        }
    }

    m_visibleObjectCount = visibleObjects;
}

void c_adv::Realm::spawnObjects() {
    while (!m_spawnQueue.empty()) {
        GameObject *u = m_spawnQueue.front(); m_spawnQueue.pop();
        u->initialize();
        registerGameObject(u);
    }
}

void c_adv::Realm::respawnObjects() {
    while (!m_respawnQueue.empty()) {
        GameObject *u = m_respawnQueue.front(); m_respawnQueue.pop();
        registerGameObject(u);
    }
}

dbasic::DeltaEngine &c_adv::Realm::getEngine() {
    return m_world->getEngine();
}

void c_adv::Realm::updateRealms() {
    int N = (int)m_gameObjects.size();
    for (int i = 0; i < N; ++i) {
        if (m_gameObjects[i]->isChangingRealm()) {
            GameObject *object = m_gameObjects[i];
            Realm *newRealm = object->getNewRealm();
            object->resetRealmChange();

            unregisterGameObject(object);

            if (newRealm != nullptr) {
                newRealm->registerGameObject(object);
            }

            GameObject *lastPortal = object->getLastPortal();
            object->setLastPortal(nullptr);

            if (lastPortal != nullptr) {
                if (newRealm == lastPortal->getTargetRealm()) {
                    lastPortal->onEnter(object);
                }
                else if (newRealm == lastPortal->getRealm()) {
                    lastPortal->onExit(object);
                }
            }

            --i;
            N = (int)m_gameObjects.size();
        }
    }
}

void c_adv::Realm::unload(GameObject *object) {
    m_unloadQueue.push(object);
}

void c_adv::Realm::respawn(GameObject *object) {
    m_respawnQueue.push(object);
}

void c_adv::Realm::addToSpawnQueue(GameObject *object) {
    m_spawnQueue.push(object);
}

void c_adv::Realm::cleanObjectList() {
    int N = (int)m_gameObjects.size();
    for (int i = 0; i < N; ++i) {
        if (m_gameObjects[i]->getDeletionFlag()) {
            m_gameObjects[i]->setDead();
            m_deadObjects.push_back(m_gameObjects[i]);
            unregisterGameObject(m_gameObjects[i]);

            --i; --N;
        }
    }

    int N_dead = (int)m_deadObjects.size();
    for (int i = 0; i < N_dead; ++i) {
        if (m_deadObjects[i]->getReferenceCount() == 0) {
            destroyObject(m_deadObjects[i]);

            m_deadObjects[i] = m_deadObjects.back();
            m_deadObjects.pop_back();

            --i; --N_dead;
        }
    }

    int N_unloaded = (int)m_unloadQueue.size();
    for (int i = 0; i < N_unloaded; ++i) {
        GameObject *u = m_unloadQueue.front(); m_unloadQueue.pop();
        unregisterGameObject(u);
    }
}

void c_adv::Realm::destroyObject(GameObject *object) {
    _aligned_free((void *)object);
}

void c_adv::Realm::initializeFrictionTable() {
    PhysicsSystem.InitializeFrictionTable(10, 0.5f, 0.5f);

    PhysicsSystem.SetFriction(
        GameObject::PlayerFrictionMaterial, GameObject::GenericFrictionMaterial,
        0.1f, 0.1f);

    PhysicsSystem.SetFriction(
        GameObject::GenericFrictionMaterial, GameObject::GenericFrictionMaterial,
        2.5f, 2.5f);
}
