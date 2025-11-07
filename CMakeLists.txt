# Minimum required version for modern CMake features
cmake_minimum_required(VERSION 3.10)

# Define the project name
project(IPTracker CXX)

# Set the C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Enable testing support
enable_testing()

# --- 1. Dependency Finding ---
# Find CURL and GTest. These steps are now proven to work with the CI fix.
find_package(CURL REQUIRED)
find_package(GTest REQUIRED) 

# --- 2. Main Executable Target ---

# Define the executable target first.
add_executable(${PROJECT_NAME} src/ip_tracker.cpp)

# Apply includes to the newly defined target.
target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Link the executable against the necessary libraries.
# FIX: Use the variable ${CURL_LIBRARIES} instead of the target CURL::CURL
# to avoid target generation errors on some systems.
target_link_libraries(${PROJECT_NAME}
    PUBLIC
    ${CURL_LIBRARIES} # Links to the full path of libcurl found by find_package
    pthread           # For curl threading operations
)

# --- 3. Unit Test Target ---

# Define the test executable and specify its source file.
add_executable(IPTrackerTests tests/unit_tests.cpp)

# Apply include directories needed by the test.
target_include_directories(IPTrackerTests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Link the test executable with GTest.
target_link_libraries(IPTrackerTests 
    GTest::gtest_main  # Links with GTest framework
)

# Define a CTest test that executes the test binary
add_test(NAME run_unit_tests COMMAND IPTrackerTests)
