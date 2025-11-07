#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h>
#include <iomanip> // For formatting output

// External Libraries (Ensure these headers are in your 'include/' directory)
#include "json.hpp" // nlohmann/json
#include "cpr/cpr.h"  // C++ Requests (cpr)

using namespace std;
using json = nlohmann::json;

// --- ANSI COLOR CODES ---
#define COLOR_VINTAGE_GREEN "\033[38;5;10m" // A bright, almost lime green (Menu Text)
#define COLOR_BLUE_SELECTION "\033[1;34m"  // Bold Blue (Selection/Results)
#define COLOR_PROMPT "\033[1;36m"         // Cyan (User Prompts)
#define COLOR_YELLOW_BANNER "\033[1;33m"  // Bold Yellow (The Banner Art)
#define COLOR_RESET "\033[0m"

// --- UTILITY FUNCTIONS ---

// Function to simulate typing out text with a delay
void type_out(const string& text, int delay = 25000) {
    for (char c : text) {
        cout << c << flush;
        usleep(delay);
    }
    cout << endl;
}

// Function to print the vintage banner
void print_banner() {
    cout << COLOR_YELLOW_BANNER; 
    // Your exact ASCII art banner
    cout << R"(
.####.####.####.########................########.########.....###.....######..##....##.########.########..####.####
.####.####..##..##.....##..................##....##.....##...##.##...##....##.##...##..##.......##.....##.####.####
..##...##...##..##.....##..................##....##.....##..##...##..##.......##..##...##.......##.....##..##...##.
...##...##..##..########.....#######.......##....########..##.....##.##.......#####....######...########..##...##..
............##..##.........................##....##...##...#########.##.......##..##...##.......##...##............
............##..##.........................##....##....##..##.....##.##....##.##...##..##.......##....##...........
...........####.##.........................##....##.....##.##.....##..######..##....##.########.##.....##..........

)" << COLOR_RESET << endl;
    cout << COLOR_VINTAGE_GREEN << "  >> ENTERPRISE GEOLOCATION UTILITY <<\n" << COLOR_RESET << endl;
}

/**
 * @brief Safely extracts a string value from a JSON object.
 * @param j The JSON object.
 * @param key The key to look up.
 * @return The string value or "N/A" if not found or null.
 */
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

// Function to display the menu and get user choice
string show_menu() {
    string ip_or_command;

    cout << "\n" << COLOR_VINTAGE_GREEN << "--- Main Menu ---" << COLOR_RESET << endl;

    // Display Menu Options with Blue highlight on the primary action
    cout << COLOR_VINTAGE_GREEN << "1. " << COLOR_BLUE_SELECTION << "➤ LOOKUP IP/DOMAIN" << COLOR_RESET << " (Enter IP/Domain)" << endl;
    cout << COLOR_VINTAGE_GREEN << "2. " << COLOR_VINTAGE_GREEN << "  Show Help" << COLOR_RESET << endl;
    cout << COLOR_VINTAGE_GREEN << "3. " << COLOR_VINTAGE_GREEN << "  Exit Program" << COLOR_RESET << endl;
    cout << "-----------------" << endl;

    cout << COLOR_PROMPT << "Enter selection (1-3) or an IP/Domain directly: " << COLOR_RESET;
    
    // Read the user input line
    if (!(cin >> ip_or_command)) return "EXIT"; 

    // Check if the input is a menu number
    if (ip_or_command == "1") {
        cout << COLOR_PROMPT << "Enter IP or domain to lookup: " << COLOR_RESET;
        cin >> ip_or_command;
    } else if (ip_or_command == "2") {
        return "HELP";
    } else if (ip_or_command == "3") {
        return "EXIT";
    }
    
    // Assume any other input is the IP/Domain itself
    return ip_or_command;
}

// Function to handle the HELP command
void show_help() {
    cout << "\n" << COLOR_BLUE_SELECTION << "--- HELP ---" << COLOR_RESET << endl;
    cout << COLOR_VINTAGE_GREEN << "The IP Tracker can resolve IP addresses or domains to geolocation data." << endl;
    cout << "Lookup: Enter an IP address (e.g., 8.8.8.8) or a domain (e.g., google.com)." << endl;
    cout << "Commands: Enter '2' for this help screen, or '3' to exit." << endl;
    cout << "----------------" << endl;
}

/**
 * @brief Makes an HTTP GET request to the IP geolocation API and handles the response.
 * @param ip The IP address or domain to look up.
 * @return The parsed JSON object, or an empty JSON object on failure.
 */
