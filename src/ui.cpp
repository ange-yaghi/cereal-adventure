#include "../include/ui.h"

#include "../include/world.h"

c_adv::Ui::Ui() {
    m_world = nullptr;
    m_playerHealth = 1.0f;

    initialize();
}

c_adv::Ui::~Ui() {
    /* void */
}

void c_adv::Ui::initialize() {
    m_damageTimer.setCooldownPeriod(0.2f);
    m_healthHeartbeat.setPeriod(0.2f);
}

void c_adv::Ui::render() {
    dbasic::DeltaEngine &engine = m_world->getEngine();
    Shaders &shaders = m_world->getShaders();

    dbasic::TextureAsset *healthOverlay = m_world->getAssetManager().GetTexture("Health_Overlay");
    dbasic::TextureAsset *damageOverlay = m_world->getAssetManager().GetTexture("Damage_Overlay");

    shaders.ResetBrdfParameters();
    shaders.SetLit(false);
    shaders.SetObjectTransform(ysMath::TranslationTransform(ysMath::LoadVector(0.0f, 0.0f, 0.1f)));
    shaders.ConfigureBox(engine.GetScreenWidth(), engine.GetScreenHeight());
    shaders.SetColorReplace(false);
    shaders.SetAoMap(false);

    if (m_damageTimer.active()) {
        const float s = m_damageTimer.get();
        const float intensity = std::sin((s - 1) * (s - 1) * ysMath::Constants::PI);

        shaders.SetDiffuseTexture(damageOverlay->GetTexture());
        shaders.SetBaseColor(ysMath::LoadVector(1.0f, 1.0f, 1.0f, intensity * 0.5f));
        engine.DrawBox(m_world->getUiStageFlags());
    }

    float heartbeat = m_healthHeartbeat.get();
    const float curvePortion = 0.8f;
    if (heartbeat < curvePortion) {
        heartbeat =
            sin((heartbeat * (1 / curvePortion) - 1) * (heartbeat * (1 / curvePortion) - 1) * ysMath::Constants::PI);
    }
    else {
        heartbeat = 0.0f;
    }

    shaders.SetDiffuseTexture(healthOverlay->GetTexture());
    shaders.SetBaseColor(ysMath::LoadVector(1.0f, 1.0f, 1.0f, heartbeat * (1 - m_playerHealth)));
    engine.DrawBox(m_world->getUiStageFlags());
}

void c_adv::Ui::process(float dt) {
    m_damageTimer.update(dt);
    m_healthHeartbeat.update(dt);

    m_healthHeartbeat.adjustPeriod(m_playerHealth + 3.0f);
}

void c_adv::Ui::triggerDamage(float amount) {
    const float baseLength = amount * 10.0f + 0.1f;
    if (m_damageTimer.active()) {
        m_damageTimer.setCooldownPeriod(baseLength + m_damageTimer.getCooldownPeriod());
    }
    else {
        m_damageTimer.setCooldownPeriod(baseLength);
        m_damageTimer.trigger();
    }
}
