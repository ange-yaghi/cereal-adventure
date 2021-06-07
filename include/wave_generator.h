#ifndef CEREAL_ADVENTURE_WAVE_GENERATOR_H
#define CEREAL_ADVENTURE_WAVE_GENERATOR_H

namespace c_adv {

    class WaveGenerator {
    public:
        WaveGenerator();
        ~WaveGenerator();

        void update(float dt);
        float get() const { return m_f; }

        void setPeriod(float period) { m_period = period; }
        float getPeriod() const { return m_period; }

    protected:
        float m_timer;
        float m_period;
        float m_f;
    };

} /* namespace c_adv */

#endif /* CEREAL_ADVENTURE_WAVE_GENERATOR_H */
