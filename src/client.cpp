#include <curl/curl.h>
#include <iostream>
#include <utility>
#include <string>

namespace IPTracker {

// Fallback declaration if header resolution fails
class GeolocationClient {
public:
    explicit GeolocationClient(std::string apiKey = "");
    std::string fetchIpData(const std::string& ip = "");
private:
    std::string apiKey_;
};

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

GeolocationClient::GeolocationClient(std::string apiKey) : apiKey_(std::move(apiKey)) {}

std::string GeolocationClient::fetchIpData(const std::string& ip) {
    CURL* curl = curl_easy_init();
    std::string readBuffer;

    if (curl) {
        std::string url = "https://ipinfo.io/" + (ip.empty() ? "" : ip + "/") + "json";
        if (!apiKey_.empty()) {
            url += "?token=" + apiKey_;
        }

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "IPTracker/1.0");
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        }

        curl_easy_cleanup(curl);
    }
    return readBuffer;
}

} // namespace IPTracker
