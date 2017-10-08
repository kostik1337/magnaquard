#include "saucer.h"
#include "world.h"

#include <Urho3D/Physics/RigidBody.h>
#include <Urho3D/IO/Log.h>

Saucer::Saucer(Context* context) : LogicComponent(context) {

}

void Saucer::OnNodeSet(Node* node) {
    if (node) {
        Pair<Vector2, Vector2> bbox = GetSubsystem<World>()->GetEnemiesBoundingBox();
        node->SetPosition(Vector3(Random(bbox.first_.x_, bbox.second_.x_), 2.8, bbox.first_.y_ + 3));
        node_->GetComponent<RigidBody>()->SetLinearVelocity(Vector3(0,0,-5));

        node->SubscribeToEvent(E_DAMAGEDEAL, URHO3D_HANDLER(Saucer, HandleDamageDeal));
    }
}

void Saucer::HandleDamageDeal(StringHash eventType, VariantMap &eventData) {
    hp -= eventData[DAMAGEDEAL_DAMAGE].GetFloat();
    if(hp <= 0) {
        node_->Remove();

        VariantMap map;
        map[ENEMYDIE_ENERGY] = 40.f;
        map[ENEMYDIE_SCORE] = 150000000;
        SendEvent(E_ENEMYDIE, map);
    }
}

void Saucer::Update(float timeStep) {
    shootingTime -= timeStep;
    if (shootingTime <= 0) {
        shootingTime = maxShootTimer;

        for (int i=0; i<3; ++i) {
            Vector3 dir = Vector3(i-1, 0, -1);
            dir.Normalize();
            GetSubsystem<World>()->SpawnBullet(node_->GetWorldPosition(), dir, 20, Color(1, 0.25, 0.25), false);
        }
    }

    if(node_->GetPosition().z_ < -13) node_->Remove();
}
