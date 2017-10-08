#ifndef WORLD_H
#define WORLD_H

#include <Urho3D/Urho3D.h>
#include <Urho3D/Core/Object.h>
#include <Urho3D/Graphics/Camera.h>
#include <Urho3D/Audio/SoundSource.h>

#define E_DAMAGEDEAL StringHash("DamageDeal")
#define DAMAGEDEAL_DAMAGE StringHash("Damage")
#define DAMAGEDEAL_NODE StringHash("Node")

#define E_ENEMYDIE StringHash("EnemyDie")
#define ENEMYDIE_SCORE StringHash("Score")
#define ENEMYDIE_ENERGY StringHash("Energy")

using namespace Urho3D;

class World : public Object
{
URHO3D_OBJECT(World, Object);
private:
    WeakPtr<Camera> camera;
    WeakPtr<Scene> scene;
    WeakPtr<Node> hero;
    WeakPtr<SoundSource> soundSource;
    WeakPtr<SoundSource> soundSourceMusic;
public:
    World(Context* context);
    void SetCamera(Camera* camera);
    void SetScene(Scene* scene);
    void SetHero(Node* hero);
    void SetSoundSource(SoundSource *effects);
    void SetSoundSourceMusic(SoundSource *music);
    Pair<Vector2, Vector2> GetEnemiesBoundingBox();
    SharedPtr<Camera> GetCamera();
    SharedPtr<Node> GetHero();
    SharedPtr<SoundSource> GetSoundSource();
    void PlaySound(String file);
    Node *SpawnBullet(Vector3 position, Vector3 direction, float damage, Color color, bool friendly);
    Node *SpawnSingleEnemy();
    Node *SpawnSaucer();
    Node *SpawnBonus(Vector3 position);
    void PlayMusic(String file);
};

#endif // WORLD_H
