#ifndef CEREAL_ADVENTURE_DEATH_COMPONENT_H
#define CEREAL_ADVENTURE_DEATH_COMPONENT_H

#include "cooldown_timer.h"

#include "delta.h"

namespace c_adv {

    class Player;
    class GameObject;

    class DeathComponent {
        friend Player;

    public:
        static const float AfterDeathSequenceLength;

    public:
        DeathComponent();
        ~DeathComponent();

        void initialize(Player *player);
        void process(float dt);

    protected:
        CooldownTimer m_afterDeathCooldown;
        Player *m_player;
    };

} /* namespace c_adv */

#endif /* CEREAL_ADVENTURE_DEATH_COMPONENT_H */
