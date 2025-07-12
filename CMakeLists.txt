cmake_minimum_required(VERSION 3.10)
project(drone-hangar-simulator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Fetch nlohmann/json
include(FetchContent)
FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.2
)
FetchContent_MakeAvailable(nlohmann_json)

# Create the simulator library
add_library(docksim
    simulator/DockSimulator.cpp
    simulator/StateMachine.cpp
)

target_include_directories(docksim PUBLIC simulator)
target_link_libraries(docksim PUBLIC nlohmann_json::nlohmann_json)

# Create the main executable
add_executable(core main.cpp)
target_link_libraries(core PRIVATE docksim)

# Create demo executable if demo exists
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/demo/demo.cpp)
    add_executable(demo demo/demo.cpp)
    target_link_libraries(demo PRIVATE docksim)
endif()
