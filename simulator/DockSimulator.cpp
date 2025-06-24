#include "DockSimulator.hpp"
#include <chrono>
#include <random>
#include <sstream>
#include <iomanip>
#include <nlohmann/json.hpp>

// Constructor
DockSimulator::DockSimulator() {
    // Initialize timestamp
    last_update_timestamp = currentTimestampMillis();
    // Other initialization if needed
}

// Helper: generate a pseudo-UUID: 32 hex chars
std::string DockSimulator::generateUUID() {
    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_int_distribution<uint64_t> dis;
    uint64_t p1 = dis(gen);
    uint64_t p2 = dis(gen);
    std::ostringstream oss;
    oss << std::hex << std::setfill('0')
        << std::setw(16) << p1
        << std::setw(16) << p2;
    return oss.str();
}

// Helper: current time in milliseconds since epoch
long long DockSimulator::currentTimestampMillis() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()
    ).count();
}

// Mark a field as dirty
void DockSimulator::markDirty(const std::string& fieldName) {
    dirtyFields.insert(fieldName);
}

// Convert state enum to string
std::string DockSimulator::stateToString(DroneState s) {
    switch (s) {
        case DroneState::Idle: return "Idle";
        case DroneState::PreFlight: return "PreFlight";
        case DroneState::InFlight: return "InFlight";
        case DroneState::Returning: return "Returning";
        case DroneState::Charging: return "Charging";
        case DroneState::Maintenance: return "Maintenance";
        case DroneState::Emergency: return "Emergency";
        default: return "Unknown";
    }
}

// Transition to a new state
bool DockSimulator::transitionTo(DroneState newState) {
    // Simple example: allow any transition.
    // For more complex rules, insert logic here.
    if (current_state != newState) {
        current_state = newState;
        markDirty("current_state");
        return true;
    }
    return false;
}

// getOSDPayload: serialize all properties plus metadata
nlohmann::json DockSimulator::getOSDPayload() {
    nlohmann::json payload;
    nlohmann::json data;

    // Serialize each property:
    data["silent_mode"] = silent_mode;
    data["mode_code"] = mode_code;
    data["temperature"] = temperature;
    data["drone_in_dock"] = drone_in_dock;
    data["gateway_sn"] = gateway_sn;
    data["air_transfer_enabled"] = air_transfer_enabled;
    data["battery_level"] = battery_level;
    data["error_code"] = error_code;
    data["status_message"] = status_message;
    data["latitude"] = latitude;
    data["longitude"] = longitude;
    data["altitude"] = altitude;
    data["landing_gear_deployed"] = landing_gear_deployed;
    data["mission_id"] = mission_id;
    data["firmware_version"] = firmware_version;
    data["signal_strength"] = signal_strength;
    data["cpu_usage"] = cpu_usage;
    data["memory_usage"] = memory_usage;
    data["storage_usage"] = storage_usage;
    data["is_charging"] = is_charging;
    data["charging_level"] = charging_level;
    data["battery_health"] = battery_health;
    data["last_maintenance_date"] = last_maintenance_date;
    data["flight_hours"] = flight_hours;
    data["geofence_status"] = geofence_status;
    data["obstacle_detected"] = obstacle_detected;
    data["weather_condition"] = weather_condition;
    data["camera_status"] = camera_status;
    data["sensor_status"] = sensor_status;
    data["autopilot_mode"] = autopilot_mode;
    data["return_to_home_enabled"] = return_to_home_enabled;
    data["emergency_flag"] = emergency_flag;
    data["communication_status"] = communication_status;
    data["gps_lock_status"] = gps_lock_status;
    data["payload_weight"] = payload_weight;
    data["max_payload"] = max_payload;
    data["software_version"] = software_version;
    data["security_patch_level"] = security_patch_level;
    data["last_update_timestamp"] = currentTimestampMillis();
    // State:
    data["current_state"] = stateToString(current_state);
    // (Add any additional fields if present)

    // Metadata
    payload["tid"] = generateUUID();
    payload["bid"] = generateUUID();
    payload["timestamp"] = currentTimestampMillis();
    payload["gateway_sn"] = gateway_sn;
    payload["data"] = data;
    return payload;
}

