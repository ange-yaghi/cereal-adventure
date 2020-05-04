#ifndef CEREAL_ADVENTURE_COOLDOWN_TIMER_H
#define CEREAL_ADVENTURE_COOLDOWN_TIMER_H

namespace c_adv {

    class CooldownTimer {
    public:
        CooldownTimer();
        ~CooldownTimer();

        void trigger();
        bool ready() const { return !m_triggered && !m_locked; }
        void update(float dt);

        void setCooldownPeriod(float t) { m_cooldownPeriod = t; }
        float getCooldownPeriod() const { return m_cooldownPeriod; }

        void lock() { m_locked = true; }
        void unlock() { m_locked = false; }

    protected:
        float m_timer;
        float m_cooldownPeriod;

        bool m_triggered;
        bool m_locked;
    };

} /* namespace c_adv */

#endif /* CEREAL_ADVENTURE_COOLDOWN_TIMER_H */
