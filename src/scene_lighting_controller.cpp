#include "../include/scene_lighting_controller.h"

#include "../include/world.h"
#include "../include/colors.h"

c_adv::SceneLightingController::SceneLightingController() {
    /* void */
}

c_adv::SceneLightingController::~SceneLightingController() {
    /* void */
}

void c_adv::SceneLightingController::initialize() {
    GameObject::initialize();
}

void c_adv::SceneLightingController::render() {
    ysVector4 ambient = WallColor;
    ambient.Scale(0.01f);
    m_world->getShaders().SetAmbientLight(ambient);

    m_world->getShaders().SetClearColor(Black);
    m_world->getShaders().SetFogNear(1000.0f);
    m_world->getShaders().SetFogFar(1001.0f);
    m_world->getShaders().SetFogColor(ysMath::LoadVector(ambient.x, ambient.y, ambient.z, 1.0f));
}

void c_adv::SceneLightingController::process(float dt) {
    /* void */
}
