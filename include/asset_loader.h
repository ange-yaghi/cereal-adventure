#ifndef CEREAL_ADVENTURE_ASSET_LOADER_H
#define CEREAL_ADVENTURE_ASSET_LOADER_H

#include "delta.h"

namespace c_adv {

    class AssetLoader {
    public:
        AssetLoader();
        ~AssetLoader();

        static void createAllMaterials(dbasic::AssetManager *am);
        static void loadAllTextures(const dbasic::Path &assetPath, dbasic::AssetManager *am);
        static void loadAllAssets(const dbasic::Path &assetPath, dbasic::AssetManager *am);
        static void loadAllAudioAssets(const dbasic::Path &assetPath, dbasic::AssetManager *am);

    protected:
        static std::string getPath(const char *path, const dbasic::Path &assetPath);
    };

} /* namespace c_adv */

#endif /* CEREAL_ADVENTURE_ASSET_LOADER_H */
