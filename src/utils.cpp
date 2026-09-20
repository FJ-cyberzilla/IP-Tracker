#include "ip_tracker/utils.hpp"

namespace IPTracker {
    std::string safe_get_string(const std::string& input) {
        return input.empty() ? "N/A" : input;
    }
}
