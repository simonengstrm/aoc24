#include <chrono>
#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <map>

using std::cout, std::endl, std::ifstream, std::string, std::vector;

long part1(std::unordered_map<long, long> stoneDict) {
    for (int i = 0; i < 25; i++) {
        std::unordered_map<long, long> newStoneDict;
        for (auto &[stone, count]: stoneDict) {
            if (stone == 0) {
                newStoneDict[1] += count;
            } else if (std::to_string(stone).length() % 2 == 0) {
                string stoneStr = std::to_string(stone);
                int half = stoneStr.length() / 2;
                //cout << "Stone " << stone << " split at iteration " << i << endl;
                long firstHalf = std::stol(stoneStr.substr(0, half));
                long secondHalf = std::stol(stoneStr.substr(half, stoneStr.length()));
                newStoneDict[firstHalf] += count;
                newStoneDict[secondHalf] += count;
            } else {
                newStoneDict[stone * 2024] += count;
            }
        }
        stoneDict = newStoneDict;
    }

    long total = 0;
    for (auto &[_, count] : stoneDict) {
        total += count;
    }

    return total;
}

long part2(std::unordered_map<long, long> stoneDict) {
    for (int i = 0; i < 75; i++) {
        std::unordered_map<long, long> newStoneDict;
        for (auto &[stone, count] : stoneDict) {
            if (stone == 0) {
                newStoneDict[1] += count;
            } else if (std::to_string(stone).length() % 2 == 0) {
                string stoneStr = std::to_string(stone);
                int half = stoneStr.length() / 2;
                //cout << "Stone " << stone << " split at iteration " << i << endl;
                long firstHalf = std::stol(stoneStr.substr(0, half));
                long secondHalf = std::stol(stoneStr.substr(half, stoneStr.length()));
                newStoneDict[firstHalf] += count;
                newStoneDict[secondHalf] += count;
            } else {
                newStoneDict[stone * 2024] += count;
            }
        }
        stoneDict = newStoneDict;
    }

    long total = 0;
    for (auto &[_, count] : stoneDict) {
        total += count;
    }
    //cout << "Size of stoneDict/: " << stoneDict.size() << endl;

    //cout << total << endl;
    return total;
}

int main() {
    ifstream file("../d11/input.txt");
    string line;
    std::unordered_map<long, long> stoneDict;
    while (getline(file, line)) {
        std::stringstream ss(line);
        while (ss >> line) {
            stoneDict[std::stol(line)] += 1;
        }
    }

    // for (const auto &stone : stones) {
    //     cout << stone << " ";
    // }
    // cout << endl;
    std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
    long p1 = part1(stoneDict);
    std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
    long p2 = part2(stoneDict);
    std::chrono::high_resolution_clock::time_point t3 = std::chrono::high_resolution_clock::now();

    auto duration1 = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
    auto duration2 = std::chrono::duration_cast<std::chrono::microseconds>(t3 - t2).count();

    cout << "Part 1: " << p1 << " (" << duration1 << "us)" << endl;
    cout << "Part 2: " << p2 << " (" << duration2 << "us)" << endl;

    return 0;
}