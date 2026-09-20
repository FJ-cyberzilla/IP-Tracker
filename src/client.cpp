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
            data.country = json["country_name"];
            data.city = json["city_name"];
            data.isp = json["isp"];
            data.success = true;
        }
    } else {
        std::cerr << "Error fetching data: " << r.error.message << std::endl;
    }
    return data;
}

}
