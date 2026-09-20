#include "ip_tracker/renderer.hpp"
#include <iostream>
#include <iomanip>

namespace IPTracker {

void ReportRenderer::render(const GeolocationData& data) {
    if (!data.success) {
        std::cout << "Failed to fetch data for IP: " << data.ip << std::endl;
        return;
    }

    std::cout << "\n--- IP Geolocation Report ---\n";
    std::cout << std::left << std::setfill(' ');
    std::cout << std::setw(15) << "IP Address:"    << data.ip << "\n";
    std::cout << std::setw(15) << "Country:"       << data.country << "\n";
    std::cout << std::setw(15) << "Region/State:"  << data.region << "\n";
    std::cout << std::setw(15) << "City:"          << data.city << "\n";
    std::cout << std::setw(15) << "ISP/Org:"       << data.isp << "\n";
    std::cout << std::setw(15) << "Time Zone:"     << data.timezone << "\n";
    std::cout << std::setw(15) << "Proxy Status:"  << (data.is_proxy ? "Yes" : "No") << "\n";
    std::cout << std::setw(15) << "Coordinates:"   << data.latitude << ", " << data.longitude << "\n";
    std::cout << "-----------------------------\n";
}

}
