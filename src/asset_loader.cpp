#include "../include/asset_loader.h"

#include "../include/game_objects.h"
#include "../include/colors.h"

c_adv::AssetLoader::AssetLoader() {
    /* void */
}

c_adv::AssetLoader::~AssetLoader() {
    /* void */
}

void c_adv::AssetLoader::createAllMaterials(dbasic::AssetManager *am) {
    dbasic::Material *lightFill = am->NewMaterial();
    lightFill->SetName("LightFill");
    lightFill->SetDiffuseColor(ysColor::srgbiToLinear(0xEF, 0x38, 0x37));

    dbasic::Material *lineColor = am->NewMaterial();
    lineColor->SetName("LineColor");
    lineColor->SetDiffuseColor(ysColor::srgbiToLinear(0x91, 0x1A, 0x1D));

    dbasic::Material *darkFill = am->NewMaterial();
    darkFill->SetName("DarkFill");
    darkFill->SetDiffuseColor(ysColor::srgbiToLinear(0xC4, 0x21, 0x26));

    dbasic::Material *highlight = am->NewMaterial();
    highlight->SetName("Highlight");
    highlight->SetDiffuseColor(ysMath::LoadVector(1.0f, 1.0f, 1.0f, 1.0f - 0.941667f));

    dbasic::Material *playerMaterial = am->NewMaterial();
    playerMaterial->SetName("PlayerMaterial");
    playerMaterial->SetDiffuseColor(White);
    playerMaterial->SetDiffuseMap(am->GetTexture("PlayerDiffuse")->GetTexture());

    dbasic::Material *counterMaterial = am->NewMaterial();
    counterMaterial->SetName("CounterMaterial");
    counterMaterial->SetDiffuseColor(White);
    counterMaterial->SetDiffuseMap(am->GetTexture("CounterDiffuse")->GetTexture());
    counterMaterial->SetAoMap(am->GetTexture("CounterAO")->GetTexture());

    dbasic::Material *toasterMaterial = am->NewMaterial();
    toasterMaterial->SetName("ToasterMaterial");
    toasterMaterial->SetDiffuseColor(White);
    toasterMaterial->SetDiffuseMap(am->GetTexture("Toaster_Diffuse")->GetTexture());
    toasterMaterial->SetAoMap(am->GetTexture("Toaster_AO")->GetTexture());

    dbasic::Material *cabinetMaterial = am->NewMaterial();
    cabinetMaterial->SetName("CabinetMaterial");
    cabinetMaterial->SetDiffuseColor(White);
    cabinetMaterial->SetDiffuseMap(am->GetTexture("Cabinet_Diffuse")->GetTexture());
    cabinetMaterial->SetAoMap(am->GetTexture("Cabinet_AO")->GetTexture());

    dbasic::Material *skyboxMaterial = am->NewMaterial();
    skyboxMaterial->SetName("SkyBox");
    skyboxMaterial->SetDiffuseColor(White);
    skyboxMaterial->SetDiffuseMap(am->GetTexture("SkyBox_Diffuse")->GetTexture());
    skyboxMaterial->SetLit(false);

    dbasic::Material *distantOfficeBuildingsMaterial = am->NewMaterial();
    distantOfficeBuildingsMaterial->SetName("DistantOfficeBuildings");
    distantOfficeBuildingsMaterial->SetDiffuseColor(ysColor::srgbiToLinear(0xD5C4C8));
    distantOfficeBuildingsMaterial->SetLit(false);

    dbasic::Material *distantHousesMaterial = am->NewMaterial();
    distantHousesMaterial->SetName("DistantHouses");
    distantHousesMaterial->SetDiffuseColor(ysColor::srgbiToLinear(0x2A1F50));
    distantHousesMaterial->SetLit(false);

    dbasic::Material *distantHousesMiddleMaterial = am->NewMaterial();
    distantHousesMiddleMaterial->SetName("DistantHousesMiddle");
    distantHousesMiddleMaterial->SetDiffuseColor(ysColor::srgbiToLinear(0x998FDD));
    distantHousesMiddleMaterial->SetLit(false);

    dbasic::Material *windowMaterial = am->NewMaterial();
    windowMaterial->SetName("WindowFrame");
    windowMaterial->SetDiffuseColor(White);
    windowMaterial->SetDiffuseMap(am->GetTexture("WindowFrame_Diffuse")->GetTexture());
    windowMaterial->SetAoMap(am->GetTexture("WindowFrame_AO")->GetTexture());

    dbasic::Material *fridgeMaterial = am->NewMaterial();
    fridgeMaterial->SetName("Fridge");
    fridgeMaterial->SetDiffuseColor(White);
    fridgeMaterial->SetDiffuseMap(am->GetTexture("Fridge_Diffuse")->GetTexture());
    fridgeMaterial->SetAoMap(am->GetTexture("Fridge_AO")->GetTexture());

    dbasic::Material *floorMaterial = am->NewMaterial();
    floorMaterial->SetName("Floor");
    floorMaterial->SetDiffuseColor(White);
    floorMaterial->SetDiffuseMap(am->GetTexture("Floor_Diffuse")->GetTexture());
    floorMaterial->SetAoMap(am->GetTexture("Floor_AO")->GetTexture());

    dbasic::Material *toastMaterial = am->NewMaterial();
    toastMaterial->SetName("Toast");
    toastMaterial->SetDiffuseColor(White);
    toastMaterial->SetDiffuseMap(am->GetTexture("Toast_Diffuse")->GetTexture());
}

