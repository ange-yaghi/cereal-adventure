#include "../include/turn_table_camera.h"

#include "../include/world.h"

c_adv::TurnTableCamera::TurnTableCamera() {
    m_turnTableAngle = 0;
    m_verticalAngle = 0;
    m_distance = 0;
    m_targetHeight = 1.0f;
}

c_adv::TurnTableCamera::~TurnTableCamera() {
    /* void */
}

void c_adv::TurnTableCamera::initialize() {
    GameObject::initialize();

    m_turnTableAngle = 0;
    m_verticalAngle = ysMath::Constants::PI / 3.0f;
    m_distance = 10.0f;

    m_smoothPosition.setPosition(ysMath::LoadVector(0.0f, m_distance, 0.0f));
    m_smoothPosition.setStiffnessTensor(ysMath::LoadVector(100.0f, 100.0f, 100.0f));
    m_smoothPosition.setDampingTensor(ysMath::LoadVector(0.5f, 0.5f, 0.5f));

    m_smoothTarget.setPosition(ysMath::LoadVector(0.0f, 0.0f, m_targetHeight));
    m_smoothTarget.setStiffnessTensor(ysMath::LoadVector(100.0f, 100.0f, 100.0f));
    m_smoothTarget.setDampingTensor(ysMath::LoadVector(0.5f, 0.5f, 0.5f));
}

void c_adv::TurnTableCamera::render() {
    m_world->getShaders().SetCameraMode(Shaders::CameraMode::Target);
    m_world->getShaders().SetCameraTarget(m_smoothTarget.getPosition());
    m_world->getShaders().SetCameraUp(ysMath::Constants::ZAxis);
    m_world->getShaders().SetCameraPosition(m_smoothPosition.getPosition());
    m_world->getShaders().SetCameraFov(ysMath::Constants::PI / 4.0f);
}

void c_adv::TurnTableCamera::process(float dt) {
    const ysMatrix tt_rot =
        ysMath::RotationTransform(ysMath::Constants::ZAxis, m_turnTableAngle);
    const ysMatrix elevation_rot =
        ysMath::RotationTransform(ysMath::Constants::XAxis, m_verticalAngle);

    ysVector cameraPosition = ysMath::LoadVector(0.0f, m_distance, 0.0f);
    cameraPosition = ysMath::MatMult(elevation_rot, cameraPosition);
    cameraPosition = ysMath::MatMult(tt_rot, cameraPosition);

    m_smoothPosition.setTarget(cameraPosition);
    m_smoothPosition.update(dt);

    m_smoothTarget.setTarget(ysMath::LoadVector(0.0f, 0.0f, m_targetHeight));
    m_smoothTarget.update(dt);

    if (m_world->getEngine().IsKeyDown(ysKey::Code::Left)) {
        m_turnTableAngle -= 1.0f * dt;
    }
    else if (m_world->getEngine().IsKeyDown(ysKey::Code::Right)) {
        m_turnTableAngle += 1.0f * dt;
    }

    if (m_world->getEngine().IsKeyDown(ysKey::Code::Up)) {
        m_verticalAngle += 1.0f * dt;
    }
    else if (m_world->getEngine().IsKeyDown(ysKey::Code::Down)) {
        m_verticalAngle -= 1.0f * dt;
    }

    if (m_world->getEngine().IsKeyDown(ysKey::Code::Add)) {
        m_distance -= 5.0f * dt;
    }
    else if (m_world->getEngine().IsKeyDown(ysKey::Code::Subtract)) {
        m_distance += 5.0f * dt;
    }

    if (m_world->getEngine().IsKeyDown(ysKey::Code::W)) {
        m_targetHeight += 5.0f * dt;
    }
    else if (m_world->getEngine().IsKeyDown(ysKey::Code::S)) {
        m_targetHeight -= 5.0f * dt;
    }

    const float MaxVerticalAngle = ysMath::Constants::PI * 0.9f / 2;
    if (m_verticalAngle >= MaxVerticalAngle) {
        m_verticalAngle = MaxVerticalAngle;
    }
    else if (m_verticalAngle <= -MaxVerticalAngle) {
        m_verticalAngle = -MaxVerticalAngle;
    }

    if (m_distance <= 2.0f) {
        m_distance = 2.0f;
    }
}
