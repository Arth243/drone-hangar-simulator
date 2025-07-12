#ifndef STATEMACHINE_HPP
#define STATEMACHINE_HPP

#include <string>

enum class DroneState {
    Idle,
    PreFlight,
    InFlight,
    Returning,
    Charging,
    Maintenance,
    Emergency
};

class StateMachine {
public:
    StateMachine();

    DroneState getCurrentState() const;
    bool transitionTo(DroneState newState);
    std::string stateToString(DroneState state) const;
    bool processCommand(const std::string& command);

private:
    DroneState currentState;
    bool isValidTransition(DroneState from, DroneState to) const;
};

#endif // STATEMACHINE_HPP