void c_adv::AssetLoader::loadAllTextures(const dbasic::Path &assetPath, dbasic::AssetManager *am) {
    am->LoadTexture(getPath("textures/Counter_128_v2.png", assetPath).c_str(), "CounterDiffuse");
    am->LoadTexture(getPath("textures/Counter_AO_128.png", assetPath).c_str(), "CounterAO");

    am->LoadTexture(getPath("textures/Toaster_Diffuse_64.png", assetPath).c_str(), "Toaster_Diffuse");
    am->LoadTexture(getPath("textures/Toaster_AO_64.png", assetPath).c_str(), "Toaster_AO");

    am->LoadTexture(getPath("textures/Cabinet_Diffuse_128.png", assetPath).c_str(), "Cabinet_Diffuse");
    am->LoadTexture(getPath("textures/Cabinet_AO_128.png", assetPath).c_str(), "Cabinet_AO");

    am->LoadTexture(getPath("textures/WindowFrame_Diffuse_128.png", assetPath).c_str(), "WindowFrame_Diffuse");
    am->LoadTexture(getPath("textures/WindowFrame_AO_128.png", assetPath).c_str(), "WindowFrame_AO");

    am->LoadTexture(getPath("textures/Fridge_Diffuse_128.png", assetPath).c_str(), "Fridge_Diffuse");
    am->LoadTexture(getPath("textures/Fridge_AO_128.png", assetPath).c_str(), "Fridge_AO");

    am->LoadTexture(getPath("textures/Floor_Diffuse_2048.png", assetPath).c_str(), "Floor_Diffuse");
    am->LoadTexture(getPath("textures/Floor_AO_2048.png", assetPath).c_str(), "Floor_AO");

    am->LoadTexture(getPath("textures/Toast_Diffuse_64.png", assetPath).c_str(), "Toast_Diffuse");

    am->LoadTexture(getPath("cereal-box/PlayerTexture_128x128_v2.png", assetPath).c_str(), "PlayerDiffuse");

    am->LoadTexture(getPath("textures/SkyBox_Diffuse.png", assetPath).c_str(), "SkyBox_Diffuse");
}

void c_adv::AssetLoader::loadAllAssets(const dbasic::Path &assetPath, dbasic::AssetManager *am) {
    loadAllTextures(assetPath, am);
    loadAllAudioAssets(assetPath, am);
    createAllMaterials(am);

    /* Load all model and animation assets here */
    am->CompileInterchangeFile(getPath("cereal-box/cereal_box", assetPath).c_str(), 1.0f, true);
    am->LoadSceneFile(getPath("cereal-box/cereal_box", assetPath).c_str());

    am->LoadAnimationFile(getPath("cereal-box/cereal_box_new_run.dimo", assetPath).c_str());

    am->ResolveNodeHierarchy();

    /* Connect all assets to classes */
    Player::configureAssets(am);
    TestObstacle::configureAssets(am);
    Ledge::configureAssets(am);
    Counter::configureAssets(am);
    Toaster::configureAssets(am);
    Shelves::configureAssets(am);
    MilkCarton::configureAssets(am);
    Fridge::configureAssets(am);
    Stool_1::configureAssets(am);
    Microwave::configureAssets(am);
    Oven::configureAssets(am);
    SingleShelf::configureAssets(am);
    Vase::configureAssets(am);
    Cabinet::configureAssets(am);
    Sink::configureAssets(am);
    StoveHood::configureAssets(am);
    FruitBowl::configureAssets(am);
    FruitProjectile::configureAssets(am);
    ToastProjectile::configureAssets(am);
    Fan::configureAssets(am);
    Table::configureAssets(am);
}

void c_adv::AssetLoader::loadAllAudioAssets(const dbasic::Path &assetPath, dbasic::AssetManager *am) {
    am->LoadAudioFile(getPath("audio/cereal_box_Footstep_01.wav", assetPath).c_str(), "CerealBox::Footstep01");
    am->LoadAudioFile(getPath("audio/cereal_box_Footstep_02.wav", assetPath).c_str(), "CerealBox::Footstep02");
    am->LoadAudioFile(getPath("audio/cereal_box_Footstep_03.wav", assetPath).c_str(), "CerealBox::Footstep03");
    am->LoadAudioFile(getPath("audio/cereal_box_Footstep_04.wav", assetPath).c_str(), "CerealBox::Footstep04");
    am->LoadAudioFile(getPath("audio/cereal_box_damage_impact.wav", assetPath).c_str(), "CerealBox::DamageImpact");
}

std::string c_adv::AssetLoader::getPath(const char *path, const dbasic::Path &assetPath) {
    return assetPath.Append(dbasic::Path(path)).ToString();
}
