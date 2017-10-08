#include "enemyline.h"
#include "spawncontroller.h"
#include "world.h"
#include <Urho3D/Scene/Scene.h>

SpawnController::SpawnController(Context* context) : LogicComponent(context) {

}

void SpawnController::OnNodeSet(Node* node) {
    if (node) {
        enemyLineTimer = 0;//Random(enemyLineTimerMin, enemyLineTimerMax);
        saucerTimer = Random(saucerTimerMin, saucerTimerMax);
    }
}

void SpawnController::Update(float timeStep) {
    if (enemyLineTimer < 0.0f) {
        enemyLineTimer = Random(enemyLineTimerMin, enemyLineTimerMax);
        GetScene()->CreateChild()->CreateComponent<EnemyLine>();
    }
    if (saucerTimer < 0.0f) {
        saucerTimer = Random(saucerTimerMin, saucerTimerMax);
        GetSubsystem<World>()->SpawnSaucer();
    }
    enemyLineTimer -= timeStep;
    saucerTimer -= timeStep;
}
