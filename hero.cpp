#include "backgrounds.h"
#include "hero.h"
#include "world.h"
#include <Urho3D/Core/Context.h>
#include <Urho3D/Scene/Node.h>
#include <Urho3D/Scene/SceneEvents.h>
#include <Urho3D/Graphics/AnimatedModel.h>
#include <Urho3D/Graphics/AnimationController.h>
#include <Urho3D/Graphics/AnimationState.h>
#include <Urho3D/Graphics/Animation.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Physics/RigidBody.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/IK/IKEffector.h>
#include <Urho3D/IK/IKSolver.h>

Hero::Hero(Context* context) : LogicComponent(context) {
    SubscribeToEvent(E_SCENEDRAWABLEUPDATEFINISHED, URHO3D_HANDLER(Hero, HandleSceneDrawableUpdateFinished));
    SubscribeToEvent(E_ENEMYDIE, URHO3D_HANDLER(Hero, HandleEnemyDie));
}

void Hero::OnNodeSet(Node* node) {
    if (node) {
        ResourceCache* cache = GetSubsystem<ResourceCache>();
        AnimatedModel *model = node->GetComponent<AnimatedModel>();
        AnimationController* controller = node->CreateComponent<AnimationController>();

        controller->PlayExclusive("Model/Robot/Run.ani", 0, true, 0.0f);

        SubscribeToEvent(node, E_DAMAGEDEAL, URHO3D_HANDLER(Hero, HandleDamageDeal));
    }
}

void Hero::HandleDamageDeal(StringHash eventType, VariantMap &eventData) {
    hp -= eventData[DAMAGEDEAL_DAMAGE].GetFloat();
    GetSubsystem<World>()->PlaySound("Sound/hurt.wav");
}

void Hero::HandleEnemyDie(StringHash /*eventType*/, VariantMap& eventData) {
    energy += eventData[ENEMYDIE_ENERGY].GetFloat();
    if(energy > 100) energy = 100;
    score += eventData[ENEMYDIE_SCORE].GetInt();
    GetSubsystem<World>()->PlaySound("Sound/explosion.wav");
}

void Hero::HandleSceneDrawableUpdateFinished(StringHash /*eventType*/, VariantMap& eventData) {
    Node *shoulder = node_->GetChild("Shoulder.R", true);
    node_->GetChild("Arm.R", true)->GetComponent<IKEffector>()->SetTargetPosition(shoulder->GetWorldPosition() + direction * 2.0);
    shoulder->GetComponent<IKSolver>()->Solve();
}

void Hero::Update(float timeStep) {
    if (node_->GetPosition().x_ < -8 || node_->GetPosition().x_ > 8) {
        Vector3 pos = node_->GetPosition();
        pos.x_ = Clamp(pos.x_, -8.f, 8.f);
        node_->SetPosition(pos);
    }

    if (shootTimer <= 0) {
        if (shooting) {
            GetSubsystem<World>()->SpawnBullet(node_->GetChild("Arm.R", true)->GetWorldPosition(), direction, 20, Color(0.25, 0.25, 1), true);
            shootTimer = maxShootTimer;
            GetSubsystem<World>()->PlaySound("Sound/blaster.wav");
        }
    } else {
        shootTimer -= timeStep;
    }
}

void Hero::SetSpeed(Vector3 speed) {
    node_->GetComponent<RigidBody>()->SetLinearVelocity(speed);
}

void Hero::SetDirection(Vector3 dir, bool relative) {
    if (relative) {
        this->direction = dir;
    } else {
        Node *shoulder = node_->GetChild("Shoulder.R", true);
        this->direction = dir - shoulder->GetWorldPosition();
        this->direction.y_ = 0;

        float angle = direction.Angle(Vector3(0, 0, 1));
        if (angle > 30) {
            direction = Quaternion(direction.x_ > 0? 30: -30, Vector3(0, 1, 0)) * Vector3(0, 0, 1);
        }
    }
    this->direction.Normalize();
}

void Hero::SetShooting(bool shooting) {
    this->shooting = shooting;
}

void Hero::TryActivateBonus() {
    if (energy == 100) {
        GetScene()->GetComponent<Backgrounds>()->CreateBonus(node_->GetPosition());
        energy = 0;
    }
}
