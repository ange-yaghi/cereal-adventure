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
    m_world->getShaders().SetObjectTransform(RigidBody.Transform.GetWorldTransform());
    m_world->getShaders().ConfigureModel(1.0f, m_asset);
    m_world->getEngine().DrawModel(m_world->getShaders().GetRegularFlags(), m_asset);
}
