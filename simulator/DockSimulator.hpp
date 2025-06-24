#ifndef DOCKSIMULATOR_HPP
#define DOCKSIMULATOR_HPP

#include <string>
#include <unordered_set>
#include <nlohmann/json.hpp>

class DockSimulator {
public:
    DockSimulator();

    // Returns a JSON object representing the full On-Screen Display payload
    nlohmann::json getOSDPayload();

    // Returns a JSON object representing only the changed (dirty) fields; clears dirty state
    nlohmann::json getStatePayload();

    // Handle property/set commands: input is a JSON object containing fields to set or commands
    // Returns a JSON object indicating success (0) or failure (1) per field
    nlohmann::json handleSet(const nlohmann::json& setMsg);

private:
    // ---------- Properties (example ~50 fields; adjust per your spec) ----------
    bool silent_mode = false;
    int mode_code = 0;
    double temperature = 25.0;
    bool drone_in_dock = false;
    std::string gateway_sn = "UNKNOWN_SN";

    bool air_transfer_enabled = false;
    double battery_level = 100.0;
    int error_code = 0;
    std::string status_message = "";
    double latitude = 0.0;
    double longitude = 0.0;
    double altitude = 0.0;
    bool landing_gear_deployed = false;
    int mission_id = 0;
    std::string firmware_version = "v0.0.1";
    int signal_strength = 0;
    double cpu_usage = 0.0;
    double memory_usage = 0.0;
    double storage_usage = 0.0;
    bool is_charging = false;
    double charging_level = 0.0;
    double battery_health = 100.0;
    std::string last_maintenance_date = "";
    double flight_hours = 0.0;
    std::string geofence_status = "OK";
    bool obstacle_detected = false;
    std::string weather_condition = "Clear";
    std::string camera_status = "Idle";
    std::string sensor_status = "OK";
    std::string autopilot_mode = "Manual";
    bool return_to_home_enabled = true;
    bool emergency_flag = false;
    std::string communication_status = "Connected";
    bool gps_lock_status = true;
    double payload_weight = 0.0;
    double max_payload = 5.0;
    std::string software_version = "v0.0.1";
    std::string security_patch_level = "latest";
    long long last_update_timestamp = 0;
    // (Add or adjust fields here to match exactly your Week 2 specification; ensure ~50 total.)

    // State machine: current state
    enum class DroneState {
        Idle,
        PreFlight,
        InFlight,
        Returning,
        Charging,
        Maintenance,
        Emergency
    };
    DroneState current_state = DroneState::Idle;

    // Track which fields have been changed by handleSet()
    std::unordered_set<std::string> dirtyFields;

    // ---------- Helper declarations ----------
    void markDirty(const std::string& fieldName);
    std::string generateUUID();
    long long currentTimestampMillis();

    // State-machine helpers
    bool transitionTo(DroneState newState);
    std::string stateToString(DroneState s);
};

#endif // DOCKSIMULATOR_HPP
