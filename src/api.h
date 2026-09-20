#ifndef API_H
#define API_H

#include <string>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

std::string safe_get_string(const json& j, const std::string& key);
json fetch_ip_data_cpr(const std::string& ip, const std::string& api_key);

#endif // API_H
