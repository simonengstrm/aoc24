#include <fstream>
#include <iostream>
#include <vector>

using std::string, std::cout, std::endl;

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
    int score = 0;
    int i = 0;
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
    cout << "Part 1: " << part1(input) << endl;
    return 0;
}

