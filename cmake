# Minimum required version for modern CMake features
cmake_minimum_required(VERSION 3.10)

# Define the project name
project(IPTracker CXX)

# Set the C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# --- Dependency Configuration ---

# 1. Find the required system libraries (libcurl)
find_package(CURL REQUIRED)

# --- Directory Configuration ---

# Add the 'include' directory so the compiler can find json.hpp and cpr.h
target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# --- Executable Creation ---

# Define the executable target and specify its source file
add_executable(${PROJECT_NAME} src/ip_tracker.cpp)

# --- Linking Libraries ---

# Link the executable against the necessary libraries
target_link_libraries(${PROJECT_NAME}
    PUBLIC
    CURL::CURL      # Links against libcurl
    pthread         # For thread safety/asynchronous operations often needed by curl
)
