#include "../include/demo_shader_controls.h"

#include "../include/world.h"
#include "../include/math_utilities.h"

c_adv::DemoShaderControls::DemoShaderControls() {
    /* void */
}

c_adv::DemoShaderControls::~DemoShaderControls() {
    /* void */
}

void c_adv::DemoShaderControls::initialize() {
    GameObject::initialize();

    initialize(Controls::AmbientDiffuse, ysKey::Code::F1, true);
    initialize(Controls::AmbientSpecular, ysKey::Code::F2, true);
    initialize(Controls::BakedAo, ysKey::Code::F3, true);
    initialize(Controls::Diffuse, ysKey::Code::F4, true);
    initialize(Controls::Specular, ysKey::Code::F5, true);
    initialize(Controls::Shadow, ysKey::Code::F6, true);
    initialize(Controls::Ssao, ysKey::Code::F7, true);
    initialize(Controls::Bloom, ysKey::Code::F8, true);
    initialize(Controls::Dither, ysKey::Code::F9, true);
    initialize(Controls::VisualizeSsao, ysKey::Code::C, false);
    initialize(Controls::VisualizeBloom, ysKey::Code::V, false);
    initialize(Controls::DebugBlackAndWhite, ysKey::Code::B, false);

    for (int i = 0; i < (int)Controls::Count; ++i) {
        const ysVector value = m_controls[i]
            ? ysMath::Constants::One
            : ysMath::Constants::Zero;

        m_connectors[i].setPosition(value);
        m_connectors[i].setStiffnessTensor(ysMath::LoadVector(100.0f, 100.0f, 100.0f, 100.0f));
        m_connectors[i].setDampingTensor(ysMath::LoadVector(0.5f, 0.5f, 0.5f, 0.5f));
    }
}

void c_adv::DemoShaderControls::render() {
    for (int i = 0; i < (int)Controls::Count; ++i) {
        submit((Controls)i, ysMath::GetScalar(m_connectors[i].getPosition()));
    }
}

void c_adv::DemoShaderControls::process(float dt) {
    for (int i = 0; i < (int)Controls::Count; ++i) {
        if (m_world->getEngine().ProcessKeyDown(m_keyMapping[i])) {
            invert((Controls)i);
        }
    }

    if (m_world->getEngine().ProcessKeyDown(ysKey::Code::L)) {
        set(Controls::Specular, get(Controls::Diffuse));

        invert(Controls::Specular);
        invert(Controls::Diffuse);
    }

    if (m_world->getEngine().ProcessKeyDown(ysKey::Code::N1)) {
        for (int i = 0; i < (int)Controls::Count; ++i) {
            set((Controls)i, m_controlDefault[i]);
        }
    }

    for (int i = 0; i < (int)Controls::Count; ++i) {
        const ysVector value = m_controls[i]
            ? ysMath::Constants::One
            : ysMath::Constants::Zero;
        m_connectors[i].setTarget(value);
        m_connectors[i].update(dt);
    }
}

void c_adv::DemoShaderControls::initialize(Controls control, ysKey::Code key, bool value) {
    set(control, value);
    m_controlDefault[(int)control] = value;
    m_keyMapping[(int)control] = key;
}

void c_adv::DemoShaderControls::invert(Controls control) {
    m_controls[(int)control] = !m_controls[(int)control];
}

void c_adv::DemoShaderControls::set(Controls control, bool value) {
    m_controls[(int)control] = value;
}

bool c_adv::DemoShaderControls::get(Controls control) {
    return m_controls[(int)control];
}

void c_adv::DemoShaderControls::submit(Controls control, float value) {
    Shaders &shaders = m_world->getShaders();

    value = min(max(value, 0.0f), 1.0);

    switch (control) {
    case Controls::AmbientDiffuse:
        shaders.SetAmbientDiffuseAmount(value);
        break;
    case Controls::AmbientSpecular:
        shaders.SetAmbientSpecularAmount(value);
        break;
    case Controls::Ssao:
        shaders.SetSsaoAmount(value);
        break;
    case Controls::BakedAo:
        shaders.SetBakedAoAmount(value);
        break;
    case Controls::Diffuse:
        shaders.SetDiffuseAmount(value);
        break;
    case Controls::Specular:
        shaders.SetSpecularAmount(value);
        break;
    case Controls::Shadow:
        shaders.SetShadowAmount(value);
        break;
    case Controls::VisualizeSsao:
        shaders.SetDebugSsao(value);
        break;
    case Controls::VisualizeBloom:
        shaders.SetDebugBloom(value);
        break;
    case Controls::Bloom:
        shaders.SetBloomAmount(value);
        break;
    case Controls::Dither:
        shaders.SetDitherAmount(value);
        break;
    case Controls::DebugBlackAndWhite:
        shaders.SetDebugBlackAndWhite(value);
        break;
    }
}
