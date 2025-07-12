#include "DockSimulator.hpp"

#include <chrono>
#include <random>
#include <iostream>
#include <sstream>            // ← Add this

DockSimulator::DockSimulator()
  : stateMachine(),
    current_state(DroneState::Idle),
    battery_level(100.0),
    temperature(25.0),
    silent_mode(false),
    gateway_sn("UNKNOWN_SN")
{}

nlohmann::json DockSimulator::getState() const {
    return {
        {"tid",       generateId()},
        {"bid",       generateId()},
        {"timestamp", getCurrentTimestamp()},
        {"gateway_sn", gateway_sn},
        {"data", {
            {"current_state", stateMachine.stateToString(current_state)},
            {"battery_level", battery_level},
            {"temperature", temperature},
            {"silent_mode", silent_mode},
            {"gateway_sn", gateway_sn}
        }}
    };
}

nlohmann::json DockSimulator::handleSet(const nlohmann::json& input) {
    nlohmann::json result;
    if (!input.is_object()) {
        return {{"error", "Invalid input"}};
    }

    for (auto& [key, val] : input.items()) {
        if (key == "battery_level" && val.is_number()) {
            double v = val.get<double>();
            result[key] = (v>=0.0 && v<=100.0 ? (battery_level=v, markDirty("battery_level"), 0) : 1);
        }
        else if (key == "temperature" && val.is_number()) {
            double v = val.get<double>();
            result[key] = (v>=-40.0 && v<=85.0 ? (temperature=v, markDirty("temperature"), 0) : 1);
        }
        else if (key == "silent_mode" && val.is_boolean()) {
            silent_mode = val.get<bool>();
            markDirty("silent_mode");
            result[key] = 0;
        }
        else if (key == "gateway_sn" && val.is_string()) {
            gateway_sn = val.get<std::string>();
            markDirty("gateway_sn");
            result[key] = 0;
        }
        else if (key == "command" && val.is_string()) {
            std::string cmd = val.get<std::string>();
            bool ok = stateMachine.processCommand(cmd);
            if (ok) {
                current_state = stateMachine.getCurrentState();
                markDirty("current_state");
            }
            result[key] = ok ? 0 : 1;
        }
        else {
            result[key] = 1;
        }
    }
    return result;
}

void DockSimulator::markDirty(const std::string& prop) {
    dirty_properties[prop] = true;
}

std::string DockSimulator::generateId() const {
    std::mt19937_64 rng(std::random_device{}());
    std::uniform_int_distribution<unsigned long long> dist;
    unsigned long long x = dist(rng);
    std::ostringstream o;
    o << std::hex << x;
    return o.str();
}

long long DockSimulator::getCurrentTimestamp() const {
    return std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
}

