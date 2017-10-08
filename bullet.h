#ifndef BULLET_H
#define BULLET_H

#include <Urho3D/Urho3D.h>
#include <Urho3D/Scene/LogicComponent.h>
#include <Urho3D/Scene/Scene.h>

using namespace Urho3D;

class Bullet : public LogicComponent
{
    URHO3D_OBJECT(Bullet, LogicComponent);
private:
    float damage;
    void HandleCollision(StringHash eventType, VariantMap &eventData);
public:
    Bullet(Context* context);
    virtual void OnNodeSet(Node* node);
    virtual void Update(float timeStep);
    void SetParams(Vector3 direction, float damage, Color color);
};

#endif // BULLET_H
