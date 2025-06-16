#include "DockSimulator.hpp"
#include <uuid/uuid.h>      // For UUID generation if you have a UUID library
#include <chrono>           // For timestamp
#include <iostream>

DockSimulator::DockSimulator() {
    // Constructor: initialize defaults if needed
    // e.g., silent_mode = false, mode_code = 0, etc. Already default-initialized.
}

nlohmann::json DockSimulator::getOSDPayload() {
    nlohmann::json payload;
    nlohmann::json data;

    // TODO: Serialize all internal fields into 'data'
    // Example:
    data["silent_mode"] = silent_mode;
    data["mode_code"] = mode_code;
    data["temperature"] = temperature;
    data["drone_in_dock"] = drone_in_dock;
    // TODO: add other fields

    // Wrap metadata: tid, bid, timestamp, gateway SN
    // For now, use placeholders
    payload["tid"] = "TID_PLACEHOLDER"; 
    payload["bid"] = "BID_PLACEHOLDER";
    payload["timestamp"] = std::chrono::duration_cast<std::chrono::milliseconds>(
                                std::chrono::system_clock::now().time_since_epoch()
                            ).count();
    payload["gateway_sn"] = gateway_sn;
    payload["data"] = data;

    return payload;
}

nlohmann::json DockSimulator::getStatePayload() {
    nlohmann::json payload;

    // If no dirty fields, return empty JSON
    if (dirtyFields.empty()) {
        return {};
    }
    nlohmann::json data;
    // TODO: For each field in dirtyFields, include updated value:
    for (const auto& field : dirtyFields) {
        // Example: handle known fields; in real code, use if-else or map of pointers
        if (field == "silent_mode") {
            data["silent_mode"] = silent_mode;
        } else if (field == "mode_code") {
            data["mode_code"] = mode_code;
        } else if (field == "temperature") {
            data["temperature"] = temperature;
        } else if (field == "drone_in_dock") {
            data["drone_in_dock"] = drone_in_dock;
        }
        // TODO: handle other fields similarly
    }

    payload["tid"] = "TID_PLACEHOLDER";
    payload["bid"] = "BID_PLACEHOLDER";
    payload["timestamp"] = std::chrono::duration_cast<std::chrono::milliseconds>(
                                std::chrono::system_clock::now().time_since_epoch()
                            ).count();
    payload["gateway_sn"] = gateway_sn;
    payload["data"] = data;

    // Clear dirtyFields after packaging
    dirtyFields.clear();
    return payload;
}

nlohmann::json DockSimulator::handleSet(const nlohmann::json& setMsg) {
    nlohmann::json result;
    // Expect setMsg is an object with key-value pairs to set
    if (!setMsg.is_object()) {
        // Invalid format
        result["error"] = "Expected JSON object for setMsg";
        return result;
    }
    for (auto it = setMsg.begin(); it != setMsg.end(); ++it) {
        const std::string& key = it.key();
        try {
            if (key == "silent_mode" && it.value().is_boolean()) {
                silent_mode = it.value().get<bool>();
                markDirty(key);
                result[key] = 0;
            } else if (key == "mode_code" && it.value().is_number_integer()) {
                mode_code = it.value().get<int>();
                markDirty(key);
                result[key] = 0;
            } else if (key == "temperature" && (it.value().is_number_float() || it.value().is_number_integer())) {
                temperature = it.value().get<double>();
                markDirty(key);
                result[key] = 0;
            } else if (key == "drone_in_dock" && it.value().is_boolean()) {
                drone_in_dock = it.value().get<bool>();
                markDirty(key);
                result[key] = 0;
            }
            // TODO: handle other writable fields similarly
            else {
                // Field not recognized or wrong type
                result[key] = 1;
            }
        } catch (...) {
            // In case of conversion error
            result[key] = 1;
        }
    }
    return result;
}

void DockSimulator::markDirty(const std::string& fieldName) {
    dirtyFields.insert(fieldName);
}
