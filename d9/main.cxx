#include <fstream>
#include <iostream>
#include <vector>

using std::string, std::cout, std::endl;

struct Block {
    int id; // -1 for free
    int size;
};

int part2(string input) {
    std::vector<Block> disk;
    int id = 0;
    for (int i = 0; i < input.size(); i++) {
        if (i % 2 == 0) {
            // Put input[i] amount of id into disk
            disk.push_back({id, input[i] - '0'});
            id++;
        } else {
            disk.push_back({-1, input[i] - '0'});
        }
    }

    for (int hi = disk.size()-1; hi >= 0; hi--) {
        // Try to move disk[hi] to a free block
        if (disk[hi].id == -1) {
            continue;
        }

        // Try to find a free block large enought to move disk[hi] to
        int lo = 0;
        while (lo < hi) {
            if (disk[lo].id != -1) {
                lo++;
                continue;
            } else if (disk[hi].size <= disk[lo].size) {
                break;
            }
            lo++;
        }

        if (lo == hi) {
            continue;
        }

        int diff = disk[lo].size - disk[hi].size;
        disk[lo].id = disk[hi].id;
        disk[lo].size = disk[hi].size;
        disk[hi].id = -1;
        if (diff > 0) {
            disk.insert(disk.begin() + lo + 1, {-1, diff});
        }

    }

    long score = 0;
    long i = 0;
    for (auto [id, size] : disk) {
        if (id == -1) {
            i += size;
            continue;
        }
        for (int j = 0; j < size; j++) {
            score += id * i;
            i++;
        }
    }

    cout << "Score: " << score << endl;

    return 0;
}

int part1(string input) {
    std::vector<int> disk; // -1 empty
    int id = 0;
    for (int i = 0; i < input.size(); i++) {
        if (i % 2 == 0) {
            // Put input[i] amount of id into disk
            for (int j = 0; j < input[i] - '0'; j++) {
                disk.push_back(id);
            }
            id++;
        } else {
            for (int j = 0; j < input[i] - '0'; j++) {
                disk.push_back(-1);
            }
        }
    }

    int lo = 0;
    int hi = disk.size()-1;
    while (lo < hi) {
        if (disk[lo] != -1) {
            lo++;
            continue;
        }
        if (disk[hi] == -1) {
            hi--;
            continue;
        }

        disk[lo] = disk[hi];
        disk[hi] = -1;
        lo++;
        hi--;
    }

    // Calculate score
    long score = 0;
    long i = 0;
    while (disk[i] != -1) {
        score += disk[i] * i;
        i++;
    }

    cout << "Score: " << score << endl;

    return 0;
}

int main() {
    std::ifstream file("../d9/input.txt");
    string input;
    getline(file, input);
    part1(input);
    part2(input);
    return 0;
}

