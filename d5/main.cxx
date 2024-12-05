#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

map<int, vector<int>> orders;

bool isInList(vector<int> list, int number) {
    for (int i = 0; i < list.size(); i++) {
        if (list[i] == number) {
            return true;
        }
    }
    return false;
}

bool checkOrder(vector<int> update, int index, int number, map<int, vector<int>> orders) {
    for (int i = 0; i < index; i++) {
        if (!isInList(orders[update[i]], number)) {
            cout << "Number " << number << " is not in the list of " << update[i] << endl;
            return false;
        }
    }
    return true;
}

int part1() {
    ifstream file("../d5/input.txt");
    string line;
    map<int, vector<int>> orders;
    vector<vector<int>> updates;
    vector<int> correctUpdateIndices;

    while (getline(file, line)) {
        if (line == "") {
            break;
        }
        // Split at | and parse out number before and after
        string s1 = line.substr(0, line.find('|'));
        string s2 = line.substr(line.find('|') + 1, line.length());
        orders[stoi(s1)].push_back(stoi(s2));
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

    // Print updates
    for (int i = 0; i < updates.size(); i++) {
        for (int n = 0; n < updates[i].size(); n++) {
            cout << updates[i][n] << " ";
        }
        cout << endl;
    }

    // Run updates and check their orders
    for (int i = 0; i < updates.size(); i++) {
        bool correct = true;
        for (int n = 0; n < updates[i].size(); n++) {
            cout << "Checking " << updates[i][n] << " at index " << n << endl;
            if (!checkOrder(updates[i], n, updates[i][n], orders)) {
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

bool comp(int a, int b) {
    // Check who precedes who
    for (int i = 0; i < orders[a].size(); i++) {
        if (orders[a][i] == b) {
            return true;
        }
    }
    return false;
} 

int part2() {
    ifstream file("../d5/input.txt");
    string line;
    vector<vector<int>> updates;
    vector<vector<int>> inCorrectUpdates;

    while (getline(file, line)) {
        if (line == "") {
            break;
        }
        // Split at | and parse out number before and after
        string s1 = line.substr(0, line.find('|'));
        string s2 = line.substr(line.find('|') + 1, line.length());
        orders[stoi(s1)].push_back(stoi(s2));
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

    // Print updates
    for (int i = 0; i < updates.size(); i++) {
        for (int n = 0; n < updates[i].size(); n++) {
            cout << updates[i][n] << " ";
        }
        cout << endl;
    }

    // Run updates and check their orders
    for (int i = 0; i < updates.size(); i++) {
        bool correct = true;
        for (int n = 0; n < updates[i].size(); n++) {
            if (!checkOrder(updates[i], n, updates[i][n], orders)) {
                correct = false;
                inCorrectUpdates.push_back(updates[i]);
                break;
            }
        }
    }

    // Sort the incorrect updates
    for (auto &update : inCorrectUpdates) {
        sort(update.begin(), update.end(), comp);
    }

    // Print sorted updates
    for (auto update : inCorrectUpdates) {
        for (auto number : update) {
            cout << number << " ";
        }
        cout << endl;
    }

    int sum = 0;
    for (auto update : inCorrectUpdates) {
        sum += update[update.size() / 2];
    }

    cout << "sum: " << sum << endl;
    return 0;
}

int main() {
    part2();
    return 0;
}