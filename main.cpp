#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include "kdtree.h"
#include <string>
#include <Windows.h>
#include <conio.h>

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

KDTree loadListFile()
{
    KDTree tree;

    // Open the CSV file
    std::ifstream inputFile("./simplemaps_worldcities_basicv1/worldcities.csv");
    if (!inputFile.is_open()) {
        std::cerr << "Failed to open the input file." << std::endl;
        return tree;
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

        //std::cout << city_ascii << "(" << latStr << " , " << lngStr << ")" << std::endl;

        // Check if the lat/lng strings are valid before conversion
        if (!latStr.empty() && !lngStr.empty()) {
            try {
                double latitude = std::stod(latStr);
                double longitude = std::stod(lngStr);

                // Insert the city into the KDTree
                tree.insert({ city_ascii, latitude, longitude });

                count++; // Increment counter
            }
            catch (const std::invalid_argument& e) {
                std::cerr << "Error: Invalid number format for city '" << city_ascii
                    << "' with lat '" << latStr << "' and lng '" << lngStr << "'." << std::endl;
            }
        }
        else {
            std::cerr << "Error: Missing lat or lng for city '" << city_ascii << "'." << std::endl;
        }
    }

    //std::cout << "Total cities inserted: " << count << std::endl;

    // Close the file
    inputFile.close();
    return tree;
}

void insertCity(KDTree& tree)
{
    std::cout << "Input city name: ";
    std::string city_ascii;
    std::cin.ignore();
    getline(std::cin, city_ascii);
    std::cout << "Input latitude, longitude: ";
    double latitude, longitude;
    std::cin >> latitude >> longitude;
    tree.insert({ city_ascii, latitude, longitude });
}

void insertMultiFromFile(KDTree& tree)
{
    std::cout << "Input file name: ";
    std::string fileName;
    //std::cin.ignore();
    getline(std::cin, fileName);
    std::ifstream inputFile(fileName);
    if (!inputFile.is_open()) {
        std::cerr << "Failed to open the input file." << std::endl;
        return;
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

        //std::cout << city_ascii << "(" << latStr << " , " << lngStr << ")" << std::endl;

        // Check if the lat/lng strings are valid before conversion
        if (!latStr.empty() && !lngStr.empty()) {
            try {
                double latitude = std::stod(latStr);
                double longitude = std::stod(lngStr);

                // Insert the city into the KDTree
                tree.insert({ city_ascii, latitude, longitude });

                count++; // Increment counter
            }
            catch (const std::invalid_argument& e) {
                std::cerr << "Error: Invalid number format for city '" << city_ascii
                    << "' with lat '" << latStr << "' and lng '" << lngStr << "'." << std::endl;
            }
        }
        else {
            std::cerr << "Error: Missing lat or lng for city '" << city_ascii << "'." << std::endl;
        }
    }

    //std::cout << "Total cities inserted: " << count << std::endl;

    // Close the file
    inputFile.close();
}

void findNearest(KDTree tree)
{
    std::cout << "Input latitude, longitude: ";
    double latitude, longitude;
    std::cin >> latitude >> longitude;
    // Example of nearest neighbor search
    City target = { "Nearest_city", latitude, longitude }; // Target city with arbitrary coordinates
    City nearestCity = tree.nearestNeighbor(target);
    std::cout << "Nearest city to target:";
    std::cout << nearestCity.name << " (" << nearestCity.latitude << ", " << nearestCity.longitude << ")" << std::endl;
}

void queryCity(KDTree tree)
{
    std::cout << "Input min_latitude, min_longitude, max_latitude, max_longitude: ";
    double min_latitude, min_longitude, max_latitude, max_longitude;
    std::cin >> min_latitude >> min_longitude >> max_latitude >> max_longitude;
    std::vector<City> citiesInRange = tree.rangeSearch(min_latitude, min_longitude, max_latitude, max_longitude);
    std::cout << "Cities in range:" << std::endl;
    for (const auto& city : citiesInRange) 
    {
        std::cout << city.name << " (" << city.latitude << ", " << city.longitude << ")" << std::endl;
    }
}

