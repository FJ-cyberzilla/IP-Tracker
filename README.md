# IPTracker

A lightweight, high-performance C++ command-line application that fetches and displays detailed IP geolocation data. Built using standard **libcurl** and **nlohmann/json**, it compiles natively on Linux, macOS, and Android (Termux) without heavy build tools or CMake overhead.

---

## Features

- **Local & Remote IP Tracking:** Fetch details for your current public IP or any target IPv4/IPv6 address.
- **Fast & Dependency-Free:** Uses a custom `Makefile` linking directly against native system `libcurl`.
- **Termux & Linux Native:** Runs smoothly in Termux on Android without symbol versioning or linker issues.
- **Structured JSON Parsing:** Safely extracts IP, ISP, region, city, and geographical coordinates.

---

## Project Architecture

```text
├── Makefile                # Universal cross-platform build script
├── configure               # Environment configuration helper
├── include/
│   └── ip_tracker/         # Core C++ header definitions
│       ├── client.hpp
│       ├── model.hpp
│       ├── renderer.hpp
│       └── utils.hpp
├── src/                    # Source code implementations
│   ├── client.cpp
│   ├── main.cpp
│   ├── renderer.cpp
│   └── utils.cpp
└── tests/
    └── unit_tests.cpp      # Test suite

Quick Start & Installation
​1. Install Prerequisites
​On Termux (Android)
pkg update
pkg install make clang libcurl nlohmann-json

On Debian / Ubuntu
sudo apt update
sudo apt install build-essential libcurl4-openssl-dev nlohmann-json3-dev

On Arch Linux
sudo pacman -S base-devel curl nlohmann-json

|2. Build the Application
​Clone the repository and run make:

3. Usage
​Run the executable directly after building:
# Track your current public IP
./ip_tracker

# Track a specific target IP
./ip_tracker 8.8.8.8

Build Targets
​The included Makefile provides several standard targets:
CommandAction
makeCompiles source files and links ./ip_tracker
make installInstalls executable to system binary path (/usr/local/bin or Termux $PREFIX/bin)
make cleanRemoves compiled object files (build/) and the binary
License
​Distributed under the MIT License. See LICENSE for details.
