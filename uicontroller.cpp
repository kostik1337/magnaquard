#include "hero.h"
#include "uicontroller.h"
#include "world.h"

#include <Urho3D/UI/UI.h>
#include <Urho3D/UI/Sprite.h>
#include <Urho3D/UI/Text.h>
#include <Urho3D/Core/Context.h>
#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/Scene/Node.h>
#include <Urho3D/Resource/ResourceCache.h>

UIController::UIController(Context *context)
    : Component(context) {
    UI* ui = context_->GetSubsystem<UI>();
    uiRoot = ui->GetRoot();

    SubscribeToEvent(E_UPDATE, URHO3D_HANDLER(UIController, HandleUpdate));
}

void UIController::HandleUpdate(StringHash eventType, VariantMap &eventData) {
    if (screen == GAME_SCREEN) {
        Hero* hero = GetSubsystem<World>()->GetHero()->GetComponent<Hero>();
        float hp = hero->GetHP();
        uiRoot->GetChildStaticCast<Sprite>("HP", true)->SetWidth((int)(300.f * hp / 100.f));
//        uiRoot->GetChildStaticCast<Text>("HPText", true)->SetText(String((int) hp));

        float energy = hero->GetEnergy();
        Sprite* energyBar = uiRoot->GetChildStaticCast<Sprite>("Energy", true);
        energyBar->SetColor(energy == 100? Color::YELLOW : Color::GRAY);
        energyBar->SetWidth((int)(300.f * energy / 100.f));
        uiRoot->GetChildStaticCast<Text>("Score", true)->SetText(String((int)hero->GetScore()));
    }
}

void UIController::SetScreen(int screen) {
    this->screen = screen;

    uiRoot->RemoveAllChildren();

    UI* ui = context_->GetSubsystem<UI>();
    ResourceCache* cache = GetSubsystem<ResourceCache>();
    XMLFile* style = cache->GetResource<XMLFile>("UI/DefaultStyle.xml");
    SharedPtr<UIElement> elem = ui->LoadLayout(cache->GetResource<XMLFile>(screen == PRE_GAME_SCREEN? "UI/MyUIStart.xml":
                screen == GAME_SCREEN? "UI/MyUI.xml": "UI/MyUILose.xml"), style);
    uiRoot->AddChild(elem);
}
