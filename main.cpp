#include "backgrounds.h"
#include "bullet.h"
#include "enemyline.h"
#include "hero.h"
#include "markbonus.h"
#include "saucer.h"
#include "spawncontroller.h"
#include "uicontroller.h"
#include "world.h"

#include <Urho3D/Input/Input.h>
#include <Urho3D/Input/InputEvents.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/Engine/Application.h>
#include <Urho3D/Engine/Engine.h>
#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Graphics/DebugRenderer.h>
#include <Urho3D/Graphics/Octree.h>
#include <Urho3D/Graphics/Graphics.h>
#include <Urho3D/Graphics/Camera.h>
#include <Urho3D/Graphics/Viewport.h>
#include <Urho3D/Graphics/Renderer.h>
#include <Urho3D/Graphics/Material.h>
#include <Urho3D/Graphics/Technique.h>
#include <Urho3D/UI/UI.h>
#include <Urho3D/Physics/PhysicsWorld.h>
#include <Urho3D/Graphics/DebugRenderer.h>
#include <Urho3D/Graphics/StaticModel.h>
#include <Urho3D/IK/IKEffector.h>
#include <Urho3D/IK/IKSolver.h>
#include <Urho3D/Audio/Sound.h>
#include <Urho3D/Audio/SoundSource.h>

using namespace Urho3D;

class MyApp : public Application {
    SharedPtr<Node> cameraNode;
    SharedPtr<Scene> scene;
    WeakPtr<Hero> hero;
    bool isFuckUp = false;
    bool isGameStarted = false;
public:
    MyApp(Context *context) : Application(context) {
    }

    virtual void Setup() {
        engineParameters_["LogLevel"] = "LOG_DEBUG";
        engineParameters_["WindowWidth"] = 1024;
        engineParameters_["WindowHeight"] = 768;
        engineParameters_["FullScreen"] = false;
        engineParameters_["ResourcePrefixPaths"] = "../..";
    }

    void RegisterFactories() {
        context_->RegisterFactory<Hero>();
        context_->RegisterFactory<Backgrounds>();
        context_->RegisterFactory<Bullet>();
        context_->RegisterFactory<EnemyLine>();
        context_->RegisterFactory<Saucer>();
        context_->RegisterFactory<SpawnController>();
        context_->RegisterFactory<UIController>();
        context_->RegisterFactory<MarkBonus>();
        context_->RegisterSubsystem(new World(context_));
    }

    void CreateCursor() {
        /*ResourceCache* cache = GetSubsystem<ResourceCache>();

        UI* ui = GetSubsystem<UI>();

        XMLFile* style = cache->GetResource<XMLFile>("UI/DefaultStyle.xml");
        SharedPtr<Cursor> cursor(new Cursor(context_));
        cursor->SetStyleAuto(style);
        ui->SetCursor(cursor);
        // Set starting position of the cursor at the rendering window center
        Graphics* graphics = GetSubsystem<Graphics>();
        cursor->SetPosition(graphics->GetWidth() / 2, graphics->GetHeight() / 2);*/
    }

    void LoadEverything() {
        SetRandomSeed(GetSubsystem<Time>()->GetSystemTime());
        ResourceCache* cache = GetSubsystem<ResourceCache>();

        scene = new Scene(context_);
        GetSubsystem<Input>()->SetMouseVisible(true);
        SharedPtr<File> file = cache->GetFile("Scene.xml");
        scene->LoadXML(*file);
        scene->CreateComponent<DebugRenderer>();

        cameraNode = scene->GetChild("CameraNode", true);
        Camera* camera = cameraNode->GetComponent<Camera>();

        Graphics* graphics = GetSubsystem<Graphics>();

        Renderer* renderer = GetSubsystem<Renderer>();
        SharedPtr<Viewport> viewport(new Viewport(context_, scene, camera));
        renderer->SetViewport(0, viewport);

        context_->GetSubsystem<World>()->SetCamera(camera);
        context_->GetSubsystem<World>()->SetScene(scene);

        Node* heroNode = scene->GetChild("Hero", true);
        hero = WeakPtr<Hero>(heroNode->CreateComponent<Hero>());
        GetSubsystem<World>()->SetHero(heroNode);

        scene->CreateComponent<Backgrounds>();
        UIController* controller = scene->CreateComponent<UIController>();
        controller->SetScreen(isFuckUp? DEATH_SCREEN : PRE_GAME_SCREEN);

        Node* soundNode = scene->CreateChild("Sound");
        SoundSource *source = soundNode->CreateComponent<SoundSource>();
        GetSubsystem<World>()->SetSoundSource(source);


        Node* musicNode = scene->CreateChild("Music");
        SoundSource *musicSource = musicNode->CreateComponent<SoundSource>();
        GetSubsystem<World>()->SetSoundSourceMusic(musicSource);

        GetSubsystem<World>()->PlayMusic("Music/Pistol Jazz - God Speed.ogg");
    }

    void OnGameStart() {
        URHO3D_LOGDEBUGF("OnGameStart");
        isGameStarted = true;
        scene->CreateComponent<SpawnController>();

        UIController* controller = scene->GetComponent<UIController>();
        controller->SetScreen(GAME_SCREEN);
    }

