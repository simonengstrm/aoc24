#include <chrono>
#include <iostream>
#include <fstream>
#include <set>
#include <string>
#include <unordered_set>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

struct hashFunction {
    size_t operator()(const pair<int, int> &p) const {
        return hash<int>()(p.first) ^ hash<int>()(p.second);
    }
};

bool checkOrder(vector<int> update, int index, int number, unordered_set<pair<int,int>, hashFunction> orders) {
    for (int i = 0; i < index; i++) {
        if (orders.find({update[i], number}) == orders.end()) {
            return false;
        }
    }
    return true;
}

int part1() {
    ifstream file("../d5/input.txt");
    string line;
    unordered_set<pair<int, int>, hashFunction> orders;
    vector<vector<int>> updates;
    vector<int> correctUpdateIndices;

    while (getline(file, line)) {
        if (line == "") {
            break;
        }
        // Split at | and parse out number before and after
        string s1 = line.substr(0, line.find('|'));
        string s2 = line.substr(line.find('|') + 1, line.length());
        orders.insert({stoi(s1), stoi(s2)});
    }

    // Parse updates
    while (getline(file, line)) {
        vector<int> update;
        // Separate by comma and parse out numbers, make sure to get the last one
        size_t pos = 0;
        while ((pos = line.find(',')) != string::npos) {
            update.push_back(stoi(line.substr(0, pos)));
            line.erase(0, pos + 1);
        }
        update.push_back(stoi(line));
        updates.push_back(update);
    }

    // Run updates and check their orders
    for (int i = 0; i < updates.size(); i++) {
        bool correct = true;
        for (int j = 1; j < updates[i].size(); j++) {
            if (orders.find({updates[i][j - 1], updates[i][j]}) == orders.end()) {
                correct = false;
                break;
            }
        }
        if (correct) {
            correctUpdateIndices.push_back(i);
        }
    }

    int sum = 0;
    // Sum the middle elements of the correct updates
    for (auto index : correctUpdateIndices) {
        sum += updates[index][updates[index].size() / 2];
    }

    cout << "sum: " << sum << endl;

    return 0;
}

int part2() {
    ifstream file("../d5/input.txt");
    string line;
    vector<vector<int>> updates;
    vector<vector<int>> inCorrectUpdates;
    unordered_set<pair<int, int>, hashFunction> orders;

    while (getline(file, line)) {
        if (line == "") {
            break;
        }
        // Split at | and parse out number before and after
        string s1 = line.substr(0, line.find('|'));
        string s2 = line.substr(line.find('|') + 1, line.length());
        orders.insert({stoi(s1), stoi(s2)});
    }

    // Parse updates
    while (getline(file, line)) {
        vector<int> update;
        // Separate by comma and parse out numbers, make sure to get the last one
        size_t pos = 0;
        while ((pos = line.find(',')) != string::npos) {
            update.push_back(stoi(line.substr(0, pos)));
            line.erase(0, pos + 1);
        }
        update.push_back(stoi(line));
        updates.push_back(update);
    }

    // Run updates and check their orders
    for (int i = 0; i < updates.size(); i++) {
        // Window over the update in sizes of two and check order with comparator
        for (int j = 1; j < updates[i].size(); j++) {
            if (orders.find({updates[i][j - 1], updates[i][j]}) == orders.end()) {
                inCorrectUpdates.push_back(updates[i]);
                break;
            }
        }
        
    }

    auto comp = [&orders](int a, int b) {
        // Check who precedes who
        if (orders.find({a, b}) != orders.end()) {
            return true;
        }
        return false;
    };

    for (auto &update : inCorrectUpdates) {
        sort(update.begin(), update.end(), comp);
    }

    int sum = 0;
    for (auto update : inCorrectUpdates) {
        sum += update[update.size() / 2];
    }

    cout << "sum: " << sum << endl;
    return 0;
}

int main() {
    std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
    part1();
    part2();
    std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
    cout << "Time: " << duration << " microseconds" << endl;

    return 0;
}