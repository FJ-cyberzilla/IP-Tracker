#!/bin/bash
# install.sh
# Installation script for the IPTracker C++ application on Debian/Ubuntu systems.

# --- Configuration ---
PROJECT_NAME="IPTracker"
BUILD_DIR="build"
INSTALL_DIR="install_package"

# --- Colors for Terminal Output ---
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
RED='\033[0;31m'
NC='\033[0m' # No Color

echo -e "${YELLOW}--- Starting $PROJECT_NAME Setup ---${NC}"

# --- 1. Install System Dependencies ---
echo -e "${GREEN}1. Installing System Dependencies...${NC}"

# Packages: build-essential (g++, make), cmake, libcurl (for cpr), and gtest (for tests)
# Note: nlohmann/json is header-only and is included in the project, so it's not installed via apt.
sudo apt-get update
sudo apt-get install -y build-essential cmake libcurl4-openssl-dev libgtest-dev
# Link system-installed gtest for easier use with CMake
# The -p flag ensures parent directories are created if needed
if [ ! -f /usr/lib/x86_64-linux-gnu/libgtest.a ]; then
    echo -e "${YELLOW}Compiling and linking gtest libraries...${NC}"
    # Go to source directory
    cd /usr/src/googletest
    # Use cmake to generate build files for the GTest source
    sudo cmake -DBUILD_GMOCK=OFF .
    # Compile GTest
    sudo make
    # Copy the compiled libraries to the standard lib directory
    sudo cp lib/libgtest*.a /usr/lib/x86_64-linux-gnu/
    # Return to the original directory
    cd - > /dev/null
fi

if [ $? -ne 0 ]; then
    echo -e "${RED}Error: Failed to install system dependencies. Aborting.${NC}"
    exit 1
fi

# Link system-installed gtest for easier use with CMake
if [ ! -f /usr/lib/libgtest.a ]; then
    echo -e "${YELLOW}Linking gtest libraries...${NC}"
    cd /usr/src/googletest
    sudo cmake .
    sudo make
    sudo cp lib/libgtest*.a /usr/lib/
    cd - > /dev/null
fi

# --- 2. Clean and Setup Build Environment ---
echo -e "${GREEN}2. Preparing build directories...${NC}"

# Remove old build artifacts
rm -rf $BUILD_DIR $INSTALL_DIR

# Create new directories
mkdir $BUILD_DIR
mkdir $INSTALL_DIR

# --- 3. Configure and Build ---
echo -e "${GREEN}3. Configuring and building the project...${NC}"

# Run CMake configuration (out-of-source build)
cmake -S . -B $BUILD_DIR

if [ $? -ne 0 ]; then
    echo -e "${RED}Error: CMake configuration failed. Aborting.${NC}"
    exit 1
fi

# Run the build command (using all CPU cores -j$(nproc))
cmake --build $BUILD_DIR --config Release -- -j$(nproc)

if [ $? -ne 0 ]; then
    echo -e "${RED}Error: Project compilation failed. Aborting.${NC}"
    exit 1
fi

# --- 4. Testing (Optional but Recommended) ---
echo -e "${GREEN}4. Running Unit Tests...${NC}"
ctest --test-dir $BUILD_DIR --output-on-failure
# If tests fail, the script continues, but the user is informed.

# --- 5. Install Final Artifacts ---
echo -e "${GREEN}5. Installing final executable to $INSTALL_DIR/...${NC}"

# Install the built executable into the custom install directory
cmake --install $BUILD_DIR --prefix $INSTALL_DIR

echo -e "${YELLOW}-----------------------------------------${NC}"
echo -e "${GREEN}✅ Installation and build complete!${NC}"
echo -e "You can run the application with: ${YELLOW}./$INSTALL_DIR/$PROJECT_NAME${NC}"
echo -e "To run tests manually: ${YELLOW}cd build && ctest --output-on-failure${NC}"
echo -e "${YELLOW}-----------------------------------------${NC}"
