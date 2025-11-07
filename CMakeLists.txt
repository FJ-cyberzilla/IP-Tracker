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
# Find CURL and GTest.
find_package(CURL REQUIRED)
find_package(GTest REQUIRED) 

# --- 2. Main Executable Target ---

# Define the executable target first.
add_executable(${PROJECT_NAME} src/ip_tracker.cpp)

# Apply includes to the newly defined target.
target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    # FIX: Explicitly add the system's CURL include directories found by find_package
    ${CURL_INCLUDE_DIRS} 
)

# Link the executable against the necessary libraries.
# FIX: Use the variable ${CURL_LIBRARIES} instead of the target CURL::CURL
target_link_libraries(${PROJECT_NAME}
    PUBLIC
    ${CURL_LIBRARIES} # Links to the full path of libcurl
    pthread           # For curl threading operations
)

# --- 3. Unit Test Target ---

# Define the test executable and specify its source file.
add_executable(IPTrackerTests tests/unit_tests.cpp)

# Apply include directories needed by the test.
target_include_directories(IPTrackerTests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CURL_INCLUDE_DIRS} # Also needed for the tests if they access cpr/curl
)

# Link the test executable with GTest.
target_link_libraries(IPTrackerTests 
    GTest::gtest_main  # Links with GTest framework
    ${CURL_LIBRARIES}  # Ensure tests can also link against curl if needed
    pthread
)

# Define a CTest test that executes the test binary
add_test(NAME run_unit_tests COMMAND IPTrackerTests)
