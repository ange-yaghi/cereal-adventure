#ifndef CEREAL_ADVENTURE_TEST_OBSTACLE_H
#define CEREAL_ADVENTURE_TEST_OBSTACLE_H

#include "game_object.h"

namespace c_adv {

    class TestObstacle : public GameObject {
    public:
        TestObstacle();
        ~TestObstacle();

        virtual void initialize();

        virtual void render();
        virtual void process();

        // Assets ----
    public:
        static void configureAssets(dbasic::AssetManager *am);
    };

} /* namespace c_adv */

#endif /* CEREAL_ADVENTURE_TEST_OBSTACLE_H */
