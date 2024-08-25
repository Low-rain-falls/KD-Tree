#include "kdtree.h"

KDTree::KDTree() : root(nullptr) {}

KDTree::~KDTree() {
    deleteTree(root);  
}

void KDTree::deleteTree(Node* node) {
    if (node == nullptr) return;  
    deleteTree(node->left);   
    deleteTree(node->right);  
    delete node;  
}

Node* KDTree::insertRec(Node* node, City city, unsigned depth) {
    if (node == nullptr) {
        return new Node(city);  
    }

    unsigned cd = depth % 2;
    if ((cd == 0 && city.latitude < node->city.latitude) || (cd == 1 && city.longitude < node->city.longitude)) {
        node->left = insertRec(node->left, city, depth + 1);  
    } else {
        node->right = insertRec(node->right, city, depth + 1);  
    }
    return node;
}

void KDTree::insert(City city) {
    root = insertRec(root, city, 0);  
}

void KDTree::rangeSearchRec(Node* node, double lat_min, double lng_min, double lat_max, double lng_max, std::vector<City>& result, unsigned depth) {
    if (node == nullptr) return;  
    
    if (node->city.latitude >= lat_min && node->city.latitude <= lat_max &&
        node->city.longitude >= lng_min && node->city.longitude <= lng_max) {
        result.push_back(node->city);  
    }
  
    unsigned cd = depth % 2;
    
    if ((cd == 0 && lat_min <= node->city.latitude) || (cd == 1 && lng_min <= node->city.longitude)) {
        rangeSearchRec(node->left, lat_min, lng_min, lat_max, lng_max, result, depth + 1);
    }
    if ((cd == 0 && lat_max > node->city.latitude) || (cd == 1 && lng_max > node->city.longitude)) {
        rangeSearchRec(node->right, lat_min, lng_min, lat_max, lng_max, result, depth + 1);
    }
}

std::vector<City> KDTree::rangeSearch(double lat_min, double lng_min, double lat_max, double lng_max) {
    std::vector<City> result;
    rangeSearchRec(root, lat_min, lng_min, lat_max, lng_max, result, 0);  
    return result;
}

double KDTree::haversine(double lat1, double lon1, double lat2, double lon2) {
    const double R = 6371; 
    double dLat = (lat2 - lat1) * M_PI / 180.0;
    double dLon = (lon2 - lon1) * M_PI / 180.0;

    lat1 = lat1 * M_PI / 180.0;
    lat2 = lat2 * M_PI / 180.0;

    double a = sin(dLat / 2) * sin(dLat / 2) +
               sin(dLon / 2) * sin(dLon / 2) * cos(lat1) * cos(lat2);
    double c = 2 * atan2(sqrt(a), sqrt(1 - a));
    return R * c;  
}

void KDTree::nearestRec(Node* node, City target, City& best, double& best_dist, unsigned depth) {
    if (node == nullptr) return;  

    double d = haversine(node->city.latitude, node->city.longitude, target.latitude, target.longitude);

    if (d < best_dist) {
        best_dist = d;  
        best = node->city;  
    }

    unsigned cd = depth % 2;
    Node *next = (cd == 0 && target.latitude < node->city.latitude) || 
                 (cd == 1 && target.longitude < node->city.longitude) ? node->left : node->right;
    Node *other = next == node->left ? node->right : node->left;

    nearestRec(next, target, best, best_dist, depth + 1);

    double d_plane = cd == 0 ? std::abs(target.latitude - node->city.latitude) : std::abs(target.longitude - node->city.longitude);
    if (best_dist > d_plane) {
        nearestRec(other, target, best, best_dist, depth + 1);
    }
}

City KDTree::nearestNeighbor(City target) {
    if (root == nullptr) {
        throw std::runtime_error("KDTree is empty");  
    }

    City best = root->city;
    double best_dist = haversine(root->city.latitude, root->city.longitude, target.latitude, target.longitude);
    nearestRec(root, target, best, best_dist, 0);  
    return best;
}

Node* KDTree::getRoot() {
    return root;  
}

Node*& KDTree::getRootAddress() {
    return root;  
}
