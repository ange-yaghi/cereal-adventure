#ifndef CEREAL_ADVENTURE_UI_H
#define CEREAL_ADVENTURE_UI_H

#include "cooldown_timer.h"
#include "wrapping_timer.h"

namespace c_adv {

    class World;

    class Ui {
    public:
        Ui();
        ~Ui();

        void setWorld(World *world) { m_world = world; }
        World *getWorld() const { return m_world; }

        void initialize();
        void render();
        void process(float dt);

        void triggerDamage(float amount);

        void setPlayerHealth(float playerHealth) { m_playerHealth = playerHealth; }

    protected:
        CooldownTimer m_damageTimer;
        WrappingTimer m_healthHeartbeat;
        float m_playerHealth;

        World *m_world;
    };

} /* namespace c_adv */

#endif /* CEREAL_ADVENTURE_UI_H */
