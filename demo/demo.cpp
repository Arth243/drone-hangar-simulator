#include "DockSimulator.hpp"
#include <iostream>
#include <nlohmann/json.hpp>

int main() {
    DockSimulator simulator;
    nlohmann::json osd = simulator.getOSDPayload();
    // Print JSON payload to stdout
    std::cout << osd.dump(4) << std::endl;
    return 0;
}
