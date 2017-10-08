#ifndef MARKBONUS_H
#define MARKBONUS_H

#include <Urho3D/Urho3D.h>
#include <Urho3D/Scene/Component.h>
#include <Urho3D/Scene/LogicComponent.h>

using namespace Urho3D;

class MarkBonus : public LogicComponent
{
    URHO3D_OBJECT(MarkBonus, LogicComponent);
private:
    void HandleCollision(StringHash eventType, VariantMap &eventData);
public:
    float disabledTimer = 3.0f;
    MarkBonus(Context* context);
    void OnNodeSet(Node *node);
    void Update(float timeStep);
};

#endif // MARKBONUS_H
