#ifndef UICONTROLLER_H
#define UICONTROLLER_H

#include <Urho3D/Urho3D.h>
#include <Urho3D/Scene/Component.h>
#include <Urho3D/UI/UIElement.h>

using namespace Urho3D;

#define PRE_GAME_SCREEN 0
#define GAME_SCREEN 1
#define DEATH_SCREEN 2

class UIController: public Component
{
    URHO3D_OBJECT(UIController, Component);
private:
    int screen;
    SharedPtr<UIElement> uiRoot;
    void HandleUpdate(StringHash eventType, VariantMap &eventData);
public:
    void SetScreen(int screen);
    UIController(Context* context);
};

#endif // UICONTROLLER_H
