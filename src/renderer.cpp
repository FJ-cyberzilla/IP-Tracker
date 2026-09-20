#include "ip_tracker/renderer.hpp"
#include <iostream>

namespace IPTracker {

void ReportRenderer::render(const GeolocationData& data) {
    if (!data.success) {
        std::cout << "Failed to fetch data for IP: " << data.ip << std::endl;
        return;
    }

    std::cout << "--- IP Report ---" << std::endl;
    std::cout << "IP:      " << data.ip << std::endl;
    std::cout << "Country: " << data.country << std::endl;
    std::cout << "City:    " << data.city << std::endl;
    std::cout << "ISP:     " << data.isp << std::endl;
    std::cout << "-----------------" << std::endl;
}

}
