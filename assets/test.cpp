#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
using namespace std;

struct City {
    string name;
    double latitude;
    double longitude;
};

string trim (string str){
    if (str.length() > 1) {
        str.erase(str.begin());   // Erase the first character
        str.pop_back();           // Erase the last character
    } else {
        // If the string is empty or has only one character, return an empty string
        return "";
    }
    return str;
}

vector <City> readDataSet (string filename){
    ifstream f(filename);
    vector<City> c;
    if (!f.is_open()){
        cout << "Cannot open file." << endl;
        f.close();
        return c;
    }

    std::string line;
    bool firstLine = true;

    while (getline(f, line)) {
        if (firstLine) {
            // Skip the header line
            firstLine = false;
            continue;
        }

        stringstream ss(line);
        string token;
        City city;

        string city_name;
        getline(ss, city_name, ',');
        getline(ss, token, ',');
        city.name = trim(token);
        getline(ss, token, ',');
        city.latitude = stod(trim(token));
        // cout << token << endl;
        //city.latitude = stod(token);
        getline(ss, token, ',');
        city.longitude = stod(trim(token));
        // cout << token << endl;
        //city.longitude = stod(token);
        

        c.push_back(city);
    }

    f.close();
    return c;
}

int main (){
    string filename = "worldcities.csv";
    vector<City> cities = readDataSet(filename);

    for (City c : cities){
        cout << c.name << " " << c.latitude << " " << c.longitude << endl;
    }

    return 0;
}
