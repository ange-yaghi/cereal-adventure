#include "../include/world.h"

#include "../include/asset_loader.h"
#include "../include/realm.h"
#include "../include/player.h"
#include "../include/test_obstacle.h"
#include "../include/ledge.h"
#include "../include/counter.h"
#include "../include/toaster.h"
#include "../include/shelves.h"

c_adv::World::World() {
    m_focus = nullptr;
    m_mainRealm = nullptr;
}

c_adv::World::~World() {
    /* void */
}

void c_adv::World::initialize(void *instance, ysContextObject::DEVICE_API api) {
    dbasic::Path modulePath = dbasic::GetModulePath();
    dbasic::Path confPath = modulePath.Append("delta.conf");

    std::string enginePath = "../../dependencies/delta/engines/basic";
    std::string assetPath = "../../assets";
    if (confPath.Exists()) {
        std::fstream confFile(confPath.ToString(), std::ios::in);

        std::getline(confFile, enginePath);
        std::getline(confFile, assetPath);
        enginePath = modulePath.Append(enginePath).ToString();
        assetPath = modulePath.Append(assetPath).ToString();

        confFile.close();
    }

    m_assetPath = dbasic::Path(assetPath);

    m_engine.GetConsole()->SetDefaultFontDirectory(enginePath + "/fonts/");

    m_engine.CreateGameWindow(
        "Ant World",
        instance,
        api,
        (enginePath + "/shaders/").c_str());

    m_assetManager.SetEngine(&m_engine);

    AssetLoader::loadAllAssets(dbasic::Path(assetPath), &m_assetManager);
}

void c_adv::World::initialSpawn() {
    m_mainRealm = newRealm<Realm>();
    m_mainRealm->setIndoor(false);

    m_focus = m_mainRealm->spawn<Player>();
    m_focus->RigidBody.Transform.SetPosition(ysMath::LoadVector(0.0f, 10.0f, 0.0f));

    ysTransform root;
    dbasic::RenderSkeleton *level1 = m_assetManager.BuildRenderSkeleton(&root, m_assetManager.GetSceneObject("Level1"));
    generateLevel(level1);
}

void c_adv::World::run() {
    initialSpawn();

    while (m_engine.IsOpen()) {
        frameTick();
    }
}

void c_adv::World::frameTick() {
    m_engine.StartFrame();

    process();
    render();

    m_engine.EndFrame();
}

c_adv::AABB c_adv::World::getCameraExtents() const {
    float cameraX, cameraY;
    m_engine.GetCameraPosition(&cameraX, &cameraY);

    float altitude = m_engine.GetCameraAltitude();

    float v, h;
    float fov = m_engine.GetCameraFov();
    float aspect = m_engine.GetCameraAspect();

    v = std::tan(fov / 2) * altitude;
    h = v * aspect;

    return { ysMath::LoadVector(-h + cameraX, -v + cameraY, 0.0f, 0.0f), ysMath::LoadVector(h + cameraX, v + cameraY) };
}

void c_adv::World::render() {
    ysVector focusPosition = m_focus->RigidBody.Transform.GetWorldPosition();
    m_engine.SetCameraPosition(ysMath::GetX(focusPosition), 3.0f);
    m_engine.SetCameraAltitude(7.0f);

    m_mainRealm->render();
}

void c_adv::World::process() {
    m_mainRealm->process();
}

void c_adv::World::generateLevel(dbasic::RenderSkeleton *hierarchy) {
    for (int i = 0; i < hierarchy->GetNodeCount(); ++i) {
        dbasic::RenderNode *node = hierarchy->GetNode(i);
        dbasic::SceneObjectAsset *sceneAsset = node->GetSceneAsset();
        if (strcmp(sceneAsset->GetName(), "Ledge") == 0) {
            ysVector position = node->Transform.GetWorldPosition();
            Ledge *newLedge = m_mainRealm->spawn<Ledge>();
            newLedge->RigidBody.Transform.SetPosition(position);
        }
        else if (strcmp(sceneAsset->GetName(), "Counter_1") == 0) {
            ysVector position = node->Transform.GetWorldPosition();
            Counter *newCounter = m_mainRealm->spawn<Counter>();
            newCounter->RigidBody.Transform.SetPosition(position);
        }
        else if (strcmp(sceneAsset->GetName(), "Toaster") == 0) {
            ysVector position = node->Transform.GetWorldPosition();
            Toaster *newToaster = m_mainRealm->spawn<Toaster>();
            newToaster->RigidBody.Transform.SetPosition(position);
        }
        else if (strcmp(sceneAsset->GetName(), "Shelves") == 0) {
            ysVector position = node->Transform.GetWorldPosition();
            Shelves *newShelves = m_mainRealm->spawn<Shelves>();
            newShelves->RigidBody.Transform.SetPosition(position);
        }
    }
}

void c_adv::World::updateRealms() {
    for (Realm *realm : m_realms) {
        realm->updateRealms();
    }
}
