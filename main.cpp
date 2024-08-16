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
    //std::cin.ignore();
    getline(std::cin, city_ascii);
    std::cout << "Input latitude, longitude: ";
    double latitude, longitude;
    std::cin >> latitude >> longitude;
    std::cin.ignore();
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

void findCityByName(Node* pRoot, std::string name)
{
    if (pRoot == nullptr)
    {
        return;
    }
    findCityByName(pRoot->left, name);
    if (pRoot->city.name == name)
    {
        std::cout << pRoot->city.name << "(" << pRoot->city.latitude << " , " << pRoot->city.longitude << ")" << std::endl;
    }
    findCityByName(pRoot->right, name);
}

void findNearest(KDTree tree)
{
    if (tree.getRoot() == nullptr)
    {
        std::cout << "Tree is empty, please insert after searching" << std::endl;
        return;
    }
    std::cout << "Input latitude, longitude: ";
    double latitude, longitude;
    std::cin >> latitude >> longitude;
    std::cin.ignore();
    // Example of nearest neighbor search
    City target = { "Nearest_city", latitude, longitude }; // Target city with arbitrary coordinates
    City nearestCity = tree.nearestNeighbor(target);
    std::cout << "Nearest city to target:";
    std::cout << nearestCity.name << " (" << nearestCity.latitude << ", " << nearestCity.longitude << ")" << std::endl;
}

void queryCity(KDTree tree)
{
    if (tree.getRoot() == nullptr)
    {
        std::cout << "Tree is empty, please insert after searching" << std::endl;
        return;
    }
    std::cout << "Input min_latitude, min_longitude, max_latitude, max_longitude: ";
    double min_latitude, min_longitude, max_latitude, max_longitude;
    std::cin >> min_latitude >> min_longitude >> max_latitude >> max_longitude;
    std::cin.ignore();
    std::vector<City> citiesInRange = tree.rangeSearch(min_latitude, min_longitude, max_latitude, max_longitude);
    if (citiesInRange.size() == 0)
    {
        std::cout << "No cities in range" << std::endl;
        return;
    }
    std::cout << "Cities in range:" << std::endl;
    for (const auto& city : citiesInRange) 
    {
        std::cout << city.name << " (" << city.latitude << ", " << city.longitude << ")" << std::endl;
    }
}

void printTer(Node* pRoot, int& n)
{
    if (pRoot == nullptr)
    {
        return;
    }
    printTer(pRoot->left, n);
    std::cout << ++n << "." << pRoot->city.name << "(" << pRoot->city.latitude << " , " << pRoot->city.longitude << ")" << std::endl;
    printTer(pRoot->right, n);
}

void pre_order(Node* pRoot, std::fstream& out)
{
    if (pRoot == nullptr)
    {
        return;
    }

    std::string name = pRoot->city.name;

    // Apply XOR to the string to obfuscate it
    for (char& c : name)
    {
        c ^= 0xAA;  // XOR each character
    }

    // Write the length of the name
    size_t nameLength = name.size();
    out.write(reinterpret_cast<char*>(&nameLength), sizeof(nameLength));

    // Write the XOR-ed name
    out.write(name.c_str(), nameLength);

    // Write latitude and longitude
    out.write(reinterpret_cast<char*>(&pRoot->city.latitude), sizeof(pRoot->city.latitude));
    out.write(reinterpret_cast<char*>(&pRoot->city.longitude), sizeof(pRoot->city.longitude));

    // Recurse on the left and right children
    pre_order(pRoot->left, out);
    pre_order(pRoot->right, out);
}

void printFile(KDTree tree)
{
    std::string fileName;
    std::cout << "Input file name: ";
    getline(std::cin, fileName);
    std::fstream out(fileName, std::ios::out | std::ios::binary);
    
    if (!out)
    {
        std::cout << "Can not open file" << std::endl;
        return;
    }
    pre_order(tree.getRoot(), out);
}

