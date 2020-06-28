#ifndef CEREAL_ADVENTURE_CEREAL_ADVENTURE_APP_H
#define CEREAL_ADVENTURE_CEREAL_ADVENTURE_APP_H

#include "delta.h"

#include "world.h"

namespace c_adv {

    class CerealAdventureApp {
    public:
        CerealAdventureApp();
        ~CerealAdventureApp();

        void Initialize(void *instance, ysContextObject::DeviceAPI api);
        void Run();

        World m_world;
    };

} /* namespace c_adv */ 

#endif /* CEREAL_ADVENTURE_CEREAL_ADVENTURE_APP_H */
