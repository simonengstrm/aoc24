#include <functional>
#include <iostream>
#include <fstream>
#include <unordered_set>
#include <utility>
#include <vector>

using std::cout, std::endl, std::string;

enum Direction {
    UP,
    RIGHT,
    DOWN,
    LEFT
};

struct State {
    int y;
    int x;
    Direction dir;

    bool operator==(const State &other) const {
        return y == other.y && x == other.x && dir == other.dir;
    }
};

struct stateHashFunction {
    size_t operator()(const State &s) const {
        return std::hash<int>()(s.y) ^ std::hash<int>()(s.x) ^ std::hash<int>()(static_cast<int>(s.dir));
    }
};

struct hashFunction {
    size_t operator()(const std::pair<int, int> &p) const {
        return std::hash<int>()(p.first) ^ std::hash<int>()(p.second);
    }
};

bool checkLoop(int y, int x, Direction dir, const std::vector<std::vector<char>> &grid) {
    int startX = x;
    int startY = y;
    Direction startDir = dir;
    std::unordered_set<State, stateHashFunction> visited;

    while (x >= 0 && x < grid[0].size() && y >= 0 && y < grid.size()) {
        int nextX = x;
        int nextY = y;
        switch (dir) {
            case UP:
                nextY--;
                break;
            case RIGHT:
                nextX++;
                break;
            case DOWN:
                nextY++;
                break;
            case LEFT:
                nextX--;
                break;
        }

        // Check next position
        if (nextY < 0 || nextY >= grid.size() || nextX < 0 || nextX >= grid[0].size()) {
            return false;
        }
        char next = grid[nextY][nextX];
        if (next == '#') {
            dir = static_cast<Direction>((static_cast<int>(dir) + 1) % 4);
        } else {
            y = nextY;
            x = nextX;
            State s = {y, x, dir};
            if (visited.find(s) != visited.end()) {
                return true;
            }
            visited.insert(s);
        }
    }
    return false;
}

int part2(std::vector<std::vector<char>> grid, std::unordered_set<std::pair<int, int>, hashFunction> obstacleCandidates) {
    int startX = 0;
    int startY = 0;

    for (int i = 0; i < grid.size(); i++) {
        for (int j = 0; j < grid[0].size(); j++) {
            if (grid[i][j] == '^') {
                startY = i;
                startX = j;
                break;
            }
        }
    }

    Direction dir = UP;
    int loops = 0;

    // Iterate visited set
    for (auto &p : obstacleCandidates) {
        if (p.first == startY && p.second == startX) {
            continue;
        }
        grid[p.first][p.second] = '#';

        if (checkLoop(startY, startX, dir, grid)) {
            cout << "Loop found at " << p.first << " " << p.second << endl;
            loops++;
        }

        grid[p.first][p.second] = '.';
    }

    cout << loops << endl;
    return 0;
}

int part1() {
    std::ifstream file("../d6/input.txt");
    int y = 0, x = 0;
    std::vector<std::vector<char>> grid;

    string line;
    while (std::getline(file, line)) {
        grid.push_back(std::vector<char>(line.begin(), line.end()));
    }

    for (int i = 0; i < grid.size(); i++) {
        for (int j = 0; j < grid[0].size(); j++) {
            if (grid[i][j] == '^') {
                y = i;
                x = j;
                break;
            }
        }
    }

    std::unordered_set<std::pair<int, int>, hashFunction> visited;

    visited.insert({y, x});
    Direction dir = UP;

    while(y >= 0 && y < grid.size() && x >= 0 && x < grid[0].size()) {  
        // Check next position
        int nextY = y;
        int nextX = x;

        switch (dir) {
            case UP:
                nextY--;
                break;
            case RIGHT:
                nextX++;
                break;
            case DOWN:
                nextY++;
                break;
            case LEFT:
                nextX--;
                break;
        }

        // Check next position
        if (nextY < 0 || nextY >= grid.size() || nextX < 0 || nextX >= grid[0].size()) {
            //cout << "Out of bounds, exiting" << endl;
            break;
        }
        char next = grid[nextY][nextX];
        if (next == '#') {
            //cout << "Wall, turning" << endl;
            dir = static_cast<Direction>((static_cast<int>(dir) + 1) % 4);
        } else {
            //cout << "Moving to " << nextY << " " << nextX << endl;
            y = nextY;
            x = nextX;
            visited.insert({y, x});
        }
    }   

    cout << visited.size() << endl;
    part2(grid, visited);
    
    return 0;
}

int main() {
    part1();
    return 0;
}