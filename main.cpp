#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include "kdtree.h"
#include <string>
#include <Windows.h>
#include <conio.h>

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

    
    std::ifstream inputFile("./simplemaps_worldcities_basicv1/worldcities.csv");
    if (!inputFile.is_open()) {
        std::cerr << "Failed to open the input file." << std::endl;
        return tree;
    }

    std::string line;

    
    std::getline(inputFile, line);

    int count = 0; 

    
    while (std::getline(inputFile, line)) {
        std::stringstream ss(line);
        std::string city, city_ascii, latStr, lngStr;

        std::getline(ss, city, ',');        
        std::getline(ss, city_ascii, ',');  
        std::getline(ss, latStr, ',');      
        std::getline(ss, lngStr, ',');      

        city_ascii = trim(city_ascii);
        latStr = trim(latStr);
        lngStr = trim(lngStr);
        
        if (!latStr.empty() && !lngStr.empty()) {
            try {
                double latitude = std::stod(latStr);
                double longitude = std::stod(lngStr);
                tree.insert({ city_ascii, latitude, longitude });
                count++; 
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

    inputFile.close();
    return tree;
}

void insertCity(KDTree& tree)
{
    std::cout << "Input city name: ";
    std::string city_ascii;
    
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
    
    getline(std::cin, fileName);
    std::ifstream inputFile(fileName);
    if (!inputFile.is_open()) {
        std::cerr << "Failed to open the input file." << std::endl;
        return;
    }

    std::string line;
    std::getline(inputFile, line);
    int count = 0; 
    
    while (std::getline(inputFile, line)) {
        std::stringstream ss(line);
        std::string city, city_ascii, latStr, lngStr;
        std::getline(ss, city, ',');        
        std::getline(ss, city_ascii, ',');  
        std::getline(ss, latStr, ',');      
        std::getline(ss, lngStr, ',');      
        city_ascii = trim(city_ascii);
        latStr = trim(latStr);
        lngStr = trim(lngStr);
        
        if (!latStr.empty() && !lngStr.empty()) {
            try {
                double latitude = std::stod(latStr);
                double longitude = std::stod(lngStr);
                tree.insert({ city_ascii, latitude, longitude });
                count++; 
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
        std::cout << "Tree is empty, please insert before searching" << std::endl;
        return;
    }
    std::cout << "Input latitude, longitude: ";
    double latitude, longitude;
    std::cin >> latitude >> longitude;
    std::cin.ignore();
    
    City target = { "Nearest_city", latitude, longitude }; 
    City nearestCity = tree.nearestNeighbor(target);
    std::cout << "Nearest city to target: ";
    std::cout << nearestCity.name << " (" << nearestCity.latitude << ", " << nearestCity.longitude << ")" << std::endl;
}

void queryCity(KDTree tree)
{
    if (tree.getRoot() == nullptr)
    {
        std::cout << "Tree is empty, please insert before searching" << std::endl;
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
    for (char& c : name)
    {
        c ^= 0xAA;  
    }

    size_t nameLength = name.size();
    out.write(reinterpret_cast<char*>(&nameLength), sizeof(nameLength));
    out.write(name.c_str(), nameLength);
    out.write(reinterpret_cast<char*>(&pRoot->city.latitude), sizeof(pRoot->city.latitude));
    out.write(reinterpret_cast<char*>(&pRoot->city.longitude), sizeof(pRoot->city.longitude));
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
        size_t nameLength;
        in.read(reinterpret_cast<char*>(&nameLength), sizeof(nameLength));

        if (in.eof() || in.fail()) break;

        std::string name(nameLength, '\0');
        in.read(&name[0], nameLength);

        for (char& c : name)
        {
            c ^= 0xAA;
        }
        double lat, lon;
        in.read(reinterpret_cast<char*>(&lat), sizeof(lat));
        in.read(reinterpret_cast<char*>(&lon), sizeof(lon));

        if (in.eof() || in.fail()) break;

        tree.insert({ name, lat, lon });  
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
        std::cout << "Write to special file" << std::endl;
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
            printChoice(choose); 
            cursorCommand = _getch();
            if ((cursorCommand == 'w' || cursorCommand == 72)) 
            {
                choose--;
            }
            if ((cursorCommand == 's' || cursorCommand == 80)) 
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
                std::cout << "Tree is empty, please insert before searching" << std::endl;
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
    removeTree(tree.getRootAddress());
    return;
}

int main() 
{
    menu();
    return 0;
}
