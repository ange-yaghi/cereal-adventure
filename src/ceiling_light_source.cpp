#include "../include/ceiling_light_source.h"

#include "../include/colors.h"
#include "../include/world.h"

c_adv::CeilingLightSource::CeilingLightSource() {
    /* void */
}

c_adv::CeilingLightSource::~CeilingLightSource() {
    /* void */
}

void c_adv::CeilingLightSource::initialize() {
    GameObject::initialize();

    RigidBody.SetHint(dphysics::RigidBody::RigidBodyHint::Dynamic);
    RigidBody.SetInverseMass(0.0f);
}

void c_adv::CeilingLightSource::render() {
    const ysVector position = RigidBody.Transform.GetWorldPosition();

    int shadowMap = -1;
    if (ysMath::GetX(position) < 40) {
        shadowMap = m_world->getShaders().AddPerspectiveShadowMap(
            RigidBody.Transform.GetWorldPosition(),
            ysMath::Sub(position, ysMath::Constants::YAxis),
            ysMath::Constants::ZAxis,
            ysMath::Constants::PI / 2,
            1.0f, 2.0f, 50.0f);
    }

    Light light{};
    
    light.Active = 1;
    light.Attenuation0 = 0.8f;
    light.Attenuation1 = 0.7f;
    light.Color = ysMath::GetVector4(ysColor::srgbiToLinear(0xff, 0xff, 0xff, 0xff));
    light.Color.Scale(16.0f);

    light.Direction = ysVector4(0.0f, -1.0f, 0.0f, 0.0f);
    light.FalloffEnabled = 1;
    light.Position = ysMath::GetVector4(position);

    light.ShadowMap = shadowMap;

    m_world->getShaders().AddLight(light); 
}
