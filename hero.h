#ifndef HERO_H
#define HERO_H

#include <Urho3D/Urho3D.h>
#include <Urho3D/Scene/LogicComponent.h>

using namespace Urho3D;

class Hero : public LogicComponent
{
    URHO3D_OBJECT(Hero, LogicComponent);
public:
    Hero(Context* context);
    virtual void OnNodeSet(Node* node);
    virtual void Update(float timeStep);
    void SetSpeed(Vector3 speed);
    void SetDirection(Vector3 dir, bool relative);
    void SetShooting(bool shooting);
    float GetHP() {
        return hp;
    }
    float GetEnergy() {
        return energy;
    }
    float GetScore() {
        return score;
    }
    void TryActivateBonus();

private:
    float hp = 100;
    float energy = 0;
    int score = 0;
    Vector3 direction;
    bool shooting = false;
    float shootTimer = 0.0;
    constexpr static float maxShootTimer = 0.2;
    void HandleSceneDrawableUpdateFinished(StringHash, VariantMap &eventData);
    void HandleEnemyDie(StringHash, VariantMap &eventData);
    void HandleDamageDeal(StringHash eventType, VariantMap &eventData);
};

#endif // HERO_H
