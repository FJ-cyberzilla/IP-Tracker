#include "utils.h"
#include <sstream>
#include <iomanip>

namespace IPTracker {
    std::string safe_get_string(const nlohmann::json& j, const std::string& key) {
        if (j.contains(key) && !j.at(key).is_null()) {
            if (j.at(key).is_string()) {
                return j.at(key).get<std::string>();
            } else if (j.at(key).is_number()) {
                std::stringstream ss;
                ss << std::fixed << std::setprecision(6) << j.at(key).get<double>();
                return ss.str();
            }
        }
        return "N/A";
    }
}