void readFile(KDTree& tree)
{
    std::string fileName;
    std::cout << "Input file name: ";
    getline(std::cin, fileName);
    std::fstream in(fileName, std::ios::in | std::ios::binary);
    if (!in)
    {
        std::cout << "Cannot open file" << std::endl;
        return;
    }

    while (in)
    {
        // Read string length first
        size_t nameLength;
        in.read(reinterpret_cast<char*>(&nameLength), sizeof(nameLength));

        // Ensure valid reading before continuing
        if (in.eof() || in.fail()) break;

        // Read the XOR-ed string data
        std::string name(nameLength, '\0');
        in.read(&name[0], nameLength);

        // XOR the string back to its original form
        for (char& c : name)
        {
            c ^= 0xAA;
        }

        // Read latitude and longitude
        double lat, lon;
        in.read(reinterpret_cast<char*>(&lat), sizeof(lat));
        in.read(reinterpret_cast<char*>(&lon), sizeof(lon));

        // Ensure valid reading before inserting into the tree
        if (in.eof() || in.fail()) break;

        // Insert the data into the tree
        tree.insert({ name, lat, lon });  // Assuming tree.insert takes a struct or similar
    }

    in.close();
    std::cout << "File read successfully and tree reconstructed." << std::endl;
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
        std::cout << "Searching by name " << std::endl;
        std::cout << "Query cities in boundaries" << std::endl;
        std::cout << "Print all list" << std::endl;
        std::cout << "Write to special file" << std::endl;
        std::cout << "Read from special file" << std::endl;
        std::cout << "Remove all tree" << std::endl;
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
        std::cout << "Searching by name " << std::endl;
        std::cout << "Query cities in boundaries" << std::endl;
        std::cout << "Print all list" << std::endl;
        std::cout << "Write to file" << std::endl;
        std::cout << "Read from special file" << std::endl;
        std::cout << "Remove all tree" << std::endl;
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
        std::cout << "Searching by name" << std::endl;
        std::cout << "Query cities in boundaries" << std::endl;
        std::cout << "Print all list" << std::endl;
        std::cout << "Write to special file" << std::endl;
        std::cout << "Read from special file" << std::endl;
        std::cout << "Remove all tree" << std::endl;
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
        std::cout << "Searching by name" << std::endl;
        std::cout << "Query cities in boundaries" << std::endl;
        std::cout << "Print all list" << std::endl;
        std::cout << "Write to special file" << std::endl;
        std::cout << "Read from special file" << std::endl;
        std::cout << "Remove all tree" << std::endl;
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
        std::cout << ">> Searching by name" << std::endl;
        SetConsoleTextAttribute(color, 0x06);
        std::cout << "Query cities in boundaries" << std::endl;
        std::cout << "Print all list" << std::endl;
        std::cout << "Write to special file" << std::endl;
        std::cout << "Read from special file" << std::endl;
        std::cout << "Remove all tree" << std::endl;
        std::cout << "Exit program" << std::endl;
        break;
    }
    case 6:
    {
        std::cout << "Load CSV file" << std::endl;
        std::cout << "Insert new city" << std::endl;
        std::cout << "Insert multiple new cities by CSV file" << std::endl;
        std::cout << "Nearest neighbor searching" << std::endl;
        std::cout << "Searching by name" << std::endl;
        SetConsoleTextAttribute(color, 0x70);
        std::cout << ">> Query cities in boundaries" << std::endl;
        SetConsoleTextAttribute(color, 0x06);
        std::cout << "Print all list" << std::endl;
        std::cout << "Write to special file" << std::endl;
        std::cout << "Read from special file" << std::endl;
        std::cout << "Remove all tree" << std::endl;
        std::cout << "Exit program" << std::endl;
        break;
    }
    case 7:
    {
        std::cout << "Load CSV file" << std::endl;
        std::cout << "Insert new city" << std::endl;
        std::cout << "Insert multiple new cities by CSV file" << std::endl;
        std::cout << "Nearest neighbor searching" << std::endl;
        std::cout << "Searching by name" << std::endl;
        std::cout << "Query cities in boundaries" << std::endl;
        SetConsoleTextAttribute(color, 0x70);
        std::cout << ">> Print all list" << std::endl;
        SetConsoleTextAttribute(color, 0x06);
        std::cout << "Write to special file" << std::endl;
        std::cout << "Read from special file" << std::endl;
        std::cout << "Remove all tree" << std::endl;
        std::cout << "Exit program" << std::endl;
        break;
    }
    case 8:
    {
        std::cout << "Load CSV file" << std::endl;
        std::cout << "Insert new city" << std::endl;
        std::cout << "Insert multiple new cities by CSV file" << std::endl;
        std::cout << "Nearest neighbor searching" << std::endl;
        std::cout << "Searching by name" << std::endl;
        std::cout << "Query cities in boundaries" << std::endl;
        std::cout << "Print all list" << std::endl;
        SetConsoleTextAttribute(color, 0x70);
        std::cout << ">> Write to special file" << std::endl;
        SetConsoleTextAttribute(color, 0x06);
        std::cout << "Read from special file" << std::endl;
        std::cout << "Remove all tree" << std::endl;
        std::cout << "Exit program" << std::endl;
        break;
    }
    case 9:
    {
        std::cout << "Load CSV file" << std::endl;
        std::cout << "Insert new city" << std::endl;
        std::cout << "Insert multiple new cities by CSV file" << std::endl;
        std::cout << "Nearest neighbor searching" << std::endl;
        std::cout << "Searching by name" << std::endl;
        std::cout << "Query cities in boundaries" << std::endl;
        std::cout << "Print all list" << std::endl;
        std::cout << "Write to special file" << std::endl;
        SetConsoleTextAttribute(color, 0x70);
        std::cout << ">> Read from special file" << std::endl;
        SetConsoleTextAttribute(color, 0x06);
        std::cout << "Remove all tree" << std::endl;
        std::cout << "Exit program" << std::endl;
        break;
    }
    case 10:
    {
        std::cout << "Load CSV file" << std::endl;
        std::cout << "Insert new city" << std::endl;
        std::cout << "Insert multiple new cities by CSV file" << std::endl;
        std::cout << "Nearest neighbor searching" << std::endl;
        std::cout << "Searching by name" << std::endl;
        std::cout << "Query cities in boundaries" << std::endl;
        std::cout << "Print all list" << std::endl;
        std::cout << "Write to special file" << std::endl;
        std::cout << "Read from special file" << std::endl;
        SetConsoleTextAttribute(color, 0x70);
        std::cout << ">> Remove all tree" << std::endl;
        SetConsoleTextAttribute(color, 0x06);
        std::cout << "Exit program" << std::endl;
        break;
    }
    case 11:
    {
        std::cout << "Load CSV file" << std::endl;
        std::cout << "Insert new city" << std::endl;
        std::cout << "Insert multiple new cities by CSV file" << std::endl;
        std::cout << "Nearest neighbor searching" << std::endl;
        std::cout << "Searching by name" << std::endl;
        std::cout << "Query cities in boundaries" << std::endl;
        std::cout << "Print all list" << std::endl;
        std::cout << "Write to special file" << std::endl;
        std::cout << "Read from special file" << std::endl;
        std::cout << "Remove all tree" << std::endl;
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

void removeTree(Node*& pRoot)
{
    if (pRoot == nullptr)
    {
        return;
    }
    removeTree(pRoot->left);
    removeTree(pRoot->right);
    delete pRoot;
    pRoot = nullptr;
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
            if ((cursorCommand == 'w' || cursorCommand == 72)) // move up
            {
                choose--;
            }
            if ((cursorCommand == 's' || cursorCommand == 80)) // move down
            {
                choose++;
            }
            if (choose == 12)
            {
                choose = 1;
            }
            if (choose == 0)
            {
                choose = 11;
            }
        } while (cursorCommand != 13);
        std::cout << std::endl << std::endl;
        if (choose == 11)
        {
            return;
        }
        std::cout << "Result" << std::endl;
        std::cout << "------------------------------------" << std::endl;
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
            if (tree.getRoot() == nullptr)
            {
                std::cout << "Tree is empty, please insert after searching" << std::endl;
                system("pause");
                break;
            }
            std::string name;
            std::cout << "Input city name: ";
            getline(std::cin, name);
            findCityByName(tree.getRoot(), name);
            system("pause");
            break;
        }
        case 6:
        {
            queryCity(tree);
            system("pause");
            break;
        }
        case 7:
        {
            if (tree.getRoot() == nullptr)
            {
                std::cout << "Tree is empty\n";
                system("pause");
                break;
            }
            int n = 0;
            printTer(tree.getRoot(), n);
            system("pause");
            break;
        }
        case 8:
        {
            printFile(tree);
            system("pause");
            break;
        }
        case 9:
        {
            readFile(tree);
            system("pause");
            break;
        }
        case 10:
        {
            removeTree(tree.getRootAddress());
            std::cout << "Remove success !" << std::endl;
            system("pause");
            break;
        }
        default:
            break;
        }
    } while (choose != 11);
    return;
}

int main() 
{
    menu();
    return 0;
}
