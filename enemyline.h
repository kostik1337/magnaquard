#ifndef ENEMYLINE_H
#define ENEMYLINE_H

#include <Urho3D/Urho3D.h>
#include <Urho3D/Scene/LogicComponent.h>
#include <Urho3D/Scene/Node.h>
#include <Urho3D/Core/Spline.h>

using namespace Urho3D;

struct EnemyInfo {
    Node* node;
    int displacement;
    float hp;
};

class EnemyLine : public LogicComponent
{
    URHO3D_OBJECT(EnemyLine, LogicComponent);
private:
    Vector<EnemyInfo> enemies;
    Vector<Vector2> controlPoints;
    float currentMovingTime = 0;
    float shootingTime = 0;
    constexpr static float maxShootTimer = 1.5f;

    Vector2 InterpolateControlPoint(float f);
    void HandleDamageDeal(StringHash eventType, VariantMap &eventData);
    void GenerateControlPoints();
public:
    EnemyLine(Context* context);
    virtual void OnNodeSet(Node* node);
    virtual void Update(float timeStep);
};

#endif // ENEMYLINE_H
