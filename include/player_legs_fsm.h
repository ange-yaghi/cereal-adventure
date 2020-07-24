#ifndef CEREAL_ADVENTURE_PLAYER_LEGS_FSM_H
#define CEREAL_ADVENTURE_PLAYER_LEGS_FSM_H

namespace c_adv {

    class Player;

    class PlayerLegsFsm {
        friend Player;

    public:
        enum class State {
            Running,
            Idle,
            Falling,
            FastFalling,
            Hanging,
            ImpactDamage,
            Dying,
            Undefined
        };

        struct FsmResults {
            State next;
            State queued;

            float nextFade;
            float nextClip;
            float queuedFade;
            float queuedClip;
        };

    public:
        PlayerLegsFsm();
        ~PlayerLegsFsm();

        void initialize(Player *player);
        void nextState(FsmResults &result);

        void updateState(State state) { m_currentState = state; }
        State getState() const { return m_currentState; }

    protected:
        Player *m_player;
        State m_currentState;
    };

} /* namespace c_adv */

#endif /* CEREAL_ADVENTURE_PLAYER_LEGS_FSM_H */
