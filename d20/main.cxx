#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>
#include <algorithm>
#include <map>

using std::cout, std::endl;

struct Shortcut {
    std::pair<int,int> start;
    std::pair<int,int> end;
    int saved_steps;

    bool operator==(const Shortcut &other) const {
        return (start == other.start && end == other.end);
    }
};

namespace std {
    template <>
    struct hash<Shortcut> {
        std::size_t operator()(const Shortcut &shortcut) const {
            return std::hash<int>{}(shortcut.start.first) ^ std::hash<int>{}(shortcut.start.second) ^ std::hash<int>{}(shortcut.end.first) ^ std::hash<int>{}(shortcut.end.second);
        }
    };

    template <>
    struct hash<std::pair<int,int>> {
        std::size_t operator()(const std::pair<int,int> &pair) const {
            return std::hash<int>{}(pair.first) ^ std::hash<int>{}(pair.second);
        }
    };
}


std::vector<std::pair<int,int>> bfs_path(std::vector<std::vector<char>> &grid, std::pair<int, int> current, std::pair<int, int> end) {
    std::vector<std::pair<int,int>> path;
    std::vector<std::pair<int,int>> directions = {{0,1}, {0,-1}, {1,0}, {-1,0}};
    std::vector<std::vector<bool>> visited(grid.size(), std::vector<bool>(grid[0].size(), false));
    std::vector<std::vector<std::pair<int,int>>> parent(grid.size(), std::vector<std::pair<int,int>>(grid[0].size(), {-1,-1}));

    std::queue<std::pair<int,int>> q;
    q.push(current);
    visited[current.first][current.second] = true;

    while (!q.empty()) {
        std::pair<int,int> node = q.front();
        q.pop();

        if (node == end) {
            break;
        }

        for (auto dir : directions) {
            int x = node.first + dir.first;
            int y = node.second + dir.second;

            if (x >= 0 && x < grid.size() && y >= 0 && y < grid[0].size() && grid[x][y] != '#' && !visited[x][y]) {
                q.push({x,y});
                visited[x][y] = true;
                parent[x][y] = node;
            }
        }
    }

    std::pair<int,int> node = end;
    while (node != current) {
        path.push_back(node);
        node = parent[node.first][node.second];
    }
    path.push_back(current);
    std::reverse(path.begin(), path.end());

    return path;
}

bool inBounds(std::vector<std::vector<char>> &grid, std::pair<int, int> pos) {
    return pos.first >= 0 && pos.first < grid.size() && pos.second >= 0 && pos.second < grid[0].size();
}

int indexOf(std::vector<std::pair<int,int>> &path, std::pair<int,int> pos) {
    for (int i = 0; i < path.size(); i++) {
        if (path[i] == pos) {
            return i;
        }
    }
    return -1;
}

int part2(std::vector<std::vector<char>> &grid, std::pair<int, int> start, std::pair<int, int> end) {
    // Find path from start to end using bfs
    std::vector<std::pair<int,int>> path = bfs_path(grid, start, end);
    cout << "Path length: " << path.size() << endl;
    std::unordered_set<Shortcut> shortcuts;

    std::vector<std::pair<int, int>> directions = {{0,1}, {0,-1}, {1,0}, {-1,0}};
    
    for (int i = 0; i < path.size(); i++) {
        int y = path[i].first;
        int x = path[i].second;

        std::unordered_set<std::pair<int,int>> visited;
        std::vector<std::vector<int>> distance(grid.size(), std::vector<int>(grid[0].size(), -1));

        std::queue<std::pair<int,int>> q;
        q.push({y,x});
        visited.insert({y,x});
        distance[y][x] = 0;

        while (!q.empty()) {
            std::pair<int,int> node = q.front();
            q.pop();

            if (distance[node.first][node.second] == 20) {
                continue;
            }

            for (auto [dir_y, dir_x] : directions) {
                int new_y = node.first + dir_y;
                int new_x = node.second + dir_x;

                if (inBounds(grid, {new_y, new_x}) && visited.find({new_y, new_x}) == visited.end()) {
                    q.push({new_y, new_x});
                    visited.insert({new_y, new_x});
                    distance[new_y][new_x] = distance[node.first][node.second] + 1;

                    if (grid[new_y][new_x] != '#') {
                        // Calculate how many steps are saved by taking the shortcut
                        int total = path.size();
                        std::pair<int,int> shortcut_end = std::make_pair(new_y, new_x);
                        int index = indexOf(path, shortcut_end);
                        int index_to_end = path.size() - index;
                        int total_with_shortcut = i + distance[new_y][new_x] + index_to_end;
                        int saved_steps = total - total_with_shortcut;
                        if (saved_steps >= 100) {
                            shortcuts.insert({path[i], shortcut_end, saved_steps});
                        }
                    }
                }
            }
        }
    }

    // Counte the number of shortcuts with a specific number of saved steps
    std::map<int, int> shortcut_to_count;
    for (auto shortcut : shortcuts) {
        shortcut_to_count[shortcut.saved_steps]++;
    }

    cout << "Total shortcuts: " << shortcuts.size() << endl;

    return 0;
}

int part1(std::vector<std::vector<char>> &grid, std::pair<int, int> start, std::pair<int, int> end) {
    std::vector<std::pair<int,int>> path = bfs_path(grid, start, end);
    std::unordered_map<int, int> shortcut_to_count;
    int count = 0;

    std::vector<std::pair<int, int>> directions = {{0,1}, {0,-1}, {1,0}, {-1,0}};
    for (int i = 0; i < path.size(); i++) {
        int y = path[i].first;
        int x = path[i].second;
        for (auto [dir1_y, dir1_x] : directions) {
            for (auto [dir2_y, dir2_x] : directions) {
                int s1_y = y + dir1_y;
                int s1_x = x + dir1_x;
                int s2_y = s1_y + dir2_y;
                int s2_x = s1_x + dir2_x;
                if (inBounds(grid, {s1_y, s1_x}) && 
                    inBounds(grid, {s2_y, s2_x}) && 
                    grid[s1_y][s1_x] == '#' && grid[s2_y][s2_x] != '#') {
                    auto it = std::find(path.begin(), path.end(), std::pair<int,int>{s2_y, s2_x});
                    int index = std::distance(path.begin(), it);
                    int saved_steps = index - i - 2;
                    if (saved_steps >= 100) {
                        shortcut_to_count[saved_steps]++;
                        count++;
                    }
                }
            } 
        }
    }

    cout << "Total shortcuts that save over 100 steps: " << count << endl;
    
    return 0;
}

int main() {
    std::fstream file("../d20/input.txt");
    std::vector<std::vector<char>> grid;
    std::string line;

    while (std::getline(file, line)) {
        grid.push_back(std::vector<char>(line.begin(), line.end()));
    }

    std::pair<int, int> start;
    std::pair<int, int> end;
    for (int i = 0; i < grid.size(); i++) {
        for (int j = 0; j < grid[i].size(); j++) {
            if (grid[i][j] == 'S') {
                start = {i, j};
            } else if (grid[i][j] == 'E') {
                end = {i, j};
            }
        }
    }

    part1(grid, start, end);
    part2(grid, start, end);
    return 0;
}