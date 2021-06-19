#include "../include/world.h"

#include "../include/asset_loader.h"
#include "../include/realm.h"
#include "../include/player.h"
#include "../include/test_obstacle.h"
#include "../include/game_objects.h"

#include <map>
#include <stack>

const float c_adv::World::DefaultCameraDistance = 10.0f;
const std::string c_adv::World::PhysicsTimer = "Physics";

c_adv::World::World() {
    m_focus = nullptr;
    m_mainRealm = nullptr;
    m_cameraDistance = DefaultCameraDistance;
    m_respawnPosition = ysMath::Constants::Zero;
    m_intermediateRenderTarget = nullptr;
    m_uiStageFlags = 0x0;
}

c_adv::World::~World() {
    /* void */
}

void c_adv::World::initialize(void *instance, ysContextObject::DeviceAPI api) {
    dbasic::Path modulePath = dbasic::GetModulePath();
    dbasic::Path confPath = modulePath.Append("delta.conf");

    std::string enginePath = "../../dependencies/delta/engines/basic";
    std::string assetPath = "../../assets";
    std::string loggingPath = "../../workspace";
    std::string shaderPath;
    if (confPath.Exists()) {
        std::fstream confFile(confPath.ToString(), std::ios::in);

        std::getline(confFile, enginePath);
        std::getline(confFile, assetPath);
        enginePath = modulePath.Append(enginePath).ToString();
        assetPath = modulePath.Append(assetPath).ToString();
        loggingPath = modulePath.ToString();

        confFile.close();
    }

    shaderPath = enginePath + "/shaders/";
    m_assetPath = dbasic::Path(assetPath);

    m_engine.GetConsole()->SetDefaultFontDirectory(enginePath + "/fonts/");

    dbasic::DeltaEngine::GameEngineSettings settings;
    settings.API = api;
    settings.DepthBuffer = true;
    settings.FrameLogging = true;
    settings.Instance = instance;
    settings.LoggingDirectory = loggingPath.c_str();
    settings.ShaderDirectory = shaderPath.c_str();
    settings.WindowTitle = "Cereal Adventure // Development Build // " __DATE__;

    // Create timers
    m_engine.GetBreakdownTimer().CreateChannel(PhysicsTimer);

    m_engine.CreateGameWindow(settings);

    m_assetManager.SetEngine(&m_engine);
    AssetLoader::loadAllAssets(dbasic::Path(assetPath), &m_assetManager);

    // Camera settings
    m_shaders.SetCameraMode(Shaders::CameraMode::Target);

    m_smoothCamera.setPosition(ysMath::Constants::Zero);
    m_smoothCamera.setStiffnessTensor(ysMath::LoadVector(200.0f, 50.0f, 0.0f));
    m_smoothCamera.setDampingTensor(ysMath::LoadVector(0.3f, 0.5f, 0.0f));

    m_smoothTarget.setPosition(ysMath::Constants::Zero);
    m_smoothTarget.setStiffnessTensor(ysMath::LoadVector(250.0f, 100.0f, 0.0f));
    m_smoothTarget.setDampingTensor(ysMath::LoadVector(0.3f, 0.5f, 0.0f));

    m_engine.SetCursorHidden(false);
    m_engine.SetCursorPositionLock(false);

    // Shaders
    // PS1 - 512x240
    // PS2 - 720x480
    ysDevice *device = m_engine.GetDevice();
    device->CreateOffScreenRenderTarget(&m_intermediateRenderTarget, 1920, 1080, ysRenderTarget::Format::R8G8B8A8_UNORM);
    device->CreateOffScreenRenderTarget(&m_guiRenderTarget, 512, 240, ysRenderTarget::Format::R8G8B8A8_UNORM, true, false);

    Shaders::Context shaderContext{};
    shaderContext.Device = m_engine.GetDevice();
    shaderContext.GeometryFormat = m_engine.GetGeometryFormat();
    shaderContext.RenderTarget = m_intermediateRenderTarget;
    shaderContext.UiRenderTarget = m_guiRenderTarget;
    shaderContext.ShaderPath = assetPath + "/shaders/";
    shaderContext.ShaderSet = &m_shaderSet;
    shaderContext.Engine = &m_engine;

    m_engine.InitializeShaderSet(&m_shaderSet);
    m_shaders.Initialize(shaderContext);

    ysInputLayout *saqInputLayout = m_engine.GetSaqInputLayout();
    ysShader *saqVertex = m_engine.GetSaqVertexShader();
    ysShader *saqPixel = m_engine.GetSaqPixelShader();
    ysShaderProgram *saqProgram;
    device->CreateShaderProgram(&saqProgram);
    device->AttachShader(saqProgram, saqVertex);
    device->AttachShader(saqProgram, saqPixel);
    device->LinkProgram(saqProgram);

    m_uiStageFlags = m_shaders.GetUiStageFlags();

    dbasic::ShaderStage *newStage;
    m_shaderSet.NewStage("Game Final", &newStage);
    newStage->AddInput(m_intermediateRenderTarget, 0);
    newStage->AddInput(m_guiRenderTarget, 1);
    newStage->SetInputLayout(saqInputLayout);
    newStage->SetRenderTarget(m_engine.GetScreenRenderTarget());
    newStage->SetShaderProgram(saqProgram);
    newStage->SetType(dbasic::ShaderStage::Type::PostProcessing);

    m_engine.InitializeConsoleShaders(&m_shaderSet);
    m_engine.SetShaderSet(&m_shaderSet);

    m_shaders.SetFarClip(200.0f);
    m_ui.setWorld(this);
}

