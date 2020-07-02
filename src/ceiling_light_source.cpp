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
    dbasic::Light light;
    
    light.Active = 1;
    light.Attenuation0 = 0.8f;
    light.Attenuation1 = 0.7f;
    light.Color = ysMath::GetVector4(ysColor::srgbiToLinear(0xff, 0xff, 0xff, 0xff));
    light.Color.Scale(16.0f);

    light.Direction = ysVector4(0.0f, -1.0f, 0.0f, 0.0f);
    light.FalloffEnabled = 1;
    light.Position = ysMath::GetVector4(RigidBody.Transform.GetWorldPosition());

    m_world->getEngine().AddLight(light); 
}
