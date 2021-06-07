#ifndef CEREAL_ADVENTURE_TEST_OBSTACLE_H
#define CEREAL_ADVENTURE_TEST_OBSTACLE_H

#include "game_object.h"

namespace c_adv {

    class Ledge;

    class TestObstacle : public GameObject {
    public:
        TestObstacle();
        ~TestObstacle();

        virtual void initialize();
        virtual void destroy();

        virtual void render();

    protected:
        Ledge *m_ledge0, *m_ledge1, *m_ledge2, *m_ledge3;

        // Assets ----
    public:
        void getAssets(dbasic::AssetManager *am);

    protected:
        static dbasic::ModelAsset *m_obstacleMesh;
    };

} /* namespace c_adv */

#endif /* CEREAL_ADVENTURE_TEST_OBSTACLE_H */