void printTer(Node* pRoot)
{
    if (pRoot == nullptr)
    {
        return;
    }
    printTer(pRoot->left);
    std::cout << pRoot->city.name << "(" << pRoot->city.latitude << " , " << pRoot->city.longitude << ")" << std::endl;
    printTer(pRoot->right);
}

void printFile(KDTree tree)
{

}

void readFile(KDTree tree)
{

}

void printChoice(int choose)
{
	HANDLE color = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(color, 0x06);
    switch (choose)
    {
    case 1:
    {
        SetConsoleTextAttribute(color, 0x70);
        std::cout << ">> Load CSV file" << std::endl;
        SetConsoleTextAttribute(color, 0x06);
        std::cout << "Insert new city" << std::endl;
        std::cout << "Insert multiple new cities by CSV file" << std::endl;
        std::cout << "Nearest neighbor searching" << std::endl;
        std::cout << "Query cities in boundaries" << std::endl;
        std::cout << "Print all list" << std::endl;
        std::cout << "Write to special file" << std::endl;
        std::cout << "Read from special file" << std::endl;
        std::cout << "Exit program" << std::endl;
        break;
    }
    case 2:
    {
        std::cout << "Load CSV file" << std::endl;
        SetConsoleTextAttribute(color, 0x70);
        std::cout << ">> Insert new city" << std::endl;
        SetConsoleTextAttribute(color, 0x06);
        std::cout << "Insert multiple new cities by CSV file" << std::endl;
        std::cout << "Nearest neighbor searching" << std::endl;
        std::cout << "Query cities in boundaries" << std::endl;
        std::cout << "Print all list" << std::endl;
        std::cout << "Write to file" << std::endl;
        std::cout << "Read from special file" << std::endl;
        std::cout << "Exit program" << std::endl;
        break;
    }
    case 3:
    {
        std::cout << "Load CSV file" << std::endl;
        std::cout << "Insert new city" << std::endl;
        SetConsoleTextAttribute(color, 0x70);
        std::cout << ">> Insert multiple new cities by CSV file" << std::endl;
        SetConsoleTextAttribute(color, 0x06);
        std::cout << "Nearest neighbor searching" << std::endl;
        std::cout << "Query cities in boundaries" << std::endl;
        std::cout << "Print all list" << std::endl;
        std::cout << "Write to file" << std::endl;
        std::cout << "Read from special file" << std::endl;
        std::cout << "Exit program" << std::endl;
        break;
    }
    case 4:
    {
        std::cout << "Load CSV file" << std::endl;
        std::cout << "Insert new city" << std::endl;
        std::cout << "Insert multiple new cities by CSV file" << std::endl;
        SetConsoleTextAttribute(color, 0x70);
        std::cout << ">> Nearest neighbor searching" << std::endl;
        SetConsoleTextAttribute(color, 0x06);
        std::cout << "Query cities in boundaries" << std::endl;
        std::cout << "Print all list" << std::endl;
        std::cout << "Write to file" << std::endl;
        std::cout << "Read from special file" << std::endl;
        std::cout << "Exit program" << std::endl;
        break;
    }
    case 5:
    {
        std::cout << "Load CSV file" << std::endl;
        std::cout << "Insert new city" << std::endl;
        std::cout << "Insert multiple new cities by CSV file" << std::endl;
        std::cout << "Nearest neighbor searching" << std::endl;
        SetConsoleTextAttribute(color, 0x70);
        std::cout << ">> Query cities in boundaries" << std::endl;
        SetConsoleTextAttribute(color, 0x06);
        std::cout << "Print all list" << std::endl;
        std::cout << "Write to file" << std::endl;
        std::cout << "Read from special file" << std::endl;
        std::cout << "Exit program" << std::endl;
        break;
    }
    case 6:
    {
        std::cout << "Load CSV file" << std::endl;
        std::cout << "Insert new city" << std::endl;
        std::cout << "Insert multiple new cities by CSV file" << std::endl;
        std::cout << "Nearest neighbor searching" << std::endl;
        std::cout << "Query cities in boundaries" << std::endl;
        SetConsoleTextAttribute(color, 0x70);
        std::cout << ">> Print all list" << std::endl;
        SetConsoleTextAttribute(color, 0x06);
        std::cout << "Write to file" << std::endl;
        std::cout << "Read from special file" << std::endl;
        std::cout << "Exit program" << std::endl;
        break;
    }
    case 7:
    {
        std::cout << "Load CSV file" << std::endl;
        std::cout << "Insert new city" << std::endl;
        std::cout << "Insert multiple new cities by CSV file" << std::endl;
        std::cout << "Nearest neighbor searching" << std::endl;
        std::cout << "Query cities in boundaries" << std::endl;
        std::cout << "Print all list" << std::endl;
        SetConsoleTextAttribute(color, 0x70);
        std::cout << ">> Write to file" << std::endl;
        SetConsoleTextAttribute(color, 0x06);
        std::cout << "Read from special file" << std::endl;
        std::cout << "Exit program" << std::endl;
        break;
    }
    case 8:
    {
        std::cout << "Load CSV file" << std::endl;
        std::cout << "Insert new city" << std::endl;
        std::cout << "Insert multiple new cities by CSV file" << std::endl;
        std::cout << "Nearest neighbor searching" << std::endl;
        std::cout << "Query cities in boundaries" << std::endl;
        std::cout << "Print all list" << std::endl;
        std::cout << "Write to file" << std::endl;
        SetConsoleTextAttribute(color, 0x70);
        std::cout << ">> Read from special file" << std::endl;
        SetConsoleTextAttribute(color, 0x06);
        std::cout << "Exit program" << std::endl;
        break;
    }
    case 9:
    {
        std::cout << "Load CSV file" << std::endl;
        std::cout << "Insert new city" << std::endl;
        std::cout << "Insert multiple new cities by CSV file" << std::endl;
        std::cout << "Nearest neighbor searching" << std::endl;
        std::cout << "Query cities in boundaries" << std::endl;
        std::cout << "Print all list" << std::endl;
        std::cout << "Write to file" << std::endl;
        std::cout << "Read from special file" << std::endl;
        SetConsoleTextAttribute(color, 0x70);
        std::cout << ">> Exit program" << std::endl;
        SetConsoleTextAttribute(color, 0x06);
        break;
    }
    default:
    {
        break;
    }
    }
    SetConsoleTextAttribute(color, 0x07);
}

