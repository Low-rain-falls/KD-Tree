#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

void extractColumns(const std::string& inputFilename, const std::string& outputFilename) {
    std::ifstream inputFile(inputFilename);
    std::ofstream outputFile(outputFilename);

    if (!inputFile.is_open()) {
        std::cerr << "Error: Could not open the input file." << std::endl;
        return;
    }

    if (!outputFile.is_open()) {
        std::cerr << "Error: Could not open the output file." << std::endl;
        return;
    }

    std::string line;
    
    // Read the header line from the input file
    std::getline(inputFile, line);

    // Write the header for the output file
    outputFile << "city_ascii,lat,lng\n";

    // Process each line of the input file
    while (std::getline(inputFile, line)) {
        std::stringstream ss(line);
        std::string city, city_ascii, lat, lng, extra;

        // Assuming the CSV format is: city, city_ascii, lat, lng, ...
        std::getline(ss, city, ',');        // Read the city name
        std::getline(ss, city_ascii, ',');  // Read the city_ascii
        std::getline(ss, lat, ',');         // Read the latitude
        std::getline(ss, lng, ',');         // Read the longitude

        // Skip other columns if present
        std::getline(ss, extra);

        // Write the selected columns to the output file
        if (!city_ascii.empty() && !lat.empty() && !lng.empty()) {
            outputFile << city_ascii << "," << lat << "," << lng << "\n";
        }
    }

    // Close the files
    inputFile.close();
    outputFile.close();

    std::cout << "Filtered data has been saved to " << outputFilename << std::endl;
}

int main() {
    std::string inputFilename = "./worldcities.csv";  // Replace with your input file name
    std::string outputFilename = "output.csv";

    extractColumns(inputFilename, outputFilename);

    return 0;
}
