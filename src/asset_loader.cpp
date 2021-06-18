#include "../include/asset_loader.h"

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
    skyboxMaterial->SetDiffuseColor(ysMath::LoadScalar(2.0f));
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
    fridgeMaterial->SetIncidentSpecular(0.0f);

    dbasic::Material *floorMaterial = am->NewMaterial();
    floorMaterial->SetName("Floor");
    floorMaterial->SetDiffuseColor(White);
    floorMaterial->SetDiffuseMap(am->GetTexture("Floor_Diffuse")->GetTexture());
    floorMaterial->SetAoMap(am->GetTexture("Floor_AO")->GetTexture());

    dbasic::Material *carpetMaterial = am->NewMaterial();
    carpetMaterial->SetName("Carpet");
    carpetMaterial->SetDiffuseColor(ysColor::srgbiToLinear(0x25, 0x25, 0x26));
    carpetMaterial->SetIncidentSpecular(0.0f);
    carpetMaterial->SetSpecularMix(0.2f);

    dbasic::Material *toastMaterial = am->NewMaterial();
    toastMaterial->SetName("Toast");
    toastMaterial->SetDiffuseColor(White);
    toastMaterial->SetDiffuseMap(am->GetTexture("Toast_Diffuse")->GetTexture());
    toastMaterial->SetSpecularMix(0.1f);

    dbasic::Material *intelMaterial = am->NewMaterial();
    intelMaterial->SetName("Intel");
    intelMaterial->SetDiffuseColor(White);
    intelMaterial->SetDiffuseMap(am->GetTexture("Intel_Diffuse")->GetTexture());

    dbasic::Material *shelfMaterial = am->NewMaterial();
    shelfMaterial->SetName("Shelf");
    shelfMaterial->SetDiffuseColor(White);
    shelfMaterial->SetDiffuseMap(am->GetTexture("Shelf_Diffuse")->GetTexture());
    shelfMaterial->SetSpecularMix(0.5f);

    dbasic::Material *wallMaterial = am->NewMaterial();
    wallMaterial->SetName("Level1Wall");
    wallMaterial->SetDiffuseColor(ysColor::srgbiToLinear(0x3A, 0x3B, 0x3C));
    wallMaterial->SetIncidentSpecular(0.5f);
    wallMaterial->SetSpecularMix(0.25f);

    dbasic::Material *bigPlant0Material = am->NewMaterial();
    bigPlant0Material->SetName("BigPlant0");
    bigPlant0Material->SetDiffuseColor(White);
    bigPlant0Material->SetDiffuseMap(am->GetTexture("BigPlant0_Diffuse")->GetTexture());
    bigPlant0Material->SetSpecularMix(0.5f);

    dbasic::Material *books0Material = am->NewMaterial();
    books0Material->SetName("Books0");
    books0Material->SetDiffuseColor(White);
    books0Material->SetDiffuseMap(am->GetTexture("Books0_Diffuse")->GetTexture());

    dbasic::Material *box0Material = am->NewMaterial();
    box0Material->SetName("Box0");
    box0Material->SetDiffuseColor(White);
    box0Material->SetDiffuseMap(am->GetTexture("Box0_Diffuse")->GetTexture());

    dbasic::Material *box1Material = am->NewMaterial();
    box1Material->SetName("Box1");
    box1Material->SetDiffuseColor(White);
    box1Material->SetDiffuseMap(am->GetTexture("Box1_Diffuse")->GetTexture());

    dbasic::Material *almondBagMaterial = am->NewMaterial();
    almondBagMaterial->SetName("AlmondBag");
    almondBagMaterial->SetDiffuseColor(White);
    almondBagMaterial->SetDiffuseMap(am->GetTexture("AlmondBag_Diffuse")->GetTexture());

    dbasic::Material *tunaCanMaterial = am->NewMaterial();
    tunaCanMaterial->SetName("TunaCan");
    tunaCanMaterial->SetDiffuseColor(White);
    tunaCanMaterial->SetDiffuseMap(am->GetTexture("TunaCan_Diffuse")->GetTexture());

    dbasic::Material *tallCan0Material = am->NewMaterial();
    tallCan0Material->SetName("TallCan0");
    tallCan0Material->SetDiffuseColor(White);
    tallCan0Material->SetDiffuseMap(am->GetTexture("TallCan0_Diffuse")->GetTexture());

    dbasic::Material *largeCan0Material = am->NewMaterial();
    largeCan0Material->SetName("LargeCan0");
    largeCan0Material->SetDiffuseColor(White);
    largeCan0Material->SetDiffuseMap(am->GetTexture("LargeCan0_Diffuse")->GetTexture());

    dbasic::Material *wineBottle0Material = am->NewMaterial();
    wineBottle0Material->SetName("WineBottle0");
    wineBottle0Material->SetDiffuseColor(White);
    wineBottle0Material->SetDiffuseMap(am->GetTexture("WineBottle0_Diffuse")->GetTexture());

    dbasic::Material *largePot0Material = am->NewMaterial();
    largePot0Material->SetName("LargePot0");
    largePot0Material->SetDiffuseColor(White);
    largePot0Material->SetDiffuseMap(am->GetTexture("LargePot0_Diffuse")->GetTexture());

    dbasic::Material *teapotMaterial = am->NewMaterial();
    teapotMaterial->SetName("Teapot");
    teapotMaterial->SetDiffuseColor(White);
    teapotMaterial->SetDiffuseMap(am->GetTexture("Teapot_Diffuse")->GetTexture());

    dbasic::Material *clockMaterial = am->NewMaterial();
    clockMaterial->SetName("Clock");
    clockMaterial->SetDiffuseColor(White);
    clockMaterial->SetDiffuseMap(am->GetTexture("Clock_Diffuse")->GetTexture());

    dbasic::Material *vaseMaterial = am->NewMaterial();
    vaseMaterial->SetName("Vase");
    vaseMaterial->SetDiffuseColor(White);
    vaseMaterial->SetDiffuseMap(am->GetTexture("Vase_Diffuse")->GetTexture());

    dbasic::Material *lightFixture0Material = am->NewMaterial();
    lightFixture0Material->SetName("LightFixture0");
    lightFixture0Material->SetDiffuseColor(White);
    lightFixture0Material->SetDiffuseMap(am->GetTexture("LightFixture0_Diffuse")->GetTexture());

    dbasic::Material *painting0Material = am->NewMaterial();
    painting0Material->SetName("Painting0");
    painting0Material->SetDiffuseColor(White);
    painting0Material->SetDiffuseMap(am->GetTexture("Painting0_Diffuse")->GetTexture());

    dbasic::Material *painting1Material = am->NewMaterial();
    painting1Material->SetName("Painting1");
    painting1Material->SetDiffuseColor(White);
    painting1Material->SetDiffuseMap(am->GetTexture("Painting1_Diffuse")->GetTexture());

    dbasic::Material *curtainsMaterial = am->NewMaterial();
    curtainsMaterial->SetName("Curtains");
    curtainsMaterial->SetDiffuseColor(White);
    curtainsMaterial->SetDiffuseMap(am->GetTexture("Curtains_Diffuse")->GetTexture());

    dbasic::Material *neon0 = am->NewMaterial();
    neon0->SetName("Neon0");
    neon0->SetDiffuseColor(ysMath::Mul(ysColor::srgbiToLinear(0xE7D575), ysMath::LoadScalar(2.5f)));
    neon0->SetLit(false);

    dbasic::Material *neon1 = am->NewMaterial();
    neon1->SetName("Neon1");
    neon1->SetDiffuseColor(ysMath::Mul(ysColor::srgbiToLinear(0x93278F), ysMath::LoadScalar(2.5f)));
    neon1->SetLit(false);
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

    am->LoadTexture(getPath("textures/Intel_Diffuse.png", assetPath).c_str(), "Intel_Diffuse");

    am->LoadTexture(getPath("textures/Shelf_256.png", assetPath).c_str(), "Shelf_Diffuse");

    am->LoadTexture(getPath("textures/SkyBox_Diffuse.png", assetPath).c_str(), "SkyBox_Diffuse");
    am->LoadTexture(getPath("post-processing/health_overlay_v2.png", assetPath).c_str(), "Health_Overlay");
    am->LoadTexture(getPath("post-processing/damage_overlay.png", assetPath).c_str(), "Damage_Overlay");

    am->LoadTexture(getPath("textures/AlmondBag.png", assetPath).c_str(), "AlmondBag_Diffuse");
    am->LoadTexture(getPath("textures/BigPlant0_Diffuse.png", assetPath).c_str(), "BigPlant0_Diffuse");
    am->LoadTexture(getPath("textures/Books0_Diffuse.png", assetPath).c_str(), "Books0_Diffuse");
    am->LoadTexture(getPath("textures/Box0_Diffuse.png", assetPath).c_str(), "Box0_Diffuse");
    am->LoadTexture(getPath("textures/Box1_Diffuse.png", assetPath).c_str(), "Box1_Diffuse");
    am->LoadTexture(getPath("textures/Clock.png", assetPath).c_str(), "Clock_Diffuse");
    am->LoadTexture(getPath("textures/Curtains_Diffuse.png", assetPath).c_str(), "Curtains_Diffuse");
    am->LoadTexture(getPath("textures/LargeCan0_Diffuse.png", assetPath).c_str(), "LargeCan0_Diffuse");
    am->LoadTexture(getPath("textures/LargePot0_Diffuse.png", assetPath).c_str(), "LargePot0_Diffuse");
    am->LoadTexture(getPath("textures/LightFixture0.png", assetPath).c_str(), "LightFixture0_Diffuse");
    am->LoadTexture(getPath("textures/Painting0.png", assetPath).c_str(), "Painting0_Diffuse");
    am->LoadTexture(getPath("textures/Painting1_Diffuse.png", assetPath).c_str(), "Painting1_Diffuse");
    am->LoadTexture(getPath("textures/Shelf_256.png", assetPath).c_str(), "Shelf_256");
    am->LoadTexture(getPath("textures/TallCan0_Diffuse.png", assetPath).c_str(), "TallCan0_Diffuse");
    am->LoadTexture(getPath("textures/Teapot_Diffuse.png", assetPath).c_str(), "Teapot_Diffuse");
    am->LoadTexture(getPath("textures/TunaCan_Diffuse.001.png", assetPath).c_str(), "TunaCan_Diffuse");
    am->LoadTexture(getPath("textures/Vase_Diffuse.png", assetPath).c_str(), "Vase_Diffuse");
    am->LoadTexture(getPath("textures/WineBottle0_Diffuse.png", assetPath).c_str(), "WineBottle0_Diffuse");
}

