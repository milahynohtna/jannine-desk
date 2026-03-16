#pragma once

#include "../core/AppState.h"

class StateMachine {
public:

    StateMachine()
    {
        _state.set(RobotState::Idle);
    }

    void set(RobotState state)
    {
        _prev = _state.robotState;
        _state.set(state);
    }

    bool is(RobotState state) const
    {
        return _state.is(state);
    }

    RobotState current() const
    {
        return _state.robotState;
    }

    RobotState previous() const
    {
        return _prev;
    }

private:

    AppState _state;
    RobotState _prev = RobotState::Idle;
};