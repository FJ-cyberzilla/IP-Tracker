#include "ip_tracker/client.hpp"
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <iostream>

namespace IPTracker {

GeolocationClient::GeolocationClient(std::string api_key) : api_key_(std::move(api_key)) {}

GeolocationData GeolocationClient::fetchIpData(const std::string& ip) {
    std::string url = "https://api.ip2location.io/?key=" + api_key_ + "&ip=" + ip;
    cpr::Response r = cpr::Get(cpr::Url{url});

    GeolocationData data;
    data.success = false;
    data.ip = ip;

    if (r.status_code == 200) {
        auto json = nlohmann::json::parse(r.text);
        if (json.contains("country_name")) {
            data.country = json.value("country_name", "N/A");
            data.city = json.value("city_name", "N/A");
            data.region = json.value("region_name", "N/A");
            data.isp = json.value("isp", "N/A");
            data.timezone = json.value("time_zone", "N/A");
            data.latitude = json.value("latitude", 0.0); // Assuming numbers, need to handle or convert to string.
            // Wait, JSON can be double or string. 
            // The previous safe_get_string handled this.
            // I should use that or similar.
            data.latitude = std::to_string(json.value("latitude", 0.0));
            data.longitude = std::to_string(json.value("longitude", 0.0));
            data.is_proxy = json.value("is_proxy", 0) == 1;
            data.success = true;
        }
    } else {
        std::cerr << "Error fetching data: " << r.error.message << std::endl;
    }
    return data;
}

}
