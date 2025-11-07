#include "gtest/gtest.h"
#include "json/json.hpp"
#include <string>
#include <sstream>
#include <iomanip>

// Re-declare namespace and type for convenience
using namespace std;
using json = nlohmann::json;

// --- UTILITY FUNCTIONS COPIED FOR TESTING ---
// In a real project, this function would be in a shared library file (e.g., utils.cpp)
// and its header (utils.h) would be included here.

string safe_get_string(const json& j, const string& key) {
    if (j.contains(key) && !j.at(key).is_null()) {
        if (j.at(key).is_string()) {
            return j.at(key).get<string>();
        } else if (j.at(key).is_number()) {
            stringstream ss;
            ss << fixed << setprecision(6) << j.at(key).get<double>();
            return ss.str();
        }
    }
    return "N/A";
}
// --- END UTILITY FUNCTIONS ---


// --- TEST FIXTURES AND CASES ---

// 1. Test the safe_get_string utility function
TEST(UtilityTests, SafeGetStringHandlesAllTypes) {
    // Create a JSON object with various data types and nulls
    json test_data = json::parse(R"(
        {
            "ip": "8.8.8.8",
            "latitude": 37.76,
            "city_name": "Mountain View",
            "missing_key": null,
            "empty_string": ""
        }
    )");

    // Test case 1: Existing String
    EXPECT_EQ(safe_get_string(test_data, "ip"), "8.8.8.8");

    // Test case 2: Existing Number (must convert to string with precision)
    EXPECT_EQ(safe_get_string(test_data, "latitude"), "37.760000"); // 6 digits of precision

    // Test case 3: Missing Key (Should return "N/A")
    EXPECT_EQ(safe_get_string(test_data, "country"), "N/A");

    // Test case 4: Key with Null value (Should return "N/A")
    EXPECT_EQ(safe_get_string(test_data, "missing_key"), "N/A");
    
    // Test case 5: Key with Empty string (Should return the empty string)
    EXPECT_EQ(safe_get_string(test_data, "empty_string"), "");
}

// 2. Test the display logic and proxy handling (MOCK DATA)
TEST(DisplayTests, ProxyStatusIsFormattedCorrectly) {
    // Mock the API response for a proxy
    json proxy_data = json::parse(R"({"is_proxy": 1})");
    // Mock the API response for a non-proxy
    json non_proxy_data = json::parse(R"({"is_proxy": 0})");
    
    // We cannot fully test the display function (it uses cout), 
    // but we can test the critical logic that drives the display.

    // Test proxy status extraction
    EXPECT_EQ(safe_get_string(proxy_data, "is_proxy"), "1");
    EXPECT_EQ(safe_get_string(non_proxy_data, "is_proxy"), "0");
}
