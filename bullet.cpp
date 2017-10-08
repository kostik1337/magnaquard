#include "bullet.h"
#include "world.h"
#include <Urho3D/Physics/RigidBody.h>
#include <Urho3D/Physics/PhysicsEvents.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/Graphics/ParticleEffect.h>
#include <Urho3D/Graphics/ParticleEmitter.h>

Bullet::Bullet(Context* context) : LogicComponent(context) {
}

void Bullet::OnNodeSet(Node* node) {
    if (node) {
        SubscribeToEvent(node, E_NODECOLLISIONSTART, URHO3D_HANDLER(Bullet, HandleCollision));
    }
}

void Bullet::HandleCollision(StringHash eventType, VariantMap &eventData) {
    Node *otherNode = static_cast<Node*>(eventData[NodeCollisionStart::P_OTHERNODE].GetPtr());
    if(otherNode->HasComponent<Bullet>()) return;

    VariantMap map;
    map[DAMAGEDEAL_DAMAGE] = damage;
    map[DAMAGEDEAL_NODE] = otherNode;
    otherNode->SendEvent(E_DAMAGEDEAL, map);
    node_->Remove();
}

void Bullet::Update(float timeStep) {
    if (!GetSubsystem<World>()->GetCamera()->GetFrustum().IsInside(node_->GetPosition())) {
        node_->Remove();
    }
}

void Bullet::SetParams(Vector3 direction, float damage, Color color) {
    double speed = 15.0f;
    this->damage = damage;
    node_->GetComponent<RigidBody>()->SetLinearVelocity(direction * speed);
}

