cmake_minimum_required(VERSION 3.10)
project(drone-hangar-simulator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.2
)
FetchContent_MakeAvailable(nlohmann_json)

add_library(docksim
    simulator/DockSimulator.cpp
    simulator/StateMachine.cpp   # make sure this path is correct
)

target_include_directories(docksim PUBLIC simulator)
target_link_libraries(docksim PUBLIC nlohmann_json::nlohmann_json)

add_executable(core main.cpp)
target_link_libraries(core PRIVATE docksim)

