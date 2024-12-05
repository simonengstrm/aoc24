#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>

using namespace std;

int checkDirections(vector<vector<char>> &grid, int i, int j) {
    string buff = "";
    int count = 0;
    // Right
    for (int x = 0; x < 4; x++) {
        if (x+j < grid[i].size()) {
            buff += grid[i][j+x];
        } else {
            break;
        }
    }
    if (buff == "XMAS") {
        count ++;
    }
    buff = "";

    //Left
    for (int x = 0; x < 4; x++) {
        if (j-x >= 0) {
            buff += grid[i][j-x];
        } else {
            break;
        }
    }
    if (buff == "XMAS") {
        count ++;
    }
    buff = "";

    // Up
    for (int y = 0; y < 4; y++) {
        if (i-y >= 0) {
            buff += grid[i-y][j];
        } else {
            break;
        }
    }
    if (buff == "XMAS") {
        count ++;
    }
    buff = "";

    // Down
    for (int y = 0; y < 4; y++) {
        if (y+i < grid.size()) {
            buff += grid[i+y][j];
        } else {
            break;
        }
    }
    if (buff == "XMAS") {
        count ++;
    }
    buff ="";

    // Diagonals
    for (int x = 0; x < 4; x++) {
        if (i-x >= 0 && j+x < grid[i].size()) {
            buff += grid[i-x][j+x];
        } else {
            break;
        }
    }
    if (buff == "XMAS") {
        count ++;
    }
    buff = "";

    for (int x = 0; x < 4; x++) {
        if (i-x >= 0 && j-x >= 0) {
            buff += grid[i-x][j-x];
        } else {
            break;
        }
    }
    if (buff == "XMAS") {
        count ++;
    }
    buff = "";

    for (int x = 0; x < 4; x++) {
        if (i+x < grid.size() && j-x >= 0) {
            buff += grid[i+x][j-x];
        } else {
            break;
        }
    }
    if (buff == "XMAS") {
        count ++;
    }
    buff = "";

    for (int x = 0; x < 4; x++) {
        if (i+x < grid.size() && j+x < grid[i].size()) {
            buff += grid[i+x][j+x];
        } else {
            break;
        }
    }
    if (buff == "XMAS") {
        count ++;
    }
    buff = "";


    return count;
}

int checkDiags(vector<vector<char>> &grid, int i, int j) {
    string buff = "";
    int count = 0;
    // Diagonals down right
    for (int x = -1; x < 2; x++) {
        if (i+x < grid.size() && i+x >= 0 && j+x < grid[i].size() && j+x >= 0) {
            buff += grid[i+x][j+x];
        } else {
            break;
        }
    }
    if (buff == "MAS") {
        count ++;
    }
    buff = "";

    // Diagonals up right
    for (int x = -1; x < 2; x++) {
        if (i-x < grid.size() && i-x >= 0 && j+x < grid[i].size() && j+x >= 0) {
            buff += grid[i-x][j+x];
        } else {
            break;
        }
    }
    if (buff == "MAS") {
        count ++;
    }
    buff = "";

    // Diagonals down left
    for (int x = -1; x < 2; x++) {
        if (i+x < grid.size() && i+x >= 0 && j-x < grid[i].size() && j-x >= 0) {
            buff += grid[i+x][j-x];
        } else {
            break;
        }
    }
    if (buff == "MAS") {
        count ++;
    }
    buff = "";
    // Diagonals up left
    for (int x = -1; x < 2; x++) {
        if (i-x < grid.size() && i-x >= 0 && j-x < grid[i].size() && j-x >= 0) {
            buff += grid[i-x][j-x];
        } else {
            break;
        }
    }
    if (buff == "MAS") {
        count ++;
    }
    buff = "";

    // If we found 2 or more, return 1
    return count >= 2 ? 1 : 0;
}

int part1() {
    ifstream file("../d4/input.txt");

    vector<vector<char>> grid;
    int found = 0;

    string line;
    while (getline(file, line)) {
        vector<char> current;
        for (char c : line) {
            current.push_back(c);
        }
        grid.push_back(current);
    }

    for (int i = 0; i < grid.size(); i++) {
        for (int j = 0; j < grid[i].size(); j++) {
            if (grid[i][j] == 'X') {
                found += checkDirections(grid, i, j);
            }
        }
    }

    cout << found << endl;
    return 0;
}

int part2() {
    ifstream file("../d4/input.txt");

    vector<vector<char>> grid;
    int found = 0;

    // Time input reading
    string line;
    while (getline(file, line)) {
        vector<char> current;
        for (char c : line) {
            current.push_back(c);
        }
        grid.push_back(current);
    }

    for (int i = 0; i < grid.size(); i++) {
        for (int j = 0; j < grid[i].size(); j++) {
            if (grid[i][j] == 'A') {
                //cout << "Checking at " << i << " " << j << endl;
                found += checkDiags(grid, i, j);
            }
        }
    }

    cout << found << endl;
    return 0;
}

int main() {
    part1();
    part2();
    return 0;
}