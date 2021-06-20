#include "../include/demo_lighting_controller.h"

#include "../include/world.h"
#include "../include/colors.h"

c_adv::DemoLightingController::DemoLightingController() {
    /* void */
}

c_adv::DemoLightingController::~DemoLightingController() {
    /* void */
}

void c_adv::DemoLightingController::initialize() {
    GameObject::initialize();
}

void c_adv::DemoLightingController::render() {
    ysVector4 ambient = WallColor;
    ambient.Scale(0.5f);
    m_world->getShaders().SetAmbientLight(ambient);
    m_world->getShaders().SetClearColor(Black);
    m_world->getShaders().SetFogNear(40.0f);
    m_world->getShaders().SetFogFar(80.0f);
    m_world->getShaders().SetFogColor(Black);
}

void c_adv::DemoLightingController::process(float dt) {
    /* void */
}
