#ifndef CEREAL_ADVENTURE_FIRE_DAMAGE_COMPONENT_H
#define CEREAL_ADVENTURE_FIRE_DAMAGE_COMPONENT_H

#include "cooldown_timer.h"

#include "delta.h"

namespace c_adv {

    class Player;
    class GameObject;

    class FireDamageComponent {
        friend Player;

    public:
        FireDamageComponent();
        ~FireDamageComponent();

        void initialize(Player *player);
        void process(float dt);

    protected:
        GameObject *getCollidingOven(ysVector &position);

        CooldownTimer m_fireDamageCooldown;
        Player *m_player;
    };

} /* namespace c_adv */

#endif /* CEREAL_ADVENTURE_FIRE_DAMAGE_COMPONENT_H */
