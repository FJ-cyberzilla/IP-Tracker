#pragma once
#include <string>
#include "json/json.hpp" //  json.hpp must be  included

namespace IPTracker {
    // Function declaration
    std::string safe_get_string(const nlohmann::json& j, const std::string& key);
}
