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
find_package(CURL REQUIRED)
find_package(GTest REQUIRED) 

# --- 2. Main Executable Target ---

# Define the executable target FIRST.
add_executable(${PROJECT_NAME} src/ip_tracker.cpp)

# Now apply includes and linking to the newly defined target.
target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Link the executable against the necessary libraries
target_link_libraries(${PROJECT_NAME}
    PUBLIC
    CURL::CURL      # Link against libcurl
    pthread         # For curl threading operations
)

# --- 3. Unit Test Target ---

# Define the test executable and specify its source file.
add_executable(IPTrackerTests tests/unit_tests.cpp)

# Apply include directories needed by the test (inheriting from the main project)
# We use PRIVATE because the tests are the final consumer of these includes.
target_include_directories(IPTrackerTests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    src # Allows access to headers/files in src/ if needed
)

# Link the test executable with GTest. 
# It does NOT link directly to IPTracker, but it links to the source files 
# via the tests/unit_tests.cpp file which must contain or link to the tested code.
target_link_libraries(IPTrackerTests 
    GTest::gtest_main  # Links with GTest framework
    # Since we copied safe_get_string into unit_tests.cpp previously, 
    # we don't need to link IPTracker. If we moved the function 
    # to a library, we would link that library here.
)

# Define a CTest test that executes the test binary
add_test(NAME run_unit_tests COMMAND IPTrackerTests)
