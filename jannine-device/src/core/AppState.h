#pragma once

enum class RobotState {
    Idle,
    Listening,
    Talking
};

struct AppState {
    RobotState robotState = RobotState::Idle;

    void set(RobotState newState) {
        robotState = newState;
    }

    bool is(RobotState state) const {
        return robotState == state;
    }

    const char* toString() const {
        switch (robotState) {
            case RobotState::Idle:      return "Idle";
            case RobotState::Listening: return "Listening";
            case RobotState::Talking:   return "Talking";
            default:                    return "Unknown";
        }
    }
};