void menu()
{
    int choose = 1;
    KDTree tree;
    do
    {
        char cursorCommand = NULL;
        do
        {
            system("cls");
            HANDLE color = GetStdHandle(STD_OUTPUT_HANDLE);
            SetConsoleTextAttribute(color, 0x03);
            std::cout << "WELCOME TO KD-Tree !!!!!" << std::endl;
            SetConsoleTextAttribute(color, 0x07);
            printChoice(choose); // print menu
            cursorCommand = _getch();
            if ((cursorCommand == 'w' || cursorCommand == 72) && choose > 1) // move up
            {
                choose--;
            }
            if ((cursorCommand == 's' || cursorCommand == 80) && choose < 9) // move down
            {
                choose++;
            }
        } while (cursorCommand != 13);
        std::cout << std::endl << std::endl;
        if (choose == 9)
        {
            return;
        }
        std::cout << "Result" << std::endl;
        std::cout << "____________________________________" << std::endl;
        switch (choose)
        {
        case 1:
        {
            tree = loadListFile();
            std::cout << "Load file success !" << std::endl;
            system("pause");
            break;
        }
        case 2:
        {
            insertCity(tree);
            system("pause");
            break;
        }
        case 3:
        {
            insertMultiFromFile(tree);
            system("pause");
            break;
        }
        case 4:
        {
            findNearest(tree);
            system("pause");
            break;
        }
        case 5:
        {
            queryCity(tree);
            system("pause");
            break;
        }
        case 6:
        {
            printTer(tree.getRoot());
            system("pause");
            break;
        }
        case 7:
        {
            printFile(tree);
            system("pause");
            break;
        }
        case 8:
        {
            readFile(tree);
            system("pause");
            break;
        }
        default:
            break;
        }
    } while (choose != 9);
    return;
}

int main() 
{
    menu();
    return 0;
}
