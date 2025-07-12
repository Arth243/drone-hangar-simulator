#ifndef STATEMACHINE_HPP
#define STATEMACHINE_HPP

#include <string>
#include <nlohmann/json.hpp>

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
    
    // Get current state
    DroneState getCurrentState() const;
    
    // Transition to new state
    bool transitionTo(DroneState newState);
    
    // Convert state to string
    std::string stateToString(DroneState state) const;
    
    // Process command
    bool processCommand(const std::string& command);
    
private:
    DroneState currentState;
    bool isValidTransition(DroneState from, DroneState to) const;
};

#endif // STATEMACHINE_HPP
