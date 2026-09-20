#include <iostream>
#include <string>
#include <cstdlib>
#include "ip_tracker/client.hpp"
#include "ip_tracker/renderer.hpp"

int main() {
    const char* api_key_ptr = std::getenv("IP2LOCATION_API_KEY");
    if (!api_key_ptr) {
        std::cerr << "Error: IP2LOCATION_API_KEY environment variable not set." << std::endl;
        return 1;
    }

    std::string api_key(api_key_ptr);
    IPTracker::GeolocationClient client(api_key);
    IPTracker::ReportRenderer renderer;

    std::string ip;
    std::cout << "Enter IP to track: ";
    std::cin >> ip;

    IPTracker::GeolocationData data = client.fetchIpData(ip);
    renderer.render(data);

    return 0;
}
