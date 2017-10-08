#ifndef BACKGROUNDS_H
#define BACKGROUNDS_H

#include <Urho3D/Urho3D.h>
#include <Urho3D/Scene/LogicComponent.h>
#include <Urho3D/Scene/Node.h>

using namespace Urho3D;

class Backgrounds : public LogicComponent
{
    URHO3D_OBJECT(Backgrounds, LogicComponent);
private:
    const int backingSize = 10;
    Vector<Node*> backings;
    double fieldRepeatSize = 160;
    Vector<WeakPtr<Node>> bonuses;
protected:
    void OnNodeSet(Node *node);
public:
    Backgrounds(Context* context);
    void Update(float timeStep);
    void CreateBonus(Vector3 position);
};

#endif // BACKGROUNDS_H
