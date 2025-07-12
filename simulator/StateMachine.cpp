#include "StateMachine.hpp"
#include <iostream>

StateMachine::StateMachine() : currentState(DroneState::Idle) {}

DroneState StateMachine::getCurrentState() const {
    return currentState;
}

bool StateMachine::transitionTo(DroneState newState) {
    if (isValidTransition(currentState, newState)) {
        currentState = newState;
        return true;
    }
    return false;
}

std::string StateMachine::stateToString(DroneState state) const {
    switch (state) {
        case DroneState::Idle:        return "Idle";
        case DroneState::PreFlight:   return "PreFlight";
        case DroneState::InFlight:    return "InFlight";
        case DroneState::Returning:   return "Returning";
        case DroneState::Charging:    return "Charging";
        case DroneState::Maintenance: return "Maintenance";
        case DroneState::Emergency:   return "Emergency";
    }
    return "Unknown";
}

bool StateMachine::processCommand(const std::string& command) {
    std::cout << "[StateMachine] Processing command: " << command << std::endl;
    if (command == "start" || command == "takeoff")        return transitionTo(DroneState::PreFlight);
    else if (command == "launch")                         return transitionTo(DroneState::InFlight);
    else if (command == "land")                           return transitionTo(DroneState::Returning);
    else if (command == "dock")                           return transitionTo(DroneState::Charging);
    else if (command == "complete_charge")                return transitionTo(DroneState::Idle);
    else if (command == "maintenance")                    return transitionTo(DroneState::Maintenance);
    else if (command == "emergency")                      return transitionTo(DroneState::Emergency);
    return false;
}

bool StateMachine::isValidTransition(DroneState, DroneState) const {
    // Accept all for demo
    return true;
}
