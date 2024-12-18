#include <cstdio>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>

using std::cout, std::endl;

struct Byte {
    int x;
    int y;
};

int dijkstras_path(std::vector<std::vector<char>> &memory, std::pair<int, int> start, std::pair<int, int> end) {
    // Dijkstras from start to end
    std::vector<std::pair<int, int>> directions = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
    std::vector<std::vector<int>> distance;
    for (int i = 0; i < memory.size(); i++) {
        std::vector<int> row;
        for (int j = 0; j < memory[i].size(); j++) {
            row.push_back(-1);
        }
        distance.push_back(row);
    }

    distance[start.first][start.second] = 0;
    // Prio quueue
    using State = std::tuple<int, int, int>;
    auto cmp = [](const State& a, const State& b) {
        return std::get<0>(a) > std::get<0>(b); // Min-heap: prioritize smaller cost
    };
    std::priority_queue<State> queue;
    queue.push({0, start.first, start.second});

    while (!queue.empty()) {
        auto [cost, x, y] = queue.top();
        queue.pop();

        for (int i = 0; i < directions.size(); i++) {
            int new_x = x + directions[i].first;
            int new_y = y + directions[i].second;

            if (new_x < 0 || new_x >= memory.size() || new_y < 0 || new_y >= memory[new_x].size()) {
                continue;
            }

            if (memory[new_x][new_y] == '#') {
                continue;
            }

            if (distance[new_x][new_y] == -1 || distance[new_x][new_y] > distance[x][y] + 1) {
                distance[new_x][new_y] = distance[x][y] + 1;
                queue.push({distance[new_x][new_y], new_x, new_y});
            }
        }
    }
    return distance[end.first][end.second];
}

// Returns false if there is no path
bool dijkstras_check(std::vector<std::vector<char>> &memory, std::pair<int, int> start, std::pair<int, int> end) {
    // Dijkstras from start to end
    std::vector<std::pair<int, int>> directions = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
    std::vector<std::vector<int>> distance;
    for (int i = 0; i < memory.size(); i++) {
        std::vector<int> row;
        for (int j = 0; j < memory[i].size(); j++) {
            row.push_back(-1);
        }
        distance.push_back(row);
    }

    distance[start.first][start.second] = 0;
    // Prio quueue
    using State = std::tuple<int, int, int>;
    auto cmp = [](const State& a, const State& b) {
        return std::get<0>(a) > std::get<0>(b); // Min-heap: prioritize smaller cost
    };
    std::priority_queue<State> queue;
    queue.push({0, start.first, start.second});

    while (!queue.empty()) {
        auto [cost, x, y] = queue.top();
        queue.pop();

        for (int i = 0; i < directions.size(); i++) {
            int new_x = x + directions[i].first;
            int new_y = y + directions[i].second;

            if (new_x < 0 || new_x >= memory.size() || new_y < 0 || new_y >= memory[new_x].size()) {
                continue;
            }

            if (memory[new_x][new_y] == '#') {
                continue;
            }

            if (distance[new_x][new_y] == -1 || distance[new_x][new_y] > distance[x][y] + 1) {
                distance[new_x][new_y] = distance[x][y] + 1;
                queue.push({distance[new_x][new_y], new_x, new_y});
            }
        }
    }
    if (distance[end.first][end.second] == -1) {
        return false;
    }
    return true;
}

int part1(std::vector<Byte> bytes, std::vector<std::vector<char>> memory) {
    for (int i = 0; i < 1024; i++) {
        // Place byte in memory
        memory[bytes[i].y][bytes[i].x] = '#';
    }

    // Dijkstras from 0,0 to 70,70
    cout << "Shortest path " << dijkstras_path(memory, {0, 0}, {70, 70}) << endl;

    return 0;
}

int part2(std::vector<Byte> bytes, std::vector<std::vector<char>> memory) {
    int hi = bytes.size();
    int lo = 0;
    // Binary search, first dijktras to fail is the last byte to be placed
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        for (int i = 0; i < mid; i++) {
            memory[bytes[i].y][bytes[i].x] = '#';
        }
        if (dijkstras_check(memory, {0, 0}, {70, 70})) {
            lo = mid + 1;
        } else {
            hi = mid;
        }
        // Reset memory
        for (int i = 0; i < mid; i++) {
            memory[bytes[i].y][bytes[i].x] = '.';
        }
    }

    cout << "Last byte to be placed: " << bytes[lo - 1].x << ", " << bytes[lo - 1].y << endl;

    return 0;
}



int main() {
    std::ifstream file("../d18/input.txt");
    std::vector<std::vector<char>> memory;
    int max_x = 71;
    int max_y = 71; // 6x6 grid
    std::vector<Byte> bytes;
    std::string line;
    while (std::getline(file, line)) {
        // Parse comma separated value %d, %d
        Byte byte;
        sscanf(line.c_str(), "%d, %d", &byte.x, &byte.y);
        bytes.push_back(byte);
    }

    for (int i = 0; i < max_y; i++) {
        std::vector<char> row;
        for (int j = 0; j < max_x; j++) {
            row.push_back('.');
        }
        memory.push_back(row);
    }

    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    part1(bytes, memory);
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    part2(bytes, memory);
    std::chrono::steady_clock::time_point end2 = std::chrono::steady_clock::now();

    cout << "Part 1 duration: " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[µs]" << endl;
    cout << "Part 2 duration: " << std::chrono::duration_cast<std::chrono::microseconds>(end2 - end).count() << "[µs]" << endl;

    return 0;
}