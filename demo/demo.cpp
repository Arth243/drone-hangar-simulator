#include "DockSimulator.hpp"
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>
#include <fstream>

// Utility: prompt user for input line
static std::string promptLine(const std::string& prompt) {
    std::cout << prompt;
    std::string line;
    std::getline(std::cin, line);
    return line;
}

// Utility: display menu
static void printMenu() {
    std::cout << "\n--- Drone Hangar Simulator CLI Menu ---\n";
    std::cout << "1. View full payload (getOSDPayload)\n";
    std::cout << "2. View state payload (getStatePayload)\n";
    std::cout << "3. Set property or send command via JSON\n";
    std::cout << "4. Load state from JSON file\n";
    std::cout << "5. Save state to JSON file\n";
    std::cout << "6. Exit\n";
    std::cout << "Select an option (1-6): ";
}

int main() {
    DockSimulator sim;
    std::cout << "=== Drone Hangar Simulator CLI ===\n";

    while (true) {
        printMenu();
        std::string choiceStr;
        if (!std::getline(std::cin, choiceStr)) {
            std::cout << "\nInput closed. Exiting.\n";
            break;
        }
        if (choiceStr.empty()) {
            continue;
        }
        int choice = 0;
        try {
            choice = std::stoi(choiceStr);
        } catch (...) {
            std::cout << "Invalid input. Please enter a number 1-6.\n";
            continue;
        }

        switch (choice) {
            case 1: { // Full payload
                try {
                    auto payload = sim.getOSDPayload();
                    std::cout << "Full payload:\n" << payload.dump(4) << "\n";
                } catch (const std::exception& e) {
                    std::cout << "Error retrieving full payload: " << e.what() << "\n";
                }
                break;
            }
            case 2: { // State payload
                try {
                    auto statePayload = sim.getStatePayload();
                    if (statePayload.is_null() || statePayload.empty()) {
                        std::cout << "No state changes since last retrieval.\n";
                    } else {
                        std::cout << "State payload:\n" << statePayload.dump(4) << "\n";
                    }
                } catch (const std::exception& e) {
                    std::cout << "Error retrieving state payload: " << e.what() << "\n";
                }
                break;
            }
            case 3: { // Set property or command via JSON
                std::string jsonLine = promptLine("Enter JSON (e.g., {\"silent_mode\": true} or {\"command\":\"takeoff\"}): ");
                if (jsonLine.empty()) {
                    std::cout << "Empty input, returning to menu.\n";
                    break;
                }
                try {
                    auto j = nlohmann::json::parse(jsonLine);
                    auto res = sim.handleSet(j);
                    std::cout << "handleSet result: " << res.dump(4) << "\n";
                } catch (const std::exception& e) {
                    std::cout << "Invalid JSON or error: " << e.what() << "\n";
                }
                break;
            }
            case 4: { // Load state from JSON file
                std::string filepath = promptLine("Enter JSON filename to load (e.g., state.json): ");
                if (filepath.empty()) {
                    std::cout << "Empty filename, returning to menu.\n";
                    break;
                }
                try {
                    sim.load_from_json(filepath);
                    std::cout << "State loaded from " << filepath << "\n";
                } catch (const std::exception& e) {
                    std::cout << "Error loading JSON file: " << e.what() << "\n";
                }
                break;
            }
            case 5: { // Save state to JSON file
                std::string filepath = promptLine("Enter JSON filename to save (e.g., updated_state.json): ");
                if (filepath.empty()) {
                    std::cout << "Empty filename, returning to menu.\n";
                    break;
                }
                try {
                    sim.save_to_json(filepath);
                    std::cout << "State saved to " << filepath << "\n";
                } catch (const std::exception& e) {
                    std::cout << "Error saving JSON file: " << e.what() << "\n";
                }
                break;
            }
            case 6: {
                std::cout << "Exiting CLI. Goodbye.\n";
                return 0;
            }
            default:
                std::cout << "Invalid choice. Please enter 1-6.\n";
                break;
        }
    }

    return 0;
}
