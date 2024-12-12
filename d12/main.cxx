#include <chrono>
#include <iostream>
#include <fstream>
#include <unordered_set>
#include <utility>
#include <vector>

using std::cout, std::endl;

struct hashPair {
    template<class T1, class T2>
    std::size_t operator()(const std::pair<T1, T2> &pair) const {
        return std::hash<T1>()(pair.first) ^ std::hash<T2>()(pair.second);
    }
};

struct BoundaryCell {
    std::pair<int, int> coord;
    std::pair<int, int> direction;

    // Define operator== for BoundaryCell
    bool operator==(const BoundaryCell& other) const {
        return coord == other.coord && direction == other.direction;
    }
};

struct hashBoundaryCell {
    std::size_t operator()(const BoundaryCell &cell) const {
        return std::hash<int>()(cell.coord.first) ^ std::hash<int>()(cell.coord.second) ^
               std::hash<int>()(cell.direction.first) ^ std::hash<int>()(cell.direction.second);
    }
};

struct Region {
    char plant;
    std::unordered_set<std::pair<int, int>, hashPair> coords;
    std::unordered_set<BoundaryCell, hashBoundaryCell> boundaryCells;
};

Region bfsRegion(std::vector<std::vector<char>> &grid, int startx, int starty) {
    Region region{};
    region.plant = grid[startx][starty];
    std::vector<std::pair<int, int>> directions = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
    std::unordered_set<std::pair<int, int>, hashPair> visited;
    std::vector<std::pair<int, int>> queue;

    queue.push_back({startx, starty});
    while (!queue.empty()) {
        auto [cx, cy] = queue.back(); queue.pop_back();
        if (visited.find({cx, cy}) != visited.end()) {
            continue;
        }
        visited.insert({cx, cy});
        region.coords.insert({cx, cy});

        for (auto [dx, dy] : directions) {
            int nx = cx + dx;
            int ny = cy + dy;
            if (nx < 0 || nx >= grid.size() || ny < 0 || ny >= grid[nx].size()) { //OOB
                region.boundaryCells.insert({{cx, cy}, {dx, dy}});
                continue;
            }
            if (grid[nx][ny] == region.plant) {
                queue.push_back({nx, ny});
            } else {
                region.boundaryCells.insert({{cx, cy}, {dx, dy}});
            }
        }
    }

    return region;
}

int part1(std::vector<std::vector<char>> &grid) {
    std::vector<Region> regions;
    for (int i = 0; i < grid.size(); i++) {
        for (int j = 0; j < grid[i].size(); j++) {
            // Check if coordinate already in a region with that plant
            bool found = false;
            for (auto &region : regions) {
                if (region.plant == grid[i][j] && region.coords.find({i, j}) != region.coords.end()) {
                    found = true;
                    break;
                }
            }
            if (found) {
                continue;
            }
            regions.push_back(bfsRegion(grid, i, j));
        }
    }

    // Calculate perimiter and area of each region
    int total = 0;
    for (auto &region : regions) {
        int area = region.coords.size();
        int perimiter = 0;
        for (auto [x, y] : region.coords) {
            std::vector<std::pair<int, int>> directions = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
            for (auto [dx, dy] : directions) {
                int nx = x + dx;
                int ny = y + dy;
                if (nx < 0 || nx >= grid.size() || ny < 0 || ny >= grid[nx].size()) { //OOB
                    perimiter++;
                } else if (region.coords.find({nx, ny}) == region.coords.end()) {
                    perimiter++;
                }
            }
        }
        total += area * perimiter;
    }
    return total;
}

int part2(std::vector<std::vector<char>> &grid) {
    std::vector<Region> regions;
    for (int i = 0; i < grid.size(); i++) {
        for (int j = 0; j < grid[i].size(); j++) {
            // Check if coordinate already in a region with that plant
            bool found = false;
            for (auto &region : regions) {
                if (region.plant == grid[i][j] && region.coords.find({i, j}) != region.coords.end()) {
                    found = true;
                    break;
                }
            }
            if (found) {
                continue;
            }
            regions.push_back(bfsRegion(grid, i, j));
        }
    }

    // Calculate number of sides using boundary cells
    int total = 0;
    for (auto &region : regions) {
        int sides = 0;
        // Go through boundary cells and prune continuous sides with same direction
        for (auto &bc : region.boundaryCells) {
            int d = 1;
            // Erase the continuous side from boundaryCells the current direction
            while (region.boundaryCells.find({{bc.coord.first + d * bc.direction.second, bc.coord.second + d * bc.direction.first}, bc.direction}) != region.boundaryCells.end()) {
                // Erase the continuous side from boundaryCells
                region.boundaryCells.erase({{bc.coord.first + d * bc.direction.second, bc.coord.second + d * bc.direction.first}, bc.direction});
                d++;
            }

            d = 1;
            while (region.boundaryCells.find({{bc.coord.first + d * bc.direction.second, bc.coord.second + d * bc.direction.first}, bc.direction}) != region.boundaryCells.end()) {
                // Erase the continuous side from boundaryCells
                region.boundaryCells.erase({{bc.coord.first + d * bc.direction.second, bc.coord.second + d * bc.direction.first}, bc.direction});
                d++;
            }

        } 
        total += region.boundaryCells.size() * region.coords.size(); // Cost = sides * area
    }

    return total;
}

int main() {
    std::ifstream file("../d12/input.txt");
    std::string line;
    std::vector<std::vector<char>> grid;

    while (std::getline(file, line)) {
        std::vector<char> row;
        for (char c : line) {
            row.push_back(c);
        }
        grid.push_back(row);
    }
    
    std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
    int res1 = part1(grid);
    std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
    int res2 = part2(grid);
    std::chrono::high_resolution_clock::time_point t3 = std::chrono::high_resolution_clock::now();

    auto duration1 = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
    auto duration2 = std::chrono::duration_cast<std::chrono::microseconds>( t3 - t2 ).count();
    cout << "Part 1: " << res1 << ", " << duration1 << "us" << endl;
    cout << "Part 2: " << res2 << ", " << duration2 << "us" << endl;

    return 0;
}