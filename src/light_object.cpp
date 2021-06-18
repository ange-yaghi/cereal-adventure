#include "../include/light_object.h"

#include "../include/shaders.h"
#include "../include/world.h"

c_adv::LightObject::LightObject() {
    m_asset = nullptr;
}

c_adv::LightObject::~LightObject() {
    /* void */
}

void c_adv::LightObject::initialize() {
    GameObject::initialize();
}

void c_adv::LightObject::render() {
    const dbasic::SceneObjectAsset::LightInformation &lightInfo = m_asset->GetLightInformation();
    
    float fov_radians = lightInfo.SpotAngularSize;
    float attenuation1 = 0.0f;
    float attenuation0 = -1.0f;

    const ysVector direction = RigidBody.Transform.LocalToWorldDirection(ysMath::Negate(ysMath::Constants::ZAxis));
    const ysVector position = RigidBody.Transform.GetWorldPosition();

    int shadowMap = -1;
    bool isSun = false;
    if (lightInfo.LightType == dbasic::SceneObjectAsset::LightInformation::Type::Spot) {
        shadowMap = m_world->getShaders().AddPerspectiveShadowMap(
            position,
            ysMath::Add(position, direction),
            ysMath::Normalize(ysMath::Cross(direction, ysMath::Constants::XAxis)),
            fov_radians,
            1.0f, 2.0f, 400.0f);

        attenuation1 = std::cos(fov_radians / 2.0f);
        attenuation0 = (1 - lightInfo.SpotFade) * attenuation1 + lightInfo.SpotFade * 1.0;
    }
    else if (lightInfo.LightType == dbasic::SceneObjectAsset::LightInformation::Type::Sun) {
        isSun = true;
        shadowMap = m_world->getShaders().AddOrthographicShadowMap(
            position,
            ysMath::Add(position, direction),
            ysMath::Normalize(ysMath::Cross(direction, ysMath::Constants::XAxis)),
            20.0f, 20.0f, 2.0f, 400.0f);
    }

    Light light{};
    light.Active = 1;
    light.Attenuation0 = attenuation0;
    light.Attenuation1 = attenuation1;
    light.Color = ysVector4(lightInfo.Color.x, lightInfo.Color.y, lightInfo.Color.z, 1.0f);
    light.Color.Scale(lightInfo.Intensity / 10.0f);
    light.Direction = ysMath::GetVector4(direction);
    light.FalloffEnabled = (isSun) ? 0 : 1;
    light.Position = ysMath::GetVector4(position);
    light.ShadowMap = shadowMap;

    m_world->getShaders().AddLight(light);
}

void c_adv::LightObject::process(float dt) {
    /* void */
}