void c_adv::AssetLoader::loadAllAssets(const dbasic::Path &assetPath, dbasic::AssetManager *am) {
    loadAllTextures(assetPath, am);
    loadAllAudioAssets(assetPath, am);
    createAllMaterials(am);

    /* Load all model and animation assets here */
    am->CompileInterchangeFile(getPath("cereal-box/cereal_box_light_test", assetPath).c_str(), 1.0f, true);
    am->LoadSceneFile(getPath("cereal-box/cereal_box_light_test", assetPath).c_str());

    am->LoadAnimationFile(getPath("cereal-box/cereal_box_new_run.dimo", assetPath).c_str());

    am->ResolveNodeHierarchy();
}

void c_adv::AssetLoader::loadAllAudioAssets(const dbasic::Path &assetPath, dbasic::AssetManager *am) {
    am->LoadAudioFile(getPath("audio/cereal_box_Footstep_01.wav", assetPath).c_str(), "CerealBox::Footstep01");
    am->LoadAudioFile(getPath("audio/cereal_box_Footstep_02.wav", assetPath).c_str(), "CerealBox::Footstep02");
    am->LoadAudioFile(getPath("audio/cereal_box_Footstep_03.wav", assetPath).c_str(), "CerealBox::Footstep03");
    am->LoadAudioFile(getPath("audio/cereal_box_Footstep_04.wav", assetPath).c_str(), "CerealBox::Footstep04");

    am->LoadAudioFile(getPath("audio/cereal_box_damage_impact.wav", assetPath).c_str(), "CerealBox::DamageImpact");

    am->LoadAudioFile(getPath("audio/cereal_box_JumpVocal_01.wav", assetPath).c_str(), "CerealBox::JumpVocal01");
    am->LoadAudioFile(getPath("audio/cereal_box_JumpVocal_02.wav", assetPath).c_str(), "CerealBox::JumpVocal02");

    am->LoadAudioFile(getPath("audio/cereal_box_Shake_01.wav", assetPath).c_str(), "CerealBox::Shake01");
    am->LoadAudioFile(getPath("audio/cereal_box_Shake_02.wav", assetPath).c_str(), "CerealBox::Shake02");
    am->LoadAudioFile(getPath("audio/cereal_box_Shake_03.wav", assetPath).c_str(), "CerealBox::Shake03");

    am->LoadAudioFile(getPath("audio/cereal_box_Damage_01.wav", assetPath).c_str(), "CerealBox::Damage01");
    am->LoadAudioFile(getPath("audio/cereal_box_Damage_02.wav", assetPath).c_str(), "CerealBox::Damage02");

    am->LoadAudioFile(getPath("audio/collection/collection_mysterious.wav", assetPath).c_str(), "Collection::Mysterious");
    am->LoadAudioFile(getPath("audio/toaster/toaster_launch.wav", assetPath).c_str(), "Toaster::Launch");

}

std::string c_adv::AssetLoader::getPath(const char *path, const dbasic::Path &assetPath) {
    return assetPath.Append(dbasic::Path(path)).ToString();
}
