#include "simulator/DockSimulator.hpp"
#include <iostream>

int main() {
    DockSimulator dock;

    try {
        dock.load_from_json("state.json");
    } catch (const std::exception& e) {
        std::cerr << "Error loading state: " << e.what() << std::endl;
        return 1;
    }

    std::cout << "Loaded state:" << std::endl;
    std::cout << "Drone Count: " << dock.get_property("drone_count") << std::endl;
    std::cout << "Status: " << dock.get_property("status") << std::endl;

    // Change a property
    dock.set_property("status", "active");

    try {
        dock.save_to_json("updated_state.json");
    } catch (const std::exception& e) {
        std::cerr << "Error saving state: " << e.what() << std::endl;
        return 1;
    }

    std::cout << "Updated status and saved to updated_state.json" << std::endl;
    return 0;
}
