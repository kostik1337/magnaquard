#include "hero.h"
#include "markbonus.h"
#include "world.h"

#include <Urho3D/Scene/Node.h>
#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/Physics/PhysicsEvents.h>
#include <Urho3D/IO/Log.h>

MarkBonus::MarkBonus(Context* context) : LogicComponent(context) {
}

void MarkBonus::OnNodeSet(Node* node) {
    if (node) {
        SubscribeToEvent(node, E_NODECOLLISIONSTART, URHO3D_HANDLER(MarkBonus, HandleCollision));
    }
}

void MarkBonus::Update(float timeStep) {
    disabledTimer -= timeStep;
}

void MarkBonus::HandleCollision(StringHash eventType, VariantMap &eventData) {
    Node *otherNode = static_cast<Node*>(eventData[NodeCollisionStart::P_OTHERNODE].GetPtr());
    if (disabledTimer < 0 && otherNode->HasComponent<Hero>()) {
        Vector3 pos = node_->GetWorldPosition() + Vector3(0, 2.8, 0);
        int bullets = 15;
        for (int i=0; i<bullets; ++i) {
            Vector3 dir = Vector3(Cos(i * 180. / bullets), 0, Sin(i * 180. / bullets));
            GetSubsystem<World>()->SpawnBullet(pos, dir, 60, Color(0.25, 1.0, 0.25), true);
        }
        GetSubsystem<World>()->PlaySound("Sound/powerup.wav");

        node_->Remove();
    }
}
