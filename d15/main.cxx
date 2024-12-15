#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <fstream>

using std::cout, std::endl, std::string;

void printGrid(std::vector<std::vector<char>> grid) {
    for (std::vector<char> row : grid) {
        for (char c : row) {
            std::cout << c;
        }
        cout << endl;
    }
    cout << endl;
}

std::pair<int, int> move(std::vector<std::vector<char>>& grid, int x, int y, int nx, int ny) {
    // Check if we can move to next position
    if (grid[ny][nx] == '#') {
        return std::make_pair(x, y);
    }

    // If an obstacle is in the way, find first open space (.) and swap obstacle with open space, then move to nx, ny
    // If no open space is found (wall), return
    if (grid[ny][nx] == 'O') {
        int ox = nx, oy = ny;
        while (grid[oy][ox] != '.') {
            // Step in the current direction
            int dx = nx - x;
            int dy = ny - y;
            ox += dx;
            oy += dy;
            if (grid[oy][ox] == '#') {
                return std::make_pair(x, y);
            }
        }
        grid[ny][nx] = '.';
        grid[oy][ox] = 'O';
    }

    // We've moved the obstacle if there was one, now move to nx, ny
    grid[y][x] = '.';
    grid[ny][nx] = '@';
    return std::make_pair(nx, ny);
}

int part1(std::vector<std::vector<char>> grid, std::vector<char> instructions) {
    // Find @ (start position)
    std::pair<int, int> current; // x, y
    for (int i = 0; i < grid.size(); i++) {
        for (int j = 0; j < grid[i].size(); j++) {
            if (grid[i][j] == '@') {
                current = std::make_pair(j, i);
            }
        }
    }

    // Follow instructions
    for (char c : instructions) {
        int x = current.first;
        int y = current.second;
        int nx = x;
        int ny = y;
        switch (c) {
            case '^':
                ny--;
                break;
            case 'v':
                ny++;
                break;
            case '<':
                nx--;
                break;
            case '>':
                nx++;
                break;
        }
        //cout << "Move: " << c << endl;
        current = move(grid, x, y, nx, ny);
        //printGrid(grid);
    }

    // Calculate score
    int score = 0;
    for (int i = 0; i < grid.size(); i++) {
        for (int j = 0; j < grid[i].size(); j++) {
            if (grid[i][j] == 'O') {
                score += i * 100 + j;
            }
        }
    }

    cout << "Score: " << score << endl;

    return 0;
}

int main() {
    std::ifstream file("../d15/example.txt");
    std::vector<std::vector<char>> grid;
    std::vector<char> instructions;

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) {
            break;
        }
        std::vector<char> row;
        for (char c : line) {
            row.push_back(c);
        }
        grid.push_back(row);
    }

    while (std::getline(file, line)) {
        for (char c : line) {
            instructions.push_back(c);
        }
    }

    for (std::vector<char> row : grid) {
        for (char c : row) {
            std::cout << c;
        }
        cout << endl;
    }

    part1(grid, instructions);

    return 0;
}