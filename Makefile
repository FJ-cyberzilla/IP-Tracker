
# --- Compiler & Flags ---
CXX      := c++
CXXFLAGS := -std=c++17 -O2 -Wall -Wextra -Iinclude
LDFLAGS  := -lcurl

# --- Detect OS / Termux Environment ---
PREFIX ?= /usr/local
ifneq ($(wildcard /data/data/com.termux),)
    # Termux specific settings
    PREFIX := $(TERMUX_APP_PACKAGE_MANAGER_PREFIX:-/data/data/com.termux/files/usr)
endif

# --- Project Structure ---
TARGET   := ip_tracker
SRC_DIR  := src
BUILD_DIR:= build

# Find all C++ source files inside src/
SRCS     := $(wildcard $(SRC_DIR)/*.cpp)
OBJS     := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRCS))

# --- Default Target ---
all: setup_deps $(TARGET)

# --- Automatic Dependency Setup Check ---
setup_deps:
        @if [ -d "/data/data/com.termux" ]; then \
                if ! command -v curl > /dev/null || ! pkg list-installed nlohmann-json > /dev/null 2>&1; then \
                        echo "--> Termux detected: Installing missing dependencies..."; \
                        pkg install -y libcurl nlohmann-json; \
                fi; \
        fi

# --- Linking the Executable ---
$(TARGET): $(OBJS)
        @echo "--> Linking executable: $(TARGET)"
        $(CXX) $(OBJS) -o $@ $(LDFLAGS)
        @echo "✅ Build complete! Run with: ./$(TARGET)"

# --- Compiling Source Files ---
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
        @echo "--> Compiling $<"
        $(CXX) $(CXXFLAGS) -c $< -o $@

# Create build directory if it doesn't exist
$(BUILD_DIR):
        @mkdir -p $(BUILD_DIR)

# --- Install Target ---
install: $(TARGET)
        @echo "--> Installing $(TARGET) to $(PREFIX)/bin"
        @mkdir -p $(PREFIX)/bin
        @cp -f $(TARGET) $(PREFIX)/bin/
        @chmod 755 $(PREFIX)/bin/$(TARGET)
        @echo "✅ Installed! You can now run '$(TARGET)' from anywhere."

# --- Clean Target ---
clean:
        @echo "--> Cleaning build artifacts..."
        @rm -rf $(BUILD_DIR) $(TARGET)
        @echo "✅ Clean finished."

.PHONY: all setup_deps install clean
