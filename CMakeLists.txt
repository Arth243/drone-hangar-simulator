cmake_minimum_required(VERSION 3.10)
project(drone-hangar-simulator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ----- Fetch JSON library first -----
include(FetchContent)
FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.2
)
FetchContent_MakeAvailable(nlohmann_json)

# ----- Library -----
add_library(docksim
    simulator/DockSimulator.cpp
    simulator/StateMachine.cpp
    simulator/DockSimulator.hpp
    simulator/StateMachine.hpp
)

target_include_directories(docksim PUBLIC simulator)
target_link_libraries(docksim PUBLIC nlohmann_json::nlohmann_json)

# ----- Core executable -----
add_executable(core main.cpp)
target_link_libraries(core PRIVATE docksim)

# ----- Demo executable -----
add_executable(demo demo/demo.cpp)
target_link_libraries(demo PRIVATE docksim)
