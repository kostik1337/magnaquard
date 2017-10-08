#include "bullet.h"
#include "markbonus.h"
#include "saucer.h"
#include "world.h"
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Graphics/ParticleEffect.h>
#include <Urho3D/Graphics/ParticleEmitter.h>
#include <Urho3D/Physics/RigidBody.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Audio/Sound.h>
#include <Urho3D/Audio/SoundSource.h>

#define FRIENDLY_BULLET_COLLISION_LAYER 2
#define ENEMY_BULLET_COLLISION_LAYER 1

World::World(Context* context) : Object(context) {

}

void World::SetCamera(Camera *camera) {
    this->camera = WeakPtr<Camera>(camera);
}

void World::SetScene(Scene *scene) {
    this->scene = WeakPtr<Scene>(scene);
}

void World::SetHero(Node *hero) {
    this->hero = WeakPtr<Node>(hero);
}

void World::SetSoundSource(SoundSource *effects) {
    this->soundSource = WeakPtr<SoundSource>(effects);
}

void World::SetSoundSourceMusic(SoundSource *music) {
    this->soundSourceMusic = WeakPtr<SoundSource>(music);
}

Pair<Vector2, Vector2> World::GetEnemiesBoundingBox() {
    Vector2 lt(-8, 8);
    Vector2 rb(8, 0);
    return Pair<Vector2, Vector2>(lt, rb);
}

Node *World::SpawnBullet(Vector3 position, Vector3 direction, float damage, Color color, bool friendly) {
    if (SharedPtr<Scene> scenePtr = scene.Lock()) {
        Node* bullet = scenePtr->GetChild("BulletPrefab")->Clone();
        bullet->SetPosition(position);

        SharedPtr<ParticleEffect> effect = bullet->GetComponent<ParticleEmitter>()->GetEffect()->Clone();
        ColorFrame frame(*effect->GetColorFrame(0));
        frame.color_ = color;
        effect->SetColorFrame(0, frame);
        bullet->GetComponent<ParticleEmitter>()->SetEffect(effect);

        bullet->GetComponent<RigidBody>()->SetCollisionMask(friendly ? FRIENDLY_BULLET_COLLISION_LAYER : ENEMY_BULLET_COLLISION_LAYER);

        Bullet* bulletComponent = bullet->CreateComponent<Bullet>();
        bulletComponent->SetParams(direction, damage, color);
        return bullet;
    }
    return nullptr;
}

Node *World::SpawnSingleEnemy() {
    if (SharedPtr<Scene> scenePtr = scene.Lock()) {
        Node* enemy = scenePtr->GetChild("EnemyPrefab")->Clone();
        return enemy;
    }
    return nullptr;
}

Node* World::SpawnSaucer() {
    if (SharedPtr<Scene> scenePtr = scene.Lock()) {
        Node* enemy = scenePtr->GetChild("SaucerPrefab")->Clone();
        enemy->CreateComponent<Saucer>();
        return enemy;
    }
    return nullptr;
}

Node* World::SpawnBonus(Vector3 position) {
    if (SharedPtr<Scene> scenePtr = scene.Lock()) {
        Node* bonus = scenePtr->GetChild("MarkPrefab")->Clone();
        bonus->SetPosition(position);
        bonus->CreateComponent<MarkBonus>();
        return bonus;
    }
    return nullptr;
}

SharedPtr<Camera> World::GetCamera() {
    return camera.Lock();
}

SharedPtr<Node> World::GetHero() {
    return hero.Lock();
}

SharedPtr<SoundSource> World::GetSoundSource() {
    return soundSource.Lock();
}

void World::PlaySound(String file) {
    ResourceCache *cache = GetSubsystem<ResourceCache>();
    GetSoundSource()->Play(cache->GetResource<Sound>(file));
}

void World::PlayMusic(String file) {
    ResourceCache *cache = GetSubsystem<ResourceCache>();
    soundSourceMusic.Lock()->Play(cache->GetResource<Sound>(file));
}




