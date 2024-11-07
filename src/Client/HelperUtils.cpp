#include "HelperUtils.h"
#include <iostream>
#include <fstream>
//#include "../third_party/include/nlohmann/json.hpp"

std::string HelperUtils::toLowerCase(std::string& text)
{
	std::transform(text.begin(), text.end(), text.begin(), [](unsigned char c) { return std::tolower(c); });
	return text;
}

/*std::string HelperUtils::readDataFromJson(const std::wstring& configFile, std::string first, std::string second)
{
    std::string Data;

    try {
        // Convert wstring to const char* using wstring::c_str() for the file path
        std::ifstream file(std::string(configFile.begin(), configFile.end()));  // Open file as regular stream
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open the config file.");
        }

        nlohmann::json jsonData;
        file >> jsonData;  // Deserialize JSON data

        // Read Data from JSON
        Data = jsonData[first][second];

        return Data;
    }
    catch (const std::exception& e) {
        std::cerr << "[ERROR] Failed to read COM port from JSON: " << e.what() << std::endl;
    }
    return nullptr;
}*/