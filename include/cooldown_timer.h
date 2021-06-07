#ifndef CEREAL_ADVENTURE_COOLDOWN_TIMER_H
#define CEREAL_ADVENTURE_COOLDOWN_TIMER_H

namespace c_adv {

    class CooldownTimer {
    public:
        CooldownTimer();
        ~CooldownTimer();

        void trigger();
        bool ready() const { return !m_triggered && m_enabled; }
        bool active() const { return m_triggered && m_enabled; }
        void update(float dt);

        void setCooldownPeriod(float t) { m_cooldownPeriod = t; }
        float getCooldownPeriod() const { return m_cooldownPeriod; }

        float get() const { return m_timer / m_cooldownPeriod; }

        void enable() { m_enabled = true; }
        void disable() { m_enabled = false; }

    protected:
        float m_timer;
        float m_cooldownPeriod;

        bool m_triggered;
        bool m_enabled;
    };

} /* namespace c_adv */

#endif /* CEREAL_ADVENTURE_COOLDOWN_TIMER_H */
