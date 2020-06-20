#include "../include/asset_loader.h"

#include "../include/player.h"
#include "../include/test_obstacle.h"
#include "../include/ledge.h"
#include "../include/counter.h"
#include "../include/toaster.h"
#include "../include/shelves.h"
#include "../include/milk_carton.h"
#include "../include/fridge.h"
#include "../include/stool_1.h"
#include "../include/microwave.h"
#include "../include/oven.h"
#include "../include/single_shelf.h"

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
    playerMaterial->SetDiffuseColor(ysColor::srgbiToLinear(0x2c, 0x3e, 0x50));
}

void c_adv::AssetLoader::loadAllTextures(const dbasic::Path &assetPath, dbasic::AssetManager *am) {
    /* Load all textures here */
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
