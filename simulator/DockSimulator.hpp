#ifndef DOCKSIMULATOR_HPP
#define DOCKSIMULATOR_HPP

#include <string>
#include <unordered_set>
#include <nlohmann/json.hpp>  // JSON library

class DockSimulator {
public:
    DockSimulator();

    // Method stubs
    // Returns a JSON object representing the full On-Screen Display payload
    nlohmann::json getOSDPayload();

    // Returns a JSON object representing only the changed (dirty) fields
    nlohmann::json getStatePayload();

    // Handle property/set commands: input is a JSON object containing fields to set
    // Returns a JSON object indicating success (0) or failure (1) per field
    nlohmann::json handleSet(const nlohmann::json& setMsg);

private:
    // Example properties (add ~50 fields according to your spec/DJI API docs)
    bool silent_mode = false;
    int mode_code = 0;
    double temperature = 25.0;
    bool drone_in_dock = false;
    std::string gateway_sn = "UNKNOWN_SN";

    // TODO: Add additional properties (~50 fields total), e.g.:
    // bool air_transfer_enabled = false;
    // double battery_level = 100.0;
    // int error_code = 0;
    // std::string status_message = "";
    // ...
    // You can refer to the DJI API or your spec to list all fields here.

    // Track which fields have been changed by handleSet()
    std::unordered_set<std::string> dirtyFields;

    // Internal helper: mark a field dirty by name
    void markDirty(const std::string& fieldName);
};

#endif // DOCKSIMULATOR_HPP
