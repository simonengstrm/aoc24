#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

using std::cout, std::endl, std::string, std::vector;

struct hashPair {
    template <class T1, class T2>
    size_t operator()(const std::pair<T1, T2> &p) const {
        auto h1 = std::hash<T1>{}(p.first);
        auto h2 = std::hash<T2>{}(p.second);

        return h1 ^ h2;
    }
};

struct Robot {
    int x, y;
    int dx, dy;
};

int part2(vector<Robot> robots) {
    int gridx = 101;
    int gridy = 103;

    for (int i = 1; i < 10000; i++) { // 10000 is ish lcm of all cycles
        for (auto &r : robots) {
            r.x = (r.x + r.dx) % gridx;
            r.y = (r.y + r.dy) % gridy; 
            if (r.x < 0) {
                r.x += gridx;
            }
            if (r.y < 0) {
                r.y += gridy;
            }
        }

        //Print grid
        // cout << "Time: " << i << endl;
        // for (int y = 0; y < gridy; y++) {
        //     for (int x = 0; x < gridx; x++) {
        //         bool found = false;
        //         for (auto &r : robots) {
        //             if (r.x == x && r.y == y) {
        //                 cout << "#";
        //                 found = true;
        //                 break;
        //             }
        //         }
        //         if (!found) {
        //             cout << " ";
        //         }
        //     }
        //     cout << endl;
        // }
    }

    return 0;
}

int part1(vector<Robot> robots) {
    int dt = 100;
    // int gridx = 11;
    // int gridy = 7;
    int gridx = 101;
    int gridy = 103;

    for (auto &r : robots) {
        r.x += dt * r.dx;
        r.y += dt * r.dy;

        // Modulo positions into the grid
        r.x = ((r.x % gridx) + gridx) % gridx;
        r.y = ((r.y % gridy) + gridy) % gridy;
    }

    // Count number of robots in each quadrant, disregard the middle
    int mx = gridx / 2;
    int my = gridy / 2;
    int q1 = 0, q2 = 0, q3 = 0, q4 = 0;
    for (auto &r : robots) {
        if (r.x < mx && r.y < my) {
            q1 += 1;
        } else if (r.x < mx && r.y > my) {
            q2 += 1;
        } else if (r.x > mx && r.y < my) {
            q3 += 1;
        } else if (r.x > mx && r.y > my) {
            q4 += 1;
        }
    }

    int safety_factor = q1 * q2 * q3 * q4;

    cout << "Safety factor: " << safety_factor << endl;

    return 0;
}

int main() {
    std::ifstream file("../d14/input.txt");
    string line;

    vector<Robot> robots;
    while (std::getline(file, line)) {
        Robot robot{};
        // parse line
        // p=98,44 v=81,-75 
        sscanf(line.c_str(), "p=%d,%d v=%d,%d", &robot.x, &robot.y, &robot.dx, &robot.dy);
        robots.push_back(robot);
    }

    part1(robots);
    part2(robots);
}