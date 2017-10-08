#ifndef SPAWNCONTROLLER_H
#define SPAWNCONTROLLER_H

#include <Urho3D/Urho3D.h>
#include <Urho3D/Scene/LogicComponent.h>
#include <Urho3D/Scene/Node.h>

using namespace Urho3D;

class SpawnController : public LogicComponent
{
    URHO3D_OBJECT(SpawnController, LogicComponent);
public:
    float enemyLineTimer = 0.0f;
    float saucerTimer = 0.0f;
    constexpr static float enemyLineTimerMin = 5.0f;
    constexpr static float enemyLineTimerMax = 10.0f;
    constexpr static float saucerTimerMin = 8.0f;
    constexpr static float saucerTimerMax = 16.0f;
    SpawnController(Context* context);
    virtual void OnNodeSet(Node* node);
    virtual void Update(float timeStep);
};

#endif // SPAWNCONTROLLER_H
