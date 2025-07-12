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

    // Returns the current state
    DroneState getCurrentState() const;

    // Attempts to transition; returns true on success
    bool transitionTo(DroneState newState);

    // Converts a state enum to its string name
    std::string stateToString(DroneState state) const;

    // Processes a textual command, mapping it to a transition
    bool processCommand(const std::string& command);

private:
    DroneState currentState;

    // Validates if from→to is allowed (here, all transitions allowed)
    bool isValidTransition(DroneState from, DroneState to) const;
};

#endif // STATEMACHINE_HPP

#endif // STATEMACHINE_HPP
