cmake_minimum_required(VERSION 3.10)
project(DroneHangarSimulator)

# Use C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find nlohmann_json (you need it installed or provide it later)
find_package(nlohmann_json REQUIRED)

# Add simulator library
add_library(docksim
    simulator/DockSimulator.cpp
)
# Include headers
target_include_directories(docksim PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/simulator
)
# Link JSON library
target_link_libraries(docksim PRIVATE nlohmann_json::nlohmann_json)

# Demo executable
add_executable(demo
    demo/demo.cpp
)
target_link_libraries(demo PRIVATE docksim)

# Tests (empty for now; will add in Week 3/4)
enable_testing()
# You can add test executables in /tests later
