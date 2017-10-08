#ifndef SAUCER_H
#define SAUCER_H

#include <Urho3D/Urho3D.h>
#include <Urho3D/Scene/LogicComponent.h>
#include <Urho3D/Scene/Node.h>

using namespace Urho3D;

class Saucer : public LogicComponent
{
    URHO3D_OBJECT(Saucer, LogicComponent);
private:
    Vector3 force;
    bool isForceApplying = false;
    float timer;
    float shootingTime = 0;
    float hp = 160;
    constexpr static float maxShootTimer = 0.7f;
    void HandleDamageDeal(StringHash eventType, VariantMap &eventData);
public:
    Saucer(Context* context);
    virtual void OnNodeSet(Node* node);
    virtual void Update(float timeStep);
};

#endif // SAUCER_H