void c_adv::World::initialSpawn() {
    m_mainRealm = newRealm<Realm>();
    m_mainRealm->setIndoor(false);

    ysTransform root;
    dbasic::RenderSkeleton *level1 = m_assetManager.BuildRenderSkeleton(
        &root, m_assetManager.GetSceneObject("Level 1", ysObjectData::ObjectType::Instance));
    generateLevel(level1);

    m_focus = m_mainRealm->spawn<Player>();
    m_focus->RigidBody.Transform.SetPosition(m_respawnPosition);
    m_focus->incrementReferenceCount();
}

void c_adv::World::run() {
    initialSpawn();

    while (m_engine.IsOpen()) {
        frameTick();
    }
}

void c_adv::World::frameTick() {
    m_engine.StartFrame();

    process();
    render();

    m_engine.EndFrame();
}

c_adv::AABB c_adv::World::getCameraExtents() const {
    float cameraX, cameraY;
    m_shaders.GetCameraPosition(&cameraX, &cameraY);

    const float altitude = m_shaders.GetCameraAltitude();

    const float fov = m_shaders.GetCameraFov();
    const float aspect = m_shaders.GetCameraAspect();

    const float v = std::tan(fov / 2) * altitude;
    const float h = v * aspect;

    return { ysMath::LoadVector(-h + cameraX, -v + cameraY, 0.0f, 0.0f), ysMath::LoadVector(h + cameraX, v + cameraY) };
}

void c_adv::World::render() {
    float offset_x = 0.0f, offset_y = 0.0f;
    if (m_engine.IsKeyDown(ysKey::Code::Down)) {
        offset_y = -5.0f;
    }
    else if (m_engine.IsKeyDown(ysKey::Code::Up)) {
        offset_y = 5.0f;
    }

    const ysVector focusPosition = m_focus->RigidBody.Transform.GetWorldPosition();
    m_smoothCamera.setTarget(ysMath::Add(focusPosition, ysMath::LoadVector(0.0f, 0.0f, 0.0f)));
    m_smoothTarget.setTarget(ysMath::Add(focusPosition, ysMath::LoadVector(offset_x, offset_y, 0.0f)));

    const ysVector cameraTarget = m_smoothCamera.getPosition();

    m_shaders.SetCameraPosition(ysMath::GetX(cameraTarget), ysMath::GetY(cameraTarget));
    m_shaders.SetCameraAltitude(m_cameraDistance);
    m_shaders.SetCameraTarget(m_smoothTarget.getPosition());
    m_shaders.SetCameraUp(ysMath::Constants::YAxis);

    m_shaders.SetScreenDimensions(m_engine.GetScreenWidth(), m_engine.GetScreenHeight());
    m_shaders.CalculateCamera();

    if (m_engine.IsKeyDown(ysKey::Code::T)) {
        m_shaders.SetShadowDepth(m_shaders.GetShadowDepth() + 1.1f);
    }
    else if (m_engine.IsKeyDown(ysKey::Code::Y)) {
        m_shaders.SetShadowDepth(m_shaders.GetShadowDepth() - 1.1f);
    }

    if (m_engine.ProcessKeyDown(ysKey::Code::R)) {
        m_engine.GetDevice()->ResizeRenderTarget(
            m_intermediateRenderTarget,
            m_engine.GetScreenWidth(),
            m_engine.GetScreenHeight(),
            m_engine.GetScreenWidth(),
            m_engine.GetScreenHeight());

        m_shaders.OnResize(m_engine.GetScreenWidth(), m_engine.GetScreenHeight());
    }

    m_shaders.ResetLights();

    const float clearColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    m_engine.GetDevice()->SetRenderTarget(m_intermediateRenderTarget);
    m_engine.GetDevice()->ClearBuffers(clearColor);

    m_mainRealm->render();

    m_shaders.Update();

    renderUi();
}

