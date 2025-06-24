#include "DockSimulator.hpp"
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>

int main() {
    DockSimulator sim;
    std::cout << "Drone Hangar Simulator CLI\n";
    std::cout << "Enter JSON commands to set properties or commands, e.g., {\"silent_mode\": true} or {\"command\":\"takeoff\"}.\n";
    std::cout << "Type 'exit' to quit.\n";
    std::string line;
    while (true) {
        std::cout << "> ";
        if (!std::getline(std::cin, line)) break;
        if (line == "exit" || line == "quit") break;
        try {
            auto j = nlohmann::json::parse(line);
            auto res = sim.handleSet(j);
            std::cout << "handleSet result: " << res.dump() << "\n";
            auto state = sim.getStatePayload();
            if (!state.empty()) {
                std::cout << "State payload: " << state.dump(4) << "\n";
            } else {
                std::cout << "No changes to state.\n";
            }
        } catch (std::exception& e) {
            std::cout << "Invalid JSON or error: " << e.what() << "\n";
        }
    }
    std::cout << "Exiting.\n";
    return 0;
}
