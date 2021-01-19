#include "../include/cereal_adventure_app.h"

c_adv::CerealAdventureApp::CerealAdventureApp() {
    /* void */
}

c_adv::CerealAdventureApp::~CerealAdventureApp() {
    /* void */
}

void c_adv::CerealAdventureApp::Initialize(void *instance, ysContextObject::DeviceAPI api) {
    m_world.initialize(instance, api);
}

void c_adv::CerealAdventureApp::Run() {
    m_world.run();

    m_world.getAssetManager().Destroy();
    m_world.getShaders().Destroy();
    m_world.getEngine().Destroy();
}
