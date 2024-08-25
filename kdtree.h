#ifndef KDTREE_H
#define KDTREE_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

#define M_PI 3.14159265358979323846

struct City {
    std::string name;
    double latitude;
    double longitude;
};

struct Node {
    City city;
    Node *left;
    Node *right;
    Node(City c) : city(c), left(nullptr), right(nullptr) {}
};

class KDTree {
private:
    Node* root;

    Node* insertRec(Node* node, City city, unsigned depth);
    void rangeSearchRec(Node* node, double lat_min, double lng_min, double lat_max, double lng_max, std::vector<City>& result, unsigned depth);
    double haversine(double lat1, double lon1, double lat2, double lon2);
    void nearestRec(Node* node, City target, City& best, double& best_dist, unsigned depth);
    void deleteTree(Node* node);

public:
    KDTree();
    ~KDTree();
    void insert(City city);
    std::vector<City> rangeSearch(double lat_min, double lng_min, double lat_max, double lng_max);
    City nearestNeighbor(City target);
    Node* getRoot(); 
    Node*& getRootAddress();
};

#endif