void c_adv::World::process() {
    // Limit min framerate to 30 fps
    const float dt = min(1 / 30.0f, getEngine().GetFrameLength());

    m_mainRealm->process(dt);

    m_smoothCamera.update(dt);
    m_smoothTarget.update(dt);

    if (m_engine.IsKeyDown(ysKey::Code::Subtract)) {
        m_cameraDistance += 0.5f;
    }
    else if (m_engine.IsKeyDown(ysKey::Code::Add)) {
        m_cameraDistance -= 0.5f;
        //if (m_cameraDistance < 1.0f) m_cameraDistance = 1.0f;
    }
    else if (m_engine.IsKeyDown(ysKey::Code::Back)) {
        m_cameraDistance = DefaultCameraDistance;
    }

    if (m_focus->isDead()) {
        m_focus->decrementReferenceCount();
        m_focus = m_mainRealm->spawn<Player>();
        m_focus->RigidBody.Transform.SetPosition(m_respawnPosition);
        m_focus->incrementReferenceCount();
    }

    m_ui.process(dt);
}

void c_adv::World::generateLevel(dbasic::RenderSkeleton *hierarchy) {
    // TEMP
    CollectibleItem *item1 = m_mainRealm->spawn<CollectibleItem>();
    item1->RigidBody.Transform.SetPosition(hierarchy->FindNode("L1_Intel_0")->Transform.GetWorldPosition());
    item1->setAsset(m_assetManager.GetModelAsset("Intel"));

    CollectibleItem *item2 = m_mainRealm->spawn<CollectibleItem>();
    item2->RigidBody.Transform.SetPosition(hierarchy->FindNode("L1_Intel_1")->Transform.GetWorldPosition());
    item2->setAsset(m_assetManager.GetModelAsset("Intel"));
    // END TEMP

    std::map<dbasic::RenderNode *, std::vector<dbasic::RenderNode *>> tree;
    for (int i = 0; i < hierarchy->GetNodeCount(); ++i) {
        dbasic::RenderNode *node = hierarchy->GetNode(i);
        if (node->GetParent() == nullptr) {
            if (tree.count(node) == 0) {
                tree[node] = std::vector<dbasic::RenderNode *>();
            }
        }
        else {
            if (tree.count(node->GetParent()) == 0) {
                tree[node->GetParent()] = { node };
            }
            else {
                tree[node->GetParent()].push_back(node);
            }
        }
    }

    dbasic::RenderNode *root = hierarchy->GetRoot();

    std::stack<dbasic::RenderNode *> s;
    s.push(root);

    while (!s.empty()) {
        dbasic::RenderNode *node = s.top(); s.pop();

        dbasic::SceneObjectAsset *sceneAsset = node->GetSceneAsset();
        dbasic::SceneObjectAsset *instance = sceneAsset->GetInstance();

        bool branchTerminate = true;

        if (instance != nullptr) {
            if (strcmp(instance->GetName(), "Ledge") == 0) {
                ysVector position = node->Transform.GetWorldPosition();
                Ledge *newLedge = m_mainRealm->spawn<Ledge>();
                newLedge->RigidBody.Transform.SetPosition(position);
            }
            else if (strcmp(instance->GetName(), "Counter_1") == 0) {
                ysVector position = node->Transform.GetWorldPosition();
                Counter *newCounter = m_mainRealm->spawn<Counter>();
                newCounter->RigidBody.Transform.SetPosition(position);
            }
            else if (strcmp(instance->GetName(), "Toaster") == 0) {
                ysVector position = node->Transform.GetWorldPosition();
                Toaster *newToaster = m_mainRealm->spawn<Toaster>();
                newToaster->RigidBody.Transform.SetPosition(position);
            }
            else if (strcmp(instance->GetName(), "Shelves") == 0) {
                ysVector position = node->Transform.GetWorldPosition();
                Shelves *newShelves = m_mainRealm->spawn<Shelves>();
                newShelves->RigidBody.Transform.SetPosition(position);
            }
            else if (strcmp(instance->GetName(), "Fridge") == 0) {
                ysVector position = node->Transform.GetWorldPosition();
                Fridge *newFridge = m_mainRealm->spawn<Fridge>();
                newFridge->RigidBody.Transform.SetPosition(position);
            }
            else if (strcmp(instance->GetName(), "Stool_1") == 0) {
                ysVector position = node->Transform.GetWorldPosition();
                Stool_1 *newStool = m_mainRealm->spawn<Stool_1>();
                newStool->RigidBody.Transform.SetPosition(position);
            }
            else if (strcmp(instance->GetName(), "Microwave") == 0) {
                ysVector position = node->Transform.GetWorldPosition();
                Microwave *newMicrowave = m_mainRealm->spawn<Microwave>();
                newMicrowave->RigidBody.Transform.SetPosition(position);
            }
            else if (strcmp(instance->GetName(), "Oven") == 0) {
                ysVector position = node->Transform.GetWorldPosition();
                Oven *newOven = m_mainRealm->spawn<Oven>();
                newOven->RigidBody.Transform.SetPosition(position);
            }
            else if (strcmp(instance->GetName(), "SingleShelf") == 0) {
                ysVector position = node->Transform.GetWorldPosition();
                SingleShelf *newOven = m_mainRealm->spawn<SingleShelf>();
                newOven->RigidBody.Transform.SetPosition(position);
            }
            else if (strcmp(instance->GetName(), "Vase") == 0) {
                ysVector position = node->Transform.GetWorldPosition();
                Vase *newVase = m_mainRealm->spawn<Vase>();
                newVase->RigidBody.Transform.SetPosition(position);
            }
            else if (strcmp(instance->GetName(), "Cabinet") == 0) {
                ysVector position = node->Transform.GetWorldPosition();
                Cabinet *newCabinet = m_mainRealm->spawn<Cabinet>();
                newCabinet->RigidBody.Transform.SetPosition(position);
            }
            else if (strcmp(instance->GetName(), "Sink") == 0) {
                ysVector position = node->Transform.GetWorldPosition();
                Sink *newSink = m_mainRealm->spawn<Sink>();
                newSink->RigidBody.Transform.SetPosition(position);
            }
            else if (strcmp(instance->GetName(), "PlayerStart") == 0) {
                m_respawnPosition = node->Transform.GetWorldPosition();
            }
            else if (strcmp(instance->GetName(), "LightSource::Ceiling") == 0) {
                ysVector position = node->Transform.GetWorldPosition();
                CeilingLightSource *newLight = m_mainRealm->spawn<CeilingLightSource>();
                newLight->RigidBody.Transform.SetPosition(position);
            }
            else if (strcmp(instance->GetName(), "LightSource::Window") == 0) {
                ysVector position = node->Transform.GetWorldPosition();
                WindowLightSource *newLight = m_mainRealm->spawn<WindowLightSource>();
                newLight->RigidBody.Transform.SetPosition(position);
            }
            else if (strcmp(instance->GetName(), "StoveHood") == 0) {
                ysVector position = node->Transform.GetWorldPosition();
                StoveHood *stoveHood = m_mainRealm->spawn<StoveHood>();
                stoveHood->RigidBody.Transform.SetPosition(position);
            }
            else if (strcmp(instance->GetName(), "FruitBowl") == 0) {
                ysVector position = node->Transform.GetWorldPosition();
                FruitBowl *fruitBowl = m_mainRealm->spawn<FruitBowl>();
                fruitBowl->RigidBody.Transform.SetPosition(position);
                fruitBowl->setOrientation(node->Transform.GetWorldOrientation());
            }
            else if (strcmp(instance->GetName(), "Fan") == 0) {
                ysVector position = node->Transform.GetWorldPosition();
                Fan *fan = m_mainRealm->spawn<Fan>();
                fan->RigidBody.Transform.SetPosition(position);
            }
            else if (strcmp(instance->GetName(), "Table") == 0) {
                ysVector position = node->Transform.GetWorldPosition();
                Table *table = m_mainRealm->spawn<Table>();
                table->RigidBody.Transform.SetPosition(position);
            }
            else {
                branchTerminate = false;
            }
        }
        else if (node->GetSceneAsset()->GetType() == ysObjectData::ObjectType::Light) {
            LightObject *light = m_mainRealm->spawn<LightObject>();
            light->setAsset(node->GetSceneAsset());
            light->RigidBody.Transform.SetPosition(node->Transform.GetWorldPosition());
            light->RigidBody.Transform.SetOrientation(node->Transform.GetWorldOrientation());
        }
        else if (node->GetSceneAsset()->GetType() == ysObjectData::ObjectType::Geometry) {
            ysVector position = node->Transform.GetWorldPosition();
            ysQuaternion orientation = node->Transform.GetWorldOrientation();

            StaticArt *newStaticArt = m_mainRealm->spawn<StaticArt>();
            newStaticArt->setAsset(node->GetModelAsset());
            newStaticArt->RigidBody.Transform.SetPosition(position);
            newStaticArt->RigidBody.Transform.SetOrientation(orientation);
        }
        else {
            branchTerminate = false;
        }

        if (!branchTerminate) {
            for (dbasic::RenderNode *child : tree[node]) {
                s.push(child);
            }
        }
    }

    /*
    for (int i = 0; i < hierarchy->GetNodeCount(); ++i) {
        dbasic::RenderNode *node = hierarchy->GetNode(i);
        dbasic::SceneObjectAsset *sceneAsset = node->GetSceneAsset();
        dbasic::SceneObjectAsset *instance = sceneAsset->GetInstance();

        if (instance != nullptr) {
            if (strcmp(instance->GetName(), "Ledge") == 0) {
                ysVector position = node->Transform.GetWorldPosition();
                Ledge *newLedge = m_mainRealm->spawn<Ledge>();
                newLedge->RigidBody.Transform.SetPosition(position);
            }
            else if (strcmp(instance->GetName(), "Counter_1") == 0) {
                ysVector position = node->Transform.GetWorldPosition();
                Counter *newCounter = m_mainRealm->spawn<Counter>();
                newCounter->RigidBody.Transform.SetPosition(position);
            }
            else if (strcmp(instance->GetName(), "Toaster") == 0) {
                ysVector position = node->Transform.GetWorldPosition();
                Toaster *newToaster = m_mainRealm->spawn<Toaster>();
                newToaster->RigidBody.Transform.SetPosition(position);
            }
            else if (strcmp(instance->GetName(), "Shelves") == 0) {
                ysVector position = node->Transform.GetWorldPosition();
                Shelves *newShelves = m_mainRealm->spawn<Shelves>();
                newShelves->RigidBody.Transform.SetPosition(position);
            }
            else if (strcmp(instance->GetName(), "Fridge") == 0) {
                ysVector position = node->Transform.GetWorldPosition();
                Fridge *newFridge = m_mainRealm->spawn<Fridge>();
                newFridge->RigidBody.Transform.SetPosition(position);
            }
            else if (strcmp(instance->GetName(), "Stool_1") == 0) {
                ysVector position = node->Transform.GetWorldPosition();
                Stool_1 *newStool = m_mainRealm->spawn<Stool_1>();
                newStool->RigidBody.Transform.SetPosition(position);
            }
            else if (strcmp(instance->GetName(), "Microwave") == 0) {
                ysVector position = node->Transform.GetWorldPosition();
                Microwave *newMicrowave = m_mainRealm->spawn<Microwave>();
                newMicrowave->RigidBody.Transform.SetPosition(position);
            }
            else if (strcmp(instance->GetName(), "Oven") == 0) {
                ysVector position = node->Transform.GetWorldPosition();
                Oven *newOven = m_mainRealm->spawn<Oven>();
                newOven->RigidBody.Transform.SetPosition(position);
            }
            else if (strcmp(instance->GetName(), "SingleShelf") == 0) {
                ysVector position = node->Transform.GetWorldPosition();
                SingleShelf *newOven = m_mainRealm->spawn<SingleShelf>();
                newOven->RigidBody.Transform.SetPosition(position);
            }
            else if (strcmp(instance->GetName(), "Vase") == 0) {
                ysVector position = node->Transform.GetWorldPosition();
                Vase *newVase = m_mainRealm->spawn<Vase>();
                newVase->RigidBody.Transform.SetPosition(position);
            }
            else if (strcmp(instance->GetName(), "Cabinet") == 0) {
                ysVector position = node->Transform.GetWorldPosition();
                Cabinet *newCabinet = m_mainRealm->spawn<Cabinet>();
                newCabinet->RigidBody.Transform.SetPosition(position);
            }
            else if (strcmp(instance->GetName(), "Sink") == 0) {
                ysVector position = node->Transform.GetWorldPosition();
                Sink *newSink = m_mainRealm->spawn<Sink>();
                newSink->RigidBody.Transform.SetPosition(position);
            }
            else if (strcmp(instance->GetName(), "PlayerStart") == 0) {
                m_respawnPosition = node->Transform.GetWorldPosition();
            }
            else if (strcmp(instance->GetName(), "LightSource::Ceiling") == 0) {
                ysVector position = node->Transform.GetWorldPosition();
                CeilingLightSource *newLight = m_mainRealm->spawn<CeilingLightSource>();
                newLight->RigidBody.Transform.SetPosition(position);
            }
            else if (strcmp(instance->GetName(), "LightSource::Window") == 0) {
                ysVector position = node->Transform.GetWorldPosition();
                WindowLightSource *newLight = m_mainRealm->spawn<WindowLightSource>();
                newLight->RigidBody.Transform.SetPosition(position);
            }
            else if (strcmp(instance->GetName(), "StoveHood") == 0) {
                ysVector position = node->Transform.GetWorldPosition();
                StoveHood *stoveHood = m_mainRealm->spawn<StoveHood>();
                stoveHood->RigidBody.Transform.SetPosition(position);
            }
            else if (strcmp(instance->GetName(), "FruitBowl") == 0) {
                ysVector position = node->Transform.GetWorldPosition();
                FruitBowl *fruitBowl = m_mainRealm->spawn<FruitBowl>();
                fruitBowl->RigidBody.Transform.SetPosition(position);
            }
            else if (strcmp(instance->GetName(), "Fan") == 0) {
                ysVector position = node->Transform.GetWorldPosition();
                Fan *fan = m_mainRealm->spawn<Fan>();
                fan->RigidBody.Transform.SetPosition(position);
            }
            else if (strcmp(instance->GetName(), "Table") == 0) {
                ysVector position = node->Transform.GetWorldPosition();
                Table *table = m_mainRealm->spawn<Table>();
                table->RigidBody.Transform.SetPosition(position);
            }
        }
        else if (node->GetSceneAsset()->GetType() == ysObjectData::ObjectType::Light) {
            LightObject *light = m_mainRealm->spawn<LightObject>();
            light->setAsset(node->GetSceneAsset());
            light->RigidBody.Transform.SetPosition(node->Transform.GetWorldPosition());
            light->RigidBody.Transform.SetOrientation(node->Transform.GetWorldOrientation());
        }
        else if (node->GetSceneAsset()->GetType() == ysObjectData::ObjectType::Geometry) {
            if (node->GetModelAsset() == nullptr) continue;
            if (node->GetParent() == nullptr) continue;
            if (node->GetParent()->GetParent() != hierarchy->GetNode("Level_1")) continue;

            ysVector position = node->Transform.GetWorldPosition();
            ysQuaternion orientation = node->Transform.GetWorldOrientation();

            StaticArt *newStaticArt = m_mainRealm->spawn<StaticArt>();
            newStaticArt->setAsset(node->GetModelAsset());
            newStaticArt->RigidBody.Transform.SetPosition(position);
            newStaticArt->RigidBody.Transform.SetOrientation(orientation);
        }
    }*/
}

void c_adv::World::renderUi() {
    m_shaders.uiShaderScreenVariables().Projection =
        ysMath::OrthographicProjection(m_engine.GetScreenWidth(), m_engine.GetScreenHeight(), 0.0f, 1.0f);
    m_shaders.uiShaderScreenVariables().CameraView = ysMath::LoadIdentity();
    m_shaders.uiShaderScreenVariables().FogNear = 100.0f;
    m_shaders.uiShaderScreenVariables().FogFar = 101.0f;

    m_ui.render();
}

void c_adv::World::updateRealms() {
    for (Realm *realm : m_realms) {
        realm->updateRealms();
    }
}
