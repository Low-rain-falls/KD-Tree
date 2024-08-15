#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include "kdtree.h"

// // Helper function to trim whitespace and quotes from strings
// std::string trim(const std::string& str) {
//     size_t first = str.find_first_not_of(" \t\n\r\f\v\"");
//     size_t last = str.find_last_not_of(" \t\n\r\f\v\"");
//     return (first == std::string::npos || last == std::string::npos) ? "" : str.substr(first, last - first + 1);
// }

std::string trim(std::string str) {
    if (str.length() > 1) {
        str.erase(str.begin());
        str.pop_back();
    } else {
        return "";
    }
    return str;
}

int main() {
    KDTree tree;

    // Open the CSV file
    std::ifstream inputFile("./simplemaps_worldcities_basicv1/worldcities.csv");
    if (!inputFile.is_open()) {
        std::cerr << "Failed to open the input file." << std::endl;
        return 1;
    }

    std::string line;

    // Skip the header line
    std::getline(inputFile, line);

    int count = 0; // Counter for debugging

    // Process each line from the CSV file
    while (std::getline(inputFile, line)) {
        std::stringstream ss(line);
        std::string city, city_ascii, latStr, lngStr;

        // Assuming the CSV format is: city, city_ascii, lat, lng, ...
        std::getline(ss, city, ',');        // Read the city name
        std::getline(ss, city_ascii, ',');  // Read the city_ascii
        std::getline(ss, latStr, ',');      // Read the latitude
        std::getline(ss, lngStr, ',');      // Read the longitude

        // Trim any surrounding quotes or spaces
        city_ascii = trim(city_ascii);
        latStr = trim(latStr);
        lngStr = trim(lngStr);
        
        std::cout << city_ascii << "(" << latStr << " , " << lngStr << ")" << std::endl;

        // Check if the lat/lng strings are valid before conversion
        if (!latStr.empty() && !lngStr.empty()) {
            try {
                double latitude = std::stod(latStr);
                double longitude = std::stod(lngStr);

                // Insert the city into the KDTree
                tree.insert({city_ascii, latitude, longitude});

                count++; // Increment counter
            } catch (const std::invalid_argument& e) {
                std::cerr << "Error: Invalid number format for city '" << city_ascii 
                          << "' with lat '" << latStr << "' and lng '" << lngStr << "'." << std::endl;
            }
        } else {
            std::cerr << "Error: Missing lat or lng for city '" << city_ascii << "'." << std::endl;
        }
    }

    std::cout << "Total cities inserted: " << count << std::endl;

    // Close the file
    inputFile.close();

    // Example of range search
    // std::vector<City> citiesInRange = tree.rangeSearch(20.0, 70.0, 40.0, 140.0);
    // std::cout << "Cities in range:" << std::endl;
    // for (const auto& city : citiesInRange) {
    //     std::cout << city.name << " (" << city.latitude << ", " << city.longitude << ")" << std::endl;
    // }
    //
    // // Example of nearest neighbor search
    // City target = {"Example", 30.0, 120.0}; // Target city with arbitrary coordinates
    // City nearestCity = tree.nearestNeighbor(target);
    // std::cout << "Nearest city to target:" << std::endl;
    // std::cout << nearestCity.name << " (" << nearestCity.latitude << ", " << nearestCity.longitude << ")" << std::endl;
    //
    return 0;
}
