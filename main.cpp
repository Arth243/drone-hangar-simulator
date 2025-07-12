#include <iostream>
#include <nlohmann/json.hpp>
#include "DockSimulator.hpp"

int main() {
    DockSimulator dock;
auto state = dock.getState();   // not getStatePayload()

    std::cout << "Initial State:\n" 
              << state.dump(2) << "\n\n";

    // 2) Send a simple command:
    nlohmann::json cmd = {{"command", "start"}};
    auto response = dock.handleSet(cmd);
    std::cout << "Response:\n" 
              << response.dump(2) << "\n";

    return 0;
}
