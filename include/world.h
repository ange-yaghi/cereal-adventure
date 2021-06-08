#ifndef CEREAL_ADVENTURE_WORLD_H
#define CEREAL_ADVENTURE_WORLD_H

#include "aabb.h"

#include "delta.h"
#include "realm.h"
#include "spring_connector.h"
#include "shaders.h"
#include "ui.h"

#include <vector>
#include <string>

namespace c_adv {

    class World {
    public:
        static const float DefaultCameraDistance;
        static const std::string PhysicsTimer;

    public:
        World();
        ~World();

        void initialize(void *instance, ysContextObject::DeviceAPI api);
        void initialSpawn();
        void run();
        void frameTick();

        dbasic::DeltaEngine &getEngine() { return m_engine; }
        dbasic::AssetManager &getAssetManager() { return m_assetManager; }
        Shaders &getShaders() { return m_shaders; }
        Ui &getUi() { return m_ui; }
        dbasic::ShaderSet &getShaderSet() { return m_shaderSet; }

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

        dbasic::StageEnableFlags getUiStageFlags() const { return m_uiStageFlags; }

    protected:
        void renderUi();
        void updateRealms();

        std::vector<Realm *> m_realms;

        Realm *m_mainRealm;
        GameObject *m_focus;

        SpringConnector m_smoothCamera;
        SpringConnector m_smoothTarget;

        ysVector m_respawnPosition;
        float m_cameraDistance;

        ysRenderTarget *m_intermediateRenderTarget;
        ysRenderTarget *m_guiRenderTarget;

        Ui m_ui;

    protected:
        Shaders m_shaders;
        dbasic::DeltaEngine m_engine;
        dbasic::AssetManager m_assetManager;
        dbasic::ShaderSet m_shaderSet;

        dbasic::Path m_assetPath;

        dbasic::StageEnableFlags m_uiStageFlags;
    };

} /* namespace c_adv */

#endif /* CEREAL_ADVENTURE_GAME_OBJECT_H */
