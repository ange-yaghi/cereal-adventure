#include "../include/static_art.h"

#include "../include/colors.h"
#include "../include/world.h"

c_adv::StaticArt::StaticArt() {
    m_asset = nullptr;
}

c_adv::StaticArt::~StaticArt() {
    /* void */
}

void c_adv::StaticArt::initialize() {
    GameObject::initialize();

    RigidBody.SetHint(dphysics::RigidBody::RigidBodyHint::Dynamic);
    RigidBody.SetInverseMass(0.0f);
}

void c_adv::StaticArt::render() {
    m_world->getEngine().ResetBrdfParameters();

    if (strcmp(m_asset->GetName(), "Level1Wall") == 0) {
        m_world->getEngine().SetBaseColor(WallColor);
    }
    else {
        m_world->getEngine().SetBaseColor(ObjectColor);
    }

    m_world->getEngine().SetObjectTransform(RigidBody.Transform.GetWorldTransform());
    m_world->getEngine().DrawModel(m_asset, 1.0f, nullptr);
}
