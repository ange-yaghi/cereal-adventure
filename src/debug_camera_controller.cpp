#include "../include/debug_camera_controller.h"

#include "../include/world.h"

c_adv::DebugCameraController::DebugCameraController() {
    m_cameraDistance = 10.0f;
}

c_adv::DebugCameraController::~DebugCameraController() {
    /* void */
}

void c_adv::DebugCameraController::initialize() {
    GameObject::initialize();

    m_smoothCamera.setPosition(ysMath::Constants::Zero);
    m_smoothCamera.setStiffnessTensor(ysMath::LoadVector(200.0f, 50.0f, 0.0f));
    m_smoothCamera.setDampingTensor(ysMath::LoadVector(0.3f, 0.5f, 0.0f));

    m_smoothTarget.setPosition(ysMath::Constants::Zero);
    m_smoothTarget.setStiffnessTensor(ysMath::LoadVector(250.0f, 100.0f, 0.0f));
    m_smoothTarget.setDampingTensor(ysMath::LoadVector(0.3f, 0.5f, 0.0f));
}

void c_adv::DebugCameraController::render() {
    float offset_x = 0.0f, offset_y = 0.0f;
    if (m_world->getEngine().IsKeyDown(ysKey::Code::Down)) {
        offset_y = -5.0f;
    }
    else if (m_world->getEngine().IsKeyDown(ysKey::Code::Up)) {
        offset_y = 5.0f;
    }

    GameObject *focus = m_world->getFocus();

    if (focus != nullptr) {
        const ysVector focusPosition = focus->RigidBody.Transform.GetWorldPosition();
        m_smoothCamera.setTarget(ysMath::Add(focusPosition, ysMath::LoadVector(0.0f, 0.0f, 0.0f)));
        m_smoothTarget.setTarget(ysMath::Add(focusPosition, ysMath::LoadVector(offset_x, offset_y, 0.0f)));
    }

    const ysVector cameraTarget = m_smoothCamera.getPosition();

    m_world->getShaders().SetCameraPosition(ysMath::GetX(cameraTarget), ysMath::GetY(cameraTarget));
    m_world->getShaders().SetCameraAltitude(m_cameraDistance);
    m_world->getShaders().SetCameraTarget(m_smoothTarget.getPosition());
    m_world->getShaders().SetCameraUp(ysMath::Constants::YAxis);
}

void c_adv::DebugCameraController::process(float dt) {
    m_smoothCamera.update(dt);
    m_smoothTarget.update(dt);

    if (m_world->getEngine().IsKeyDown(ysKey::Code::Subtract)) {
        m_cameraDistance += 0.5f;
    }
    else if (m_world->getEngine().IsKeyDown(ysKey::Code::Add)) {
        m_cameraDistance -= 0.5f;
    }
    else if (m_world->getEngine().IsKeyDown(ysKey::Code::Back)) {
        m_cameraDistance = 10.0f;
    }
}
