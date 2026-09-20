cmake_minimum_required(VERSION 3.14)
project(IPTracker)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# Fetch CPR
FetchContent_Declare(
    cpr
    GIT_REPOSITORY https://github.com/libcpr/cpr.git
    GIT_TAG 1.10.5
)

# Fetch nlohmann_json
FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.3
)

FetchContent_MakeAvailable(cpr nlohmann_json)

# Define Executable
add_executable(ip_tracker 
    src/main.cpp
    src/client.cpp
    src/renderer.cpp
    src/utils.cpp
)

target_link_libraries(ip_tracker PRIVATE cpr::cpr nlohmann_json::nlohmann_json)
target_include_directories(ip_tracker PRIVATE include)