// getStatePayload: serialize only dirty fields + metadata, then clear dirtyFields
nlohmann::json DockSimulator::getStatePayload() {
    nlohmann::json payload;
    if (dirtyFields.empty()) {
        return {}; // no changes
    }
    nlohmann::json data;
    for (const auto& field : dirtyFields) {
        if (field == "silent_mode") data["silent_mode"] = silent_mode;
        else if (field == "mode_code") data["mode_code"] = mode_code;
        else if (field == "temperature") data["temperature"] = temperature;
        else if (field == "drone_in_dock") data["drone_in_dock"] = drone_in_dock;
        else if (field == "gateway_sn") data["gateway_sn"] = gateway_sn;
        else if (field == "air_transfer_enabled") data["air_transfer_enabled"] = air_transfer_enabled;
        else if (field == "battery_level") data["battery_level"] = battery_level;
        else if (field == "error_code") data["error_code"] = error_code;
        else if (field == "status_message") data["status_message"] = status_message;
        else if (field == "latitude") data["latitude"] = latitude;
        else if (field == "longitude") data["longitude"] = longitude;
        else if (field == "altitude") data["altitude"] = altitude;
        else if (field == "landing_gear_deployed") data["landing_gear_deployed"] = landing_gear_deployed;
        else if (field == "mission_id") data["mission_id"] = mission_id;
        else if (field == "firmware_version") data["firmware_version"] = firmware_version;
        else if (field == "signal_strength") data["signal_strength"] = signal_strength;
        else if (field == "cpu_usage") data["cpu_usage"] = cpu_usage;
        else if (field == "memory_usage") data["memory_usage"] = memory_usage;
        else if (field == "storage_usage") data["storage_usage"] = storage_usage;
        else if (field == "is_charging") data["is_charging"] = is_charging;
        else if (field == "charging_level") data["charging_level"] = charging_level;
        else if (field == "battery_health") data["battery_health"] = battery_health;
        else if (field == "last_maintenance_date") data["last_maintenance_date"] = last_maintenance_date;
        else if (field == "flight_hours") data["flight_hours"] = flight_hours;
        else if (field == "geofence_status") data["geofence_status"] = geofence_status;
        else if (field == "obstacle_detected") data["obstacle_detected"] = obstacle_detected;
        else if (field == "weather_condition") data["weather_condition"] = weather_condition;
        else if (field == "camera_status") data["camera_status"] = camera_status;
        else if (field == "sensor_status") data["sensor_status"] = sensor_status;
        else if (field == "autopilot_mode") data["autopilot_mode"] = autopilot_mode;
        else if (field == "return_to_home_enabled") data["return_to_home_enabled"] = return_to_home_enabled;
        else if (field == "emergency_flag") data["emergency_flag"] = emergency_flag;
        else if (field == "communication_status") data["communication_status"] = communication_status;
        else if (field == "gps_lock_status") data["gps_lock_status"] = gps_lock_status;
        else if (field == "payload_weight") data["payload_weight"] = payload_weight;
        else if (field == "max_payload") data["max_payload"] = max_payload;
        else if (field == "software_version") data["software_version"] = software_version;
        else if (field == "security_patch_level") data["security_patch_level"] = security_patch_level;
        else if (field == "last_update_timestamp") data["last_update_timestamp"] = currentTimestampMillis();
        else if (field == "current_state") data["current_state"] = stateToString(current_state);
        // (Add other fields if needed)
    }
    payload["tid"] = generateUUID();
    payload["bid"] = generateUUID();
    payload["timestamp"] = currentTimestampMillis();
    payload["gateway_sn"] = gateway_sn;
    payload["data"] = data;
    dirtyFields.clear();
    return payload;
}

// handleSet: set properties or commands, mark dirty, return result codes
nlohmann::json DockSimulator::handleSet(const nlohmann::json& setMsg) {
    nlohmann::json result;
    if (!setMsg.is_object()) {
        result["error"] = "Expected JSON object";
        return result;
    }
    for (auto it = setMsg.begin(); it != setMsg.end(); ++it) {
        const std::string& key = it.key();
        try {
            // Example property sets:
            if (key == "silent_mode" && it.value().is_boolean()) {
                silent_mode = it.value().get<bool>();
                markDirty(key);
                result[key] = 0;
            } else if (key == "mode_code" && it.value().is_number_integer()) {
                mode_code = it.value().get<int>();
                markDirty(key);
                result[key] = 0;
            } else if (key == "temperature" && it.value().is_number()) {
                temperature = it.value().get<double>();
                markDirty(key);
                result[key] = 0;
            }
            // ... repeat for each property with appropriate type checks ...
            else if (key == "battery_level" && it.value().is_number()) {
                battery_level = it.value().get<double>();
                markDirty(key);
                result[key] = 0;
            } else if (key == "status_message" && it.value().is_string()) {
                status_message = it.value().get<std::string>();
                markDirty(key);
                result[key] = 0;
            }
            // State-machine commands:
            else if (key == "command" && it.value().is_string()) {
                std::string cmd = it.value().get<std::string>();
                bool ok = false;
                // Example: Idle -> PreFlight on "takeoff"
                if (cmd == "takeoff" && current_state == DroneState::Idle) {
                    ok = transitionTo(DroneState::PreFlight);
                }
                // Example: PreFlight -> InFlight on "launch"
                else if (cmd == "launch" && current_state == DroneState::PreFlight) {
                    ok = transitionTo(DroneState::InFlight);
                }
                // Example: InFlight -> Returning on "land"
                else if (cmd == "land" && current_state == DroneState::InFlight) {
                    ok = transitionTo(DroneState::Returning);
                }
                // Example: Returning -> Charging on "dock"
                else if (cmd == "dock" && current_state == DroneState::Returning) {
                    ok = transitionTo(DroneState::Charging);
                }
                // Example: Charging -> Idle when fully charged
                else if (cmd == "complete_charge" && current_state == DroneState::Charging) {
                    ok = transitionTo(DroneState::Idle);
                }
                // Emergency command
                else if (cmd == "emergency" ) {
                    ok = transitionTo(DroneState::Emergency);
                }
                // Add other transitions per your spec
                if (ok) {
                    result[key] = 0;
                } else {
                    result[key] = 1;
                }
            }
            else {
                // Unrecognized field or wrong type
                result[key] = 1;
            }
        } catch (...) {
            result[key] = 1;
        }
    }
    return result;
}
