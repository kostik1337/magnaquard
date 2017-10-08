#include "enemyline.h"
#include "world.h"
#include <Urho3D/IO/Log.h>

EnemyLine::EnemyLine(Context* context) : LogicComponent(context) {
    SubscribeToEvent(E_DAMAGEDEAL, URHO3D_HANDLER(EnemyLine, HandleDamageDeal));
}

void EnemyLine::HandleDamageDeal(StringHash eventType, VariantMap &eventData) {
    Node *node = static_cast<Node*>(eventData[DAMAGEDEAL_NODE].GetPtr());
    for (auto it = enemies.Begin(); it < enemies.End();) {
        EnemyInfo& info = *it;
        if (info.node == node) {
            info.hp -= eventData[DAMAGEDEAL_DAMAGE].GetFloat();
            if (info.hp <= 0) {
                info.node->Remove();
                it = enemies.Erase(it);

                VariantMap map;
                map[ENEMYDIE_ENERGY] = 20;
                map[ENEMYDIE_SCORE] = 50000000;
                SendEvent(E_ENEMYDIE, map);

                if (enemies.Size() == 0) {
                    node_->Remove();
                }
                continue;
            }
        }
        ++it;
    }
}

void EnemyLine::OnNodeSet(Node* node) {
    if (node) {
        for (int i = 0; i < 5; ++i) {
            Node *node = GetSubsystem<World>()->SpawnSingleEnemy();
            EnemyInfo info;
            info.node = node;
            info.displacement = i;
            info.hp = 40;
            enemies.Push(info);
        }

        Pair<Vector2, Vector2> pair = GetSubsystem<World>()->GetEnemiesBoundingBox();
        Vector2 lt = pair.first_;
        Vector2 rb = pair.second_;
        controlPoints.Push(Vector2(Random(lt.x_, rb.x_), pair.first_.y_ + 3));
        GenerateControlPoints();
    }
}

void EnemyLine::GenerateControlPoints() {
    Pair<Vector2, Vector2> pair = GetSubsystem<World>()->GetEnemiesBoundingBox();
    Vector2 lt = pair.first_;
    Vector2 rb = pair.second_;
    bool horiz = false;
    Vector2 lastCoord = controlPoints[controlPoints.Size()-1];
    for (int i=0; i < 10; ++i) {
        float newCoord;
        newCoord = horiz ? Random(lt.x_, rb.x_): Random(lt.y_, rb.y_);
        lastCoord = horiz? Vector2(newCoord, lastCoord.y_) : Vector2(lastCoord.x_, newCoord);
        controlPoints.Push(lastCoord);
        horiz = !horiz;
    }
}

Vector2 EnemyLine::InterpolateControlPoint(float f) {
    int current = (int) f;
    int next = current + 1;
    float fract = f - current;
    return controlPoints[current] * (1. - fract) + controlPoints[next] * fract;
}

void EnemyLine::Update(float timeStep) {
    currentMovingTime += timeStep;
    for (EnemyInfo& info: enemies) {
        float f = currentMovingTime * 0.5f - info.displacement * 0.4;
        if (f >= 0) {
            if ((int)f > controlPoints.Size() - 1) {
                GenerateControlPoints();
            }
            Vector2 interpolated = InterpolateControlPoint(f);
            info.node->SetPosition(Vector3(interpolated.x_, 2.8 + Sin(f * 600.f) * 0.3, interpolated.y_));
        }
    }

    shootingTime -= timeStep;
    if (shootingTime <= 0) {
        shootingTime = maxShootTimer;
        Node* randEnemy = enemies[Random((int) enemies.Size())].node;
        SharedPtr<Node> hero = GetSubsystem<World>()->GetHero();
        Vector3 dir = hero->GetWorldPosition() - randEnemy->GetWorldPosition();
        dir.y_ = 0;
        dir.Normalize();

        GetSubsystem<World>()->SpawnBullet(randEnemy->GetWorldPosition(), dir, 10, Color(1, 0.25, 0.25), false);
    }
}
