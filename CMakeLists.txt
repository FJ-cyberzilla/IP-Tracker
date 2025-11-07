# Minimum required version for modern CMake features
cmake_minimum_required(VERSION 3.10)

# Define the project name
project(IPTracker CXX)

# Set the C++ standard to C++17 (required for modern features)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Enable testing support
enable_testing()

# --- 1. Dependency Configuration ---

# Find required system libraries
# CURL is needed for the cpr library
find_package(CURL REQUIRED)
# GTest is needed for unit testing
find_package(GTest REQUIRED) 

# --- 2. Shared Header Configuration ---

# Add the 'include' directory to the compiler's search path.
# This allows C++ files to find headers like "json/json.hpp" and "cpr/cpr.h".
target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# --- 3. Main Executable Target ---

# Define the executable target and specify its source file
add_executable(${PROJECT_NAME} src/ip_tracker.cpp)

# Link the executable against the necessary libraries
target_link_libraries(${PROJECT_NAME}
    PUBLIC
    CURL::CURL      # Link against libcurl
    pthread         # Often required for curl threading operations
)

# --- 4. Unit Test Target (Requires 'tests/unit_tests.cpp') ---

# Add the test executable
add_executable(IPTrackerTests tests/unit_tests.cpp)

# Link the test executable with GTest and the main executable's includes
target_link_libraries(IPTrackerTests 
    # Link against GTest framework
    GTest::gtest_main  
    # Link against the main executable target to inherit its public includes and dependencies
    ${PROJECT_NAME}
)

# Define a CTest test that executes the test binary
add_test(NAME run_unit_tests COMMAND IPTrackerTests)
