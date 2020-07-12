#ifndef CEREAL_ADVENTURE_PROJECTILE_DAMAGE_COMPONENT_H
#define CEREAL_ADVENTURE_PROJECTILE_DAMAGE_COMPONENT_H

#include "delta.h"

namespace c_adv {

    class Player;
    class GameObject;

    class ProjectileDamageComponent {
        friend Player;

    public:
        ProjectileDamageComponent();
        ~ProjectileDamageComponent();

        void initialize(Player *player);
        void process(float dt);

    protected:
        Player *m_player;
    };

} /* namespace c_adv */

#endif /* CEREAL_ADVENTURE_PROJECTILE_DAMAGE_COMPONENT_H */
