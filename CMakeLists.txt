cmake_minimum_required(VERSION 3.14)
project(DroneHangarSimulator LANGUAGES CXX)

# Use C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# Fetch or find nlohmann_json
find_package(nlohmann_json QUIET)
if (NOT nlohmann_json_FOUND)
    message(STATUS "nlohmann_json not found, fetching via FetchContent")
    FetchContent_Declare(
        json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG v3.11.2  # adjust to latest stable if desired
    )
    FetchContent_MakeAvailable(json)
endif()

# Simulator library
add_library(docksim
    simulator/DockSimulator.cpp
)
target_include_directories(docksim PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/simulator
)
target_link_libraries(docksim PRIVATE nlohmann_json::nlohmann_json)

# Demo executable
add_executable(demo
    demo/demo.cpp
)
target_link_libraries(demo PRIVATE docksim)

# Testing
include(CTest)
enable_testing()

# Fetch GoogleTest
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.13.0
)
# Prevent GoogleTest from overriding compiler settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Collect test source files in tests/ folder
file(GLOB TEST_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp)
if (TEST_SOURCES)
    add_executable(runUnitTests ${TEST_SOURCES})
    target_link_libraries(runUnitTests PRIVATE docksim gtest_main nlohmann_json::nlohmann_json)
    include(GoogleTest)
    gtest_discover_tests(runUnitTests)
endif()
