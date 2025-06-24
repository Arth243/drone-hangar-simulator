#include <gtest/gtest.h>
#include "DockSimulator.hpp"
#include <nlohmann/json.hpp>

// Utility: check if string is 32 hex chars
bool isHexUUID(const std::string& s) {
    if (s.size() != 32) return false;
    for (char c : s) {
        bool ok = (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
        if (!ok) return false;
    }
    return true;
}

TEST(DockSimulatorFullTest, DefaultOSDPayload) {
    DockSimulator sim;
    auto osd = sim.getOSDPayload();
    ASSERT_TRUE(osd.contains("tid"));
    ASSERT_TRUE(osd.contains("bid"));
    ASSERT_TRUE(osd.contains("timestamp"));
    ASSERT_TRUE(osd.contains("gateway_sn"));
    ASSERT_TRUE(osd.contains("data"));
    EXPECT_TRUE(isHexUUID(osd["tid"].get<std::string>()));
    EXPECT_TRUE(isHexUUID(osd["bid"].get<std::string>()));
    EXPECT_GT(osd["timestamp"].get<long long>(), 0);
    auto data = osd["data"];
    // Check some default values exist; adjust according to your defaults:
    EXPECT_TRUE(data.contains("silent_mode"));
    EXPECT_FALSE(data["silent_mode"].get<bool>());
    EXPECT_TRUE(data.contains("battery_level"));
    EXPECT_DOUBLE_EQ(data["battery_level"].get<double>(), 100.0);
    EXPECT_TRUE(data.contains("current_state"));
    EXPECT_EQ(data["current_state"].get<std::string>(), "Idle");
}

TEST(DockSimulatorFullTest, HandleSetAndStatePayload_SingleProperty) {
    DockSimulator sim;
    nlohmann::json j;
    j["silent_mode"] = true;
    auto res = sim.handleSet(j);
    ASSERT_TRUE(res.contains("silent_mode"));
    EXPECT_EQ(res["silent_mode"].get<int>(), 0);
    auto state = sim.getStatePayload();
    ASSERT_TRUE(state.contains("data"));
    auto data = state["data"];
    ASSERT_TRUE(data.contains("silent_mode"));
    EXPECT_TRUE(data["silent_mode"].get<bool>());
    // Subsequent getStatePayload returns empty
    auto emptyState = sim.getStatePayload();
    EXPECT_TRUE(emptyState.is_null() || emptyState.empty());
}

TEST(DockSimulatorFullTest, HandleSetInvalidField) {
    DockSimulator sim;
    nlohmann::json j;
    j["nonexistent_field"] = 123;
    auto res = sim.handleSet(j);
    ASSERT_TRUE(res.contains("nonexistent_field"));
    EXPECT_EQ(res["nonexistent_field"].get<int>(), 1);
    auto state = sim.getStatePayload();
    EXPECT_TRUE(state.is_null() || state.empty());
}

TEST(DockSimulatorFullTest, StateMachineValidTransition) {
    DockSimulator sim;
    // Idle -> PreFlight via "takeoff"
    nlohmann::json j1;
    j1["command"] = "takeoff";
    auto res1 = sim.handleSet(j1);
    EXPECT_EQ(res1["command"].get<int>(), 0);
    auto state1 = sim.getStatePayload();
    auto data1 = state1["data"];
    ASSERT_TRUE(data1.contains("current_state"));
    EXPECT_EQ(data1["current_state"].get<std::string>(), "PreFlight");
    // PreFlight -> InFlight via "launch"
    nlohmann::json j2;
    j2["command"] = "launch";
    auto res2 = sim.handleSet(j2);
    EXPECT_EQ(res2["command"].get<int>(), 0);
    auto state2 = sim.getStatePayload();
    auto data2 = state2["data"];
    EXPECT_EQ(data2["current_state"].get<std::string>(), "InFlight");
}

TEST(DockSimulatorFullTest, StateMachineInvalidTransition) {
    DockSimulator sim;
    // Without takeoff, try "launch" directly from Idle: expect failure
    nlohmann::json j;
    j["command"] = "launch";
    auto res = sim.handleSet(j);
    EXPECT_EQ(res["command"].get<int>(), 1);
    auto state = sim.getStatePayload();
    EXPECT_TRUE(state.is_null() || state.empty());
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
