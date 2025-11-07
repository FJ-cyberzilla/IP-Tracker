'# Minimum required version for modern CMake features
cmake_minimum_required(VERSION 3.10)

# Define the project name
project(IPTracker CXX)

# Set the C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

enable_testing()

# --- 1. Dependency Finding ---
find_package(CURL REQUIRED)
find_package(GTest REQUIRED) 

# --- 2. Utility Library Target (The FIX) ---
# Create a library containing shared functions like safe_get_string
add_library(IPTrackerUtils src/utils.cpp)

# Define includes for the library (needed for utils.cpp to find json/json.hpp)
target_include_directories(IPTrackerUtils PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# --- 3. Main Executable Target ---

# Define the executable target
add_executable(${PROJECT_NAME} src/ip_tracker.cpp)

# Link the executable against the new utility library and external dependencies
target_link_libraries(${PROJECT_NAME}
    PUBLIC
    IPTrackerUtils      # <--- Link to our shared utilities
    ${CURL_LIBRARIES}   # <--- Link to system curl (using robust variable)
    pthread
)

# FIX: Explicitly add CURL includes needed for cpr/cpr.h in the main executable
target_include_directories(${PROJECT_NAME} PUBLIC
    ${CURL_INCLUDE_DIRS} 
)

# --- 4. Unit Test Target ---

# Define the test executable and specify its source file.
add_executable(IPTrackerTests tests/unit_tests.cpp)

# Link the test executable against GTest AND the shared utility library.
target_link_libraries(IPTrackerTests 
    GTest::gtest_main     # Links with GTest framework
    IPTrackerUtils        # <--- Link to our shared utilities (The FIX)
)

# Define a CTest test that executes the test binary
add_test(NAME run_unit_tests COMMAND IPTrackerTests)