    virtual void Start() {
        RegisterFactories();
        CreateCursor();
        LoadEverything();

        SubscribeToEvent(E_UPDATE, URHO3D_HANDLER(MyApp, HandleUpdate));
        SubscribeToEvent(E_KEYUP, URHO3D_HANDLER(MyApp, HandleKey));
        SubscribeToEvent(E_KEYDOWN, URHO3D_HANDLER(MyApp, HandleKey));
        SubscribeToEvent(E_MOUSEBUTTONDOWN, URHO3D_HANDLER(MyApp, HandleMouse));
        SubscribeToEvent(E_MOUSEBUTTONUP, URHO3D_HANDLER(MyApp, HandleMouse));
        SubscribeToEvent(E_POSTRENDERUPDATE, URHO3D_HANDLER(MyApp, HandlePostUpdate));
    }

    Vector3 Raycast() {
        UI* ui = GetSubsystem<UI>();
        IntVector2 pos = ui->GetCursorPosition();
        if (ui->GetElementAt(pos, true))
            return Vector3();

        Graphics* graphics = GetSubsystem<Graphics>();
        Camera* camera = cameraNode->GetComponent<Camera>();
        Ray cameraRay = camera->GetScreenRay((float)pos.x_ / graphics->GetWidth(), (float)pos.y_ / graphics->GetHeight());

        StaticModel* raycastSolver = scene->GetChild("RaycastSolver", true)->GetComponent<StaticModel>();
        // Pick only geometry objects, not eg. zones or lights, only get the first (closest) hit
        PODVector<RayQueryResult> results;
        RayOctreeQuery query(results, cameraRay, RAY_TRIANGLE, 100, DRAWABLE_GEOMETRY);

        raycastSolver->ProcessRayQuery(query, results);
//        scene->GetComponent<Octree>()->RaycastSingle(query);
        if (results.Size()) {
            RayQueryResult& result = results[0];
            return result.position_;
        }

        return Vector3();
    }

    void HandlePostUpdate(StringHash eventType, VariantMap &eventData) {
        //DebugRenderer* renderer = scene->GetComponent<DebugRenderer>();
//        scene->GetComponent<PhysicsWorld>()->DrawDebugGeometry(false);

//        if (SharedPtr<Hero> ptr = hero.Lock()) {
//            ptr->GetNode()->GetChild("Arm.R", true)->GetComponent<IKEffector>()->DrawDebugGeometry(false);
//            ptr->GetNode()->GetChild("Shoulder.R", true)->GetComponent<IKSolver>()->DrawDebugGeometry(false);
//        }
    }

    void HandleMouse(StringHash eventType, VariantMap &eventData) {
        if (isGameStarted && eventData[MouseButtonDown::P_BUTTON].GetInt() == MOUSEB_LEFT)
            if (SharedPtr<Hero> ptr = hero.Lock()) {
                ptr->SetShooting(eventType == E_MOUSEBUTTONDOWN);
            }
    }

    void HandleUpdate(StringHash eventType, VariantMap &eventData) {
        UI* ui = GetSubsystem<UI>();
        Input* input = GetSubsystem<Input>();
        Vector3 cast = Raycast();

        if (SharedPtr<Hero> ptr = hero.Lock()) {
            ptr->SetDirection(cast, false);
        }

        if (SharedPtr<Hero> ptr = hero.Lock()) {
            if (ptr->GetHP() <= 0) {
                isFuckUp = true;
                isGameStarted = false;
                scene->RemoveAllChildren();
                LoadEverything();
                GetSubsystem<World>()->PlayMusic("Music/Pistol Jazz - Spring Snow.ogg");
            }
        }
    }

    void HandleKey(StringHash eventType, VariantMap &eventData) {
        if (isGameStarted) {
            Input* input = GetSubsystem<Input>();
            const float MOVE_SPEED = 3.0f;
            Vector3 speed;
            if (input->GetKeyDown(KEY_W))
                speed += (Vector3::FORWARD * MOVE_SPEED);
            if (input->GetKeyDown(KEY_S))
                speed += (Vector3::BACK * MOVE_SPEED);
            if (input->GetKeyDown(KEY_A))
                speed += (Vector3::LEFT * MOVE_SPEED);
            if (input->GetKeyDown(KEY_D))
                speed += (Vector3::RIGHT * MOVE_SPEED);


            if (SharedPtr<Hero> ptr = hero.Lock()) {
                ptr->SetSpeed(speed);
            }
        }

        if (eventType == E_KEYUP && eventData[KeyUp::P_KEY].GetInt() == KEY_ESCAPE) {
            engine_->Exit();
        } else if(eventType == E_KEYUP && eventData[KeyUp::P_KEY].GetInt() == KEY_SPACE) {
            if (!isGameStarted)  {
                if (isFuckUp)
                    GetSubsystem<World>()->PlayMusic("Music/Pistol Jazz - God Speed.ogg");
                OnGameStart();
            } else
                hero->TryActivateBonus();
        }
    }
};

URHO3D_DEFINE_APPLICATION_MAIN(MyApp)
