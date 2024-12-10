#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <set>

using std::cout, std::endl, std::ifstream, std::string, std::vector;

int findTrails(vector<vector<int>> grid, int y, int x, std::set<std::pair<int, int>> &visited) {
    cout << "Trail " << grid[y][x] << " at " << y << ", " << x << endl;
    if (grid[y][x] == 9) {
        if (visited.find({y, x}) != visited.end()) {
            cout << "Already visited " << y << ", " << x << endl;
            return 0;
        }
        visited.insert({y, x}); // Uncomment for part 2
        cout << "Trail ends at " << y << ", " << x << endl;
        return 1;
    }

    int trails = 0;
    if (y - 1 >= 0) { // Up
        if (grid[y - 1][x] == grid[y][x] + 1) {
            trails += findTrails(grid, y - 1, x, visited);
        }
    }
    if (y + 1 < grid.size()) { // Down
        if (grid[y + 1][x] == grid[y][x] + 1) {
            trails += findTrails(grid, y + 1, x, visited);
        }
    }
    if (x - 1 >= 0) { // Left
        if (grid[y][x - 1] == grid[y][x] + 1) {
            trails += findTrails(grid, y, x - 1,visited);
        }
    }
    if (x + 1 < grid[y].size()) { // Right
        if (grid[y][x + 1] == grid[y][x] + 1) {
            trails += findTrails(grid, y, x + 1, visited);
        }
    }
    return trails;
}

int main() {
    ifstream file("../d10/input.txt");
    vector<vector<int>> grid;

    string line;
    while (getline(file, line)) {
        vector<int> row;
        for (char c : line) {
            row.push_back(c - '0');
        }
        grid.push_back(row);
    }

    for (const auto &row : grid) {
        for (int i : row) {
            cout << i << " ";
        }
        cout << endl;
    }

    // Find trails
    int sum = 0;
    for (int i = 0; i < grid.size(); i++) {
        for (int j = 0; j < grid[i].size(); j++) {
            if (grid[i][j] == 0) {
                std::set<std::pair<int, int>> visited;
                cout << "Eventual trail start at " << i << ", " << j << endl;
                sum += findTrails(grid, i, j, visited);
            }
        }
    }
    cout << sum << endl;
}