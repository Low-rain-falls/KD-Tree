#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "kdtree.h"

int main() {
    KDTree tree;

    // Open the CSV file
    std::ifstream file("./simplemaps_worldcities_basicv1/output.csv");
    if (!file.is_open()) {
        std::cerr << "Failed to open the file." << std::endl;
        return 1;
    }

    std::string line;
    // Skip the header line
    std::getline(file, line);

    // Read each line from the CSV file
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string city, latStr, lngStr;
        std::getline(ss, city, ',');   // Read the city name
        std::getline(ss, latStr, ','); // Read the latitude
        std::getline(ss, lngStr, ','); // Read the longitude

        // Convert strings to float/double
        double latitude = std::stod(latStr);
        double longitude = std::stod(lngStr);

        // Insert the city into the KDTree
        tree.insert({city, latitude, longitude});
    }

    // Close the file
    file.close();

    // Example of range search
    std::vector<City> citiesInRange = tree.rangeSearch(20.0, 70.0, 40.0, 140.0);
    std::cout << "Cities in range:" << std::endl;
    for (const auto& city : citiesInRange) {
        std::cout << city.name << " (" << city.latitude << ", " << city.longitude << ")" << std::endl;
    }

    // Example of nearest neighbor search
    City target = {"Example", 30.0, 120.0}; // Target city with arbitrary coordinates
    City nearestCity = tree.nearestNeighbor(target);
    std::cout << "Nearest city to target:" << std::endl;
    std::cout << nearestCity.name << " (" << nearestCity.latitude << ", " << nearestCity.longitude << ")" << std::endl;

    return 0;
}
