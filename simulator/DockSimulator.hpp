#ifndef DOCKSIMULATOR_HPP
#define DOCKSIMULATOR_HPP

#include <string>
#include <unordered_map>
#include <nlohmann/json.hpp>
#include "StateMachine.hpp"

class DockSimulator {
public:
    DockSimulator();

    // Returns a JSON describing all current properties
    nlohmann::json getState() const;

    // Applies settings/commands from input JSON, returns result codes
    nlohmann::json handleSet(const nlohmann::json& input);

private:
    StateMachine stateMachine;
    DroneState    current_state;
    double        battery_level;
    double        temperature;
    bool          silent_mode;
    std::string   gateway_sn;

    // Tracks which fields were modified
    std::unordered_map<std::string, bool> dirty_properties;

    void markDirty(const std::string& property);
    std::string generateId() const;
    long long getCurrentTimestamp() const;
};

#endif // DOCKSIMULATOR_HPP
