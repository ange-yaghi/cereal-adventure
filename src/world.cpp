#include "../include/world.h"

#include "../include/asset_loader.h"
#include "../include/realm.h"
#include "../include/player.h"
#include "../include/test_obstacle.h"

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

    GameObject *testObstacle = nullptr;

    testObstacle = m_mainRealm->spawn<TestObstacle>();
    testObstacle->RigidBody.Transform.SetPosition(ysMath::LoadVector(0.0f, -3.0f, 0.0f));

    testObstacle = m_mainRealm->spawn<TestObstacle>();
    testObstacle->RigidBody.Transform.SetPosition(ysMath::LoadVector(2.0f, 1.0f, 0.0f));

    testObstacle = m_mainRealm->spawn<TestObstacle>();
    testObstacle->RigidBody.Transform.SetPosition(ysMath::LoadVector(4.0f, -3.0f, 0.0f));

    testObstacle = m_mainRealm->spawn<TestObstacle>();
    testObstacle->RigidBody.Transform.SetPosition(ysMath::LoadVector(6.0f, -2.5f, 0.0f));

    testObstacle = m_mainRealm->spawn<TestObstacle>();
    testObstacle->RigidBody.Transform.SetPosition(ysMath::LoadVector(8.0f, -1.5f, 0.0f));

    testObstacle = m_mainRealm->spawn<TestObstacle>();
    testObstacle->RigidBody.Transform.SetPosition(ysMath::LoadVector(10.0f, -0.5f, 0.0f));

    testObstacle = m_mainRealm->spawn<TestObstacle>();
    testObstacle->RigidBody.Transform.SetPosition(ysMath::LoadVector(12.0f, -2.0f, 0.0f));

    testObstacle = m_mainRealm->spawn<TestObstacle>();
    testObstacle->RigidBody.Transform.SetPosition(ysMath::LoadVector(18.0f, -2.0f, 0.0f));
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
    m_engine.SetCameraPosition(ysMath::GetX(focusPosition), 0.0f);
    m_engine.SetCameraAltitude(7.0f);

    m_mainRealm->render();
}

void c_adv::World::process() {
    m_mainRealm->process();
}

void c_adv::World::updateRealms() {
    for (Realm *realm : m_realms) {
        realm->updateRealms();
    }
}
