#include "../include/test_obstacle.h"
#include "../include/ledge.h"

#include "../include/world.h"

dbasic::ModelAsset *c_adv::TestObstacle::m_obstacleMesh = nullptr;

c_adv::TestObstacle::TestObstacle() {
    m_ledge0 = m_ledge1 = m_ledge2 = m_ledge3 = nullptr;
}

c_adv::TestObstacle::~TestObstacle() {
    /* void */
}

void c_adv::TestObstacle::initialize() {
    GameObject::initialize();

    RigidBody.SetHint(dphysics::RigidBody::RigidBodyHint::Dynamic);
    RigidBody.SetInverseMass(0.0f);

    dphysics::CollisionObject *bounds;
    RigidBody.CollisionGeometry.NewBoxObject(&bounds);
    bounds->SetMode(dphysics::CollisionObject::Mode::Fine);
    bounds->GetAsBox()->HalfHeight = 1.0f;
    bounds->GetAsBox()->HalfWidth = 1.0f;
    bounds->GetAsBox()->Orientation = ysMath::Constants::QuatIdentity;
    bounds->GetAsBox()->Position = ysMath::Constants::Zero;

    m_ledge0 = m_realm->spawn<Ledge>();
    m_ledge1 = m_realm->spawn<Ledge>();
    m_ledge2 = m_realm->spawn<Ledge>();
    m_ledge3 = m_realm->spawn<Ledge>();

    m_ledge0->RigidBody.Transform.SetPosition(ysMath::LoadVector(1.0f, 1.0f, 0.0f));
    m_ledge1->RigidBody.Transform.SetPosition(ysMath::LoadVector(-1.0f, 1.0f, 0.0f));
    m_ledge2->RigidBody.Transform.SetPosition(ysMath::LoadVector(1.0f, -1.0f, 0.0f));
    m_ledge3->RigidBody.Transform.SetPosition(ysMath::LoadVector(-1.0f, -1.0f, 0.0f));

    RigidBody.AddChild(&m_ledge0->RigidBody);
    RigidBody.AddChild(&m_ledge1->RigidBody);
    RigidBody.AddChild(&m_ledge2->RigidBody);
    RigidBody.AddChild(&m_ledge3->RigidBody);
}

void c_adv::TestObstacle::destroy() {
    m_ledge0->setDeletionFlag();
    m_ledge1->setDeletionFlag();
    m_ledge2->setDeletionFlag();
    m_ledge3->setDeletionFlag();
}

void c_adv::TestObstacle::render() {
    m_world->getShaders().SetObjectTransform(RigidBody.Transform.GetWorldTransform());
    m_world->getShaders().ResetBrdfParameters();
    m_world->getShaders().SetBaseColor(ysColor::srgbiToLinear(255, 255, 255));
    m_world->getShaders().ConfigureModel(1.0f, m_obstacleMesh);
    m_world->getEngine().DrawModel(m_world->getShaders().GetRegularFlags(), m_obstacleMesh);
}

void c_adv::TestObstacle::getAssets(dbasic::AssetManager *am) {
    m_obstacleMesh = am->GetModelAsset("TestObstacle");
}
