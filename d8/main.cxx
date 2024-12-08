#include <iostream>
#include <fstream>
#include <string>
#include <utility>
#include <vector>
#include <set>

using std::cout, std::endl, std::ifstream, std::string;

int main() {
    ifstream file("../d8/input.txt");
    string line;
    
    std::vector<std::pair<int, int>> antennas;
    std::set<std::pair<int, int>> antinodes;
    std::vector<std::vector<char>> grid;

    while (getline(file, line)) {
        std::vector<char> row;
        for (char c : line) {
            row.push_back(c);
        }
        grid.push_back(row);
    }

    for (int i = 0; i < grid.size(); i++) {
        for (int j = 0; j < grid[i].size(); j++) {
            if (grid[i][j] != '.') {
                antennas.push_back({i, j});
            }
        }
    }

    for (int i = 0; i < antennas.size(); i++) {
        int y1 = antennas[i].first;
        int x1 = antennas[i].second;
        for (int j = i + 1; j < antennas.size(); j++) {
            int y2 = antennas[j].first;
            int x2 = antennas[j].second;
            if (y1 == y2 && x1 == x2 || grid[y1][x1] != grid[y2][x2]) {
                continue;
            }
            int dx = x2 - x1;
            int dy = y2 - y1;
            int current_y = y2;
            int current_x = x2;
            // Check distance from point to antennas
            
        }
    }

    cout << antinodes.size() << endl;

    antinodes = {};

    for (int i = 0; i < antennas.size(); i++) {
        int y1 = antennas[i].first;
        int x1 = antennas[i].second;
        for (int j = i + 1; j < antennas.size(); j++) {
            int y2 = antennas[j].first;
            int x2 = antennas[j].second;
            if (y1 == y2 && x1 == x2 || grid[y1][x1] != grid[y2][x2]) {
                continue;
            }
            // Find antinodes along the line of the two antennas
            int dy = y2 - y1;
            int dx = x2 - x1;

            // Add antennas to antinodes
            antinodes.insert({y1, x1});
            antinodes.insert({y2, x2});
            int current_y = y2;
            int current_x = x2;
            // Iterate while we are not at the same point as the first antenna
            // Make sure we wrap around the grid but not to a new line equation, stay on the same line
            int wrap = 0;
            while (true) {
                antinodes.insert({current_y, current_x});
                if (current_y + dy >= grid.size() || current_y + dy < 0 || current_x + dx >= grid[0].size() || current_x + dx < 0) {
                    if (wrap == 1) {
                        break;
                    }
                    dy = -dy;
                    dx = -dx;
                    wrap++;
                }
                current_y = (current_y + dy);
                current_x = (current_x + dx);
            }
            
        }
    }   

    cout << antinodes.size() << endl;

    return 0;
}