#include <iostream>
#include <fstream>
#include <queue>
#include <stack>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>
#include <string>
#include <unordered_set>

using std::cout, std::endl;

enum Direction {
    N, E, S, W
};

struct Coordinate {
    int x, y;

    bool operator==(const Coordinate &other) const {
        return x == other.x && y == other.y;
    }

    bool operator!=(const Coordinate &other) const {
        return !(x == other.x && y == other.y);
    }
};

struct Vertex {
    Coordinate coord;
    Direction dir;

    bool operator==(const Vertex &other) const {
        return coord == other.coord && dir == other.dir;
    }
};

namespace std {
    template <>
    struct hash<Coordinate> {
        size_t operator()(const Coordinate &c) const {
            auto h1 = std::hash<int>{}(c.x);
            auto h2 = std::hash<int>{}(c.y);
            return h1 ^ (h2 << 1);
        }
    };

    template <>
    struct hash<Vertex> {
        size_t operator()(const Vertex &v) const {
            auto h1 = std::hash<Coordinate>{}(v.coord);
            auto h2 = std::hash<int>{}(v.dir);
            return h1 ^ (h2 << 1);
        }
    };
}

void dijkstras_all(std::unordered_map<Vertex, std::vector<Vertex>> &graph, 
        Vertex start, 
        Coordinate end,
        std::unordered_map<Vertex, int> &dist,
        std::unordered_map<Vertex, std::vector<Vertex>> &prev) {


    using State = std::tuple<int, Vertex>;
    auto cmp = [](const State& a, const State& b) {
        return std::get<0>(a) > std::get<0>(b); // Min-heap: prioritize smaller cost
    };

    std::priority_queue<State, std::vector<State>, decltype(cmp)> pq(cmp);
    pq.push({0, {start}});
    dist[start] = 0;

    while (!pq.empty()) {
        auto [current_cost, current] = pq.top();
        pq.pop();

        if (current.coord == end) {
            continue;
        }

        for (auto &next : graph[current]) {
            // Calculate the direction change to get to the next coordinate and include it in the weight
            int weight = 1 * (current.coord != next.coord) + (current.dir != next.dir) * 1000;

            if (dist.find(next) == dist.end() || dist[next] > dist[current] + weight) {
                dist[next] = dist[current] + weight;
                prev[next].push_back(current);
                pq.push({dist[next], next});
            } else if (dist[next] == dist[current] + weight) {
                prev[next].push_back(current);
            }
        }
    }
}

int part1(std::unordered_map<Vertex, std::vector<Vertex>> & graph, Vertex start, Coordinate end, std::vector<std::vector<char>> & grid) {
    // Dijkstras to find shortest path to E
    std::unordered_map<Vertex, int> dist;
    std::unordered_map<Vertex, std::vector<Vertex>> prev;
    dijkstras_all(graph, start, end, dist, prev);

    // Find end vertex
    Vertex end_vertex = {end, E};
    for (auto &[v, prev_v] : prev) {
        if (v.coord == end) {
            end_vertex = v;
            break;
        }
    }

    cout << dist[end_vertex] << endl;

    return 0;
}

int part2(std::unordered_map<Vertex, std::vector<Vertex>> & graph, Vertex start, Coordinate end, std::vector<std::vector<char>> & grid) {
    // Find all shortest paths to E
    std::unordered_map<Vertex, int> dist;
    std::unordered_map<Vertex, std::vector<Vertex>> prev;
    dijkstras_all(graph, start, end, dist, prev);

    // Traverse the graph to find all shortest paths to E
    std::queue<Vertex> q;
    std::unordered_set<Vertex> visited;
    // Find end direction
    Direction end_dir;
    for (auto &[v, prev_v] : prev) {
        if (v.coord == end) {
            end_dir = v.dir;
            break;
        }
    }
    q.push({end, end_dir});

    while (!q.empty()) {
        Vertex current = q.front();
        q.pop();

        if (visited.find(current) != visited.end()) {
            continue;
        }
        visited.insert(current);

        for (auto &prev_v : prev[current]) {
            q.push(prev_v);
        }
    }

    int count = 0;
    for (int i = 0; i < grid.size(); i++) {
        for (int j = 0; j < grid[i].size(); j++) {
            // If any vertex in visited includes the coordinate, print it
            bool found = false;
            for (auto &v : visited) {
                if (v.coord.x == j && v.coord.y == i) {
                    count++;
                    found = true;
                    break;
                }
            }
        }
    }

    cout << count << endl;

    return 0;
}

struct hashCoordDirPair {
    size_t operator()(const std::pair<Coordinate, Direction> &p) const {
        auto h1 = std::hash<Coordinate>{}(p.first);
        auto h2 = std::hash<int>{}(p.second);
        return h1 ^ (h2 << 1);
    }
};

int main() {
    std::ifstream file("../d16/input.txt");
    std::vector<std::vector<char>> grid;
    std::unordered_map<Vertex, std::vector<Vertex>> graph;

    std::string line;
    while (std::getline(file, line)) {
        std::vector<char> row;
        for (char c : line) {
            row.push_back(c);
        }
        grid.push_back(row);
    }

    std::vector<Direction> directions = {N, E, S, W};
    Vertex start = {{0, 0}, E};
    Coordinate end = {0, 0};
    for (int i = 1; i < grid.size()-1; i++) {
        for (int j = 1; j < grid[i].size()-1; j++) {
            for (Direction dir : directions) {
                Vertex current = {{j, i}, dir};
                if (grid[i][j] == 'S') {
                    start = current;
                } else if (grid[i][j] == 'E') {
                    end = {j, i};
                }

                // Add edge to current direction if it is not a wall
                switch (dir) {
                    case N:
                        if (grid[i-1][j] != '#') {
                            graph[current].push_back({{j, i-1}, N});
                        }
                        graph[current].push_back({{j, i}, E});
                        graph[current].push_back({{j, i}, S});
                        graph[current].push_back({{j, i}, W});
                        break;
                    case E:
                        if (grid[i][j+1] != '#') {
                            graph[current].push_back({{j+1, i}, E});
                        }
                        graph[current].push_back({{j, i}, N});
                        graph[current].push_back({{j, i}, S});
                        graph[current].push_back({{j, i}, W});
                        break;
                    case S:
                        if (grid[i+1][j] != '#') {
                            graph[current].push_back({{j, i+1}, S});
                        }
                        graph[current].push_back({{j, i}, E});
                        graph[current].push_back({{j, i}, N});
                        graph[current].push_back({{j, i}, W});
                        break;
                    case W:
                        if (grid[i][j-1] != '#') {
                            graph[current].push_back({{j-1, i}, W});
                        }
                        graph[current].push_back({{j, i}, E});
                        graph[current].push_back({{j, i}, S});
                        graph[current].push_back({{j, i}, N});
                        break;
                }
            }
        }
    }

    part1(graph, start, end, grid);
    part2(graph, start, end, grid);

    return 0;
}