json fetch_ip_data_cpr(const string& ip) {
    const string API_KEY = "555D340B34F6FC128C474931297F1B94";
    const string URL = "https://api.ip2location.io/";

    cpr::Response r = cpr::Get(cpr::Url{URL},
                               cpr::Parameters{{"key", API_KEY}, {"ip", ip}});

    // HTTP Status Code Check
    if (r.status_code != cpr::status::HTTP_OK) {
        cerr << COLOR_BLUE_SELECTION << "❌ HTTP Request Failed: Status Code " << r.status_code << COLOR_RESET << endl;
        if (r.error) cerr << "   - Network Error: " << r.error.message << endl;
        return json({});
    }

    // Network/cpr Error Check
    if (r.error) {
        cerr << COLOR_BLUE_SELECTION << "❌ Network Error: " << r.error.message << COLOR_RESET << endl;
        return json({});
    }

    // JSON Parsing and API Error Check
    try {
        json data = json::parse(r.text);
        
        if (data.contains("error") && !data.at("error").is_null()) {
            cerr << COLOR_BLUE_SELECTION << "❌ API Error: " << safe_get_string(data, "error") << COLOR_RESET << endl;
            return json({});
        }
        
        return data; // Success
    } catch (const json::parse_error& e) {
        cerr << COLOR_BLUE_SELECTION << "❌ JSON Parsing Error: " << e.what() << COLOR_RESET << endl;
        return json({});
    }
}

// Function to display the data report
void display_report(const json& data) {
    string latitude = safe_get_string(data, "latitude");
    string longitude = safe_get_string(data, "longitude");
    string is_proxy_val = safe_get_string(data, "is_proxy");
    string proxy_status = (is_proxy_val == "1" ? "Yes (Proxy/VPN detected)" : "No");

    // Display Formatted Output using COLOR_BLUE_SELECTION for the results
    cout << "\n" << COLOR_BLUE_SELECTION << "--- IP Geolocation Report ---" << COLOR_RESET << "\n";
    cout << left << setfill(' '); 
    cout << COLOR_VINTAGE_GREEN << setw(15) << "IP Address:"    << COLOR_BLUE_SELECTION << safe_get_string(data, "ip") << COLOR_RESET << endl;
    cout << COLOR_VINTAGE_GREEN << setw(15) << "Country:"       << COLOR_BLUE_SELECTION << safe_get_string(data, "country_name") << COLOR_RESET << endl;
    cout << COLOR_VINTAGE_GREEN << setw(15) << "Region/State:"  << COLOR_BLUE_SELECTION << safe_get_string(data, "region_name") << COLOR_RESET << endl;
    cout << COLOR_VINTAGE_GREEN << setw(15) << "City:"          << COLOR_BLUE_SELECTION << safe_get_string(data, "city_name") << COLOR_RESET << endl;
    cout << COLOR_VINTAGE_GREEN << setw(15) << "ISP/Org:"       << COLOR_BLUE_SELECTION << safe_get_string(data, "isp") << COLOR_RESET << endl;
    cout << COLOR_VINTAGE_GREEN << setw(15) << "Time Zone:"     << COLOR_BLUE_SELECTION << safe_get_string(data, "time_zone") << COLOR_RESET << endl;
    cout << COLOR_VINTAGE_GREEN << setw(15) << "Proxy Status:"  << COLOR_BLUE_SELECTION << proxy_status << COLOR_RESET << endl;
    cout << COLOR_VINTAGE_GREEN << setw(15) << "Coordinates:"   << COLOR_BLUE_SELECTION << latitude << ", " << longitude << COLOR_RESET << endl;
    cout << COLOR_BLUE_SELECTION << "-----------------------------" << COLOR_RESET << "\n";
    
    // Generate the Google Maps link
    cout << COLOR_PROMPT << "🗺️ Google Maps: " << COLOR_BLUE_SELECTION 
         << "http://googleusercontent.com/maps.google.com/" << latitude << "," << longitude 
         << COLOR_RESET << endl;
}

// --- MAIN APPLICATION ENTRY POINT ---
int main(int argc, char* argv[]) {
    // Standard I/O optimization
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    print_banner();

    // Use a loop to handle the menu system
    while (true) {
        string ip_to_check;
        
        // 1. Command-line argument check (for automation/scripting)
        if (argc > 1) {
            ip_to_check = argv[1];
            type_out(COLOR_PROMPT + "🚀 Using command-line argument: " + ip_to_check + COLOR_RESET, 15000);
            argc = 0; // Prevent re-using the argument on subsequent loop iterations
        } else {
            // 2. Interactive Menu Mode
            ip_to_check = show_menu();
        }

        if (ip_to_check == "EXIT") {
            type_out(COLOR_VINTAGE_GREEN + "Shutting down the IP Tracker. Goodbye!" + COLOR_RESET, 25000);
            break;
        } else if (ip_to_check == "HELP") {
            show_help();
            continue;
        }

        // --- Core IP Lookup Execution ---
        if (!ip_to_check.empty()) {
            type_out(COLOR_PROMPT + "🔍 Attempting to fetch data for " + ip_to_check + "..." + COLOR_RESET, 15000);
            
            json data = fetch_ip_data_cpr(ip_to_check);

            if (data.empty()) {
                type_out(COLOR_BLUE_SELECTION + "❌ IP lookup failed. Review error messages above." + COLOR_RESET, 15000);
            } else {
                type_out(COLOR_BLUE_SELECTION + "🎯 Data fetched and parsed successfully!" + COLOR_RESET, 15000);
                display_report(data);
            }
        }
        cout << "\n\n"; // Spacing for the next menu loop
    }

    return 0;
}
