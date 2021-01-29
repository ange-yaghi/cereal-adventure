#include "../include/window_light_source.h"

#include "../include/colors.h"
#include "../include/world.h"

c_adv::WindowLightSource::WindowLightSource() {
    /* void */
}

c_adv::WindowLightSource::~WindowLightSource() {
    /* void */
}

void c_adv::WindowLightSource::initialize() {
    GameObject::initialize();

    RigidBody.SetHint(dphysics::RigidBody::RigidBodyHint::Dynamic);
    RigidBody.SetInverseMass(0.0f);
}

void c_adv::WindowLightSource::render() {
    Light light{};

    light.Active = 1;
    light.Color = ysMath::GetVector4(ysColor::srgbiToLinear(0xB8CEF2));
    light.Color.Scale(1.0f);

    light.Attenuation0 = 0.6f;
    light.Attenuation1 = 0.3f;
    light.Direction = ysVector4(0.0f, 0.0f, 1.0f, 0.0f);
    light.FalloffEnabled = 1;
    light.FalloffStart = 0.0f;
    light.FalloffScale = 2.0f;
    light.Position = ysMath::GetVector4(RigidBody.Transform.GetWorldPosition());

    m_world->getShaders().AddLight(light);
}
