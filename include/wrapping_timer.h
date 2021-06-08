#ifndef CEREAL_ADVENTURE_WRAPPING_TIMER_H
#define CEREAL_ADVENTURE_WRAPPING_TIMER_H

namespace c_adv {

    class WrappingTimer {
    public:
        WrappingTimer();
        ~WrappingTimer();

        void update(float dt);
        float get() const { return (m_timer / m_period); }

        void adjustPeriod(float period);
        void setPeriod(float period) { m_period = period; }
        float getPeriod() const { return m_period; }

    protected:
        float m_timer;
        float m_period;
    };

} /* namespace c_adv */

#endif /* CEREAL_ADVENTURE_WRAPPING_TIMER_H */
