#ifndef CEREAL_ADVENTURE_CLOCK_H
#define CEREAL_ADVENTURE_CLOCK_H

namespace c_adv {

    class Clock {
    public:
        Clock();
        ~Clock();

        bool isEnabled() const { return m_enabled; }
        void setEnabled(bool enabled) { m_enabled = enabled; }

        bool getState() { return m_inverted ? !m_state : m_state; }

        bool isInverted() const { return m_inverted; }
        void setInverted(bool inverted) { m_inverted = inverted; }

        void reset();
        void update(float dt);

        void setLowTime(float lt) { m_lT = lt; }
        float getLowTime() const { return m_lT; }

        void setHighTime(float ht) { m_hT = ht; }
        float getHighTime() const { return m_hT; }

    protected:
        float m_t;

        float m_hT;
        float m_lT;

        bool m_enabled;
        bool m_state;
        bool m_inverted;
    };

} /* namespace c_adv */

#endif /* CEREAL_ADVENTURE_CLOCK_H */

