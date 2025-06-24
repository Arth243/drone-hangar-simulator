#include "simulator/DockSimulator.hpp"
#include <iostream>

int main() {
    DockSimulator dock;

    // Set some initial values
    dock.set_property("drone_count", 5);
    dock.set_property("status", "idle");

    // Print values
    std::cout << "Drone Count: " << dock.get_property("drone_count") << std::endl;
    std::cout << "Status: " << dock.get_property("status") << std::endl;

    // Simulate a transition
    dock.set_property("status", "active");

    std::cout << "Status changed to: " << dock.get_property("status") << std::endl;

    return 0;
}
