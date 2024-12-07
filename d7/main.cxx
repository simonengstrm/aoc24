#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <sstream>

using std::cout, std::endl, std::string;

enum Operator {
    MUL,
    ADD,
    OR
};

void generatePermutationsp1(std::vector<std::vector<Operator>> &perms, int size) {
    std::vector<Operator> perm(size);
    for (int i = 0; i < size; i++) {
        perm[i] = MUL;
    }
    perms.push_back(perm);
    while (true) {
        int i = size - 1;
        while (i >= 0 && perm[i] == ADD) {
            i--;
        }
        if (i < 0) {
            break;
        }
        perm[i] = static_cast<Operator>(perm[i] + 1);
        for (int j = i + 1; j < size; j++) {
            perm[j] = MUL;
        }
        perms.push_back(perm);
    }
}

void generatePermutationsp2(std::vector<std::vector<Operator>> &perms, int size) {
    std::vector<Operator> perm(size);
    for (int i = 0; i < size; i++) {
        perm[i] = MUL;
    }
    perms.push_back(perm);
    while (true) {
        int i = size - 1;
        while (i >= 0 && perm[i] == OR) {
            i--;
        }
        if (i < 0) {
            break;
        }
        perm[i] = static_cast<Operator>(perm[i] + 1);
        for (int j = i + 1; j < size; j++) {
            perm[j] = MUL;
        }
        perms.push_back(perm);
    }
}

int part1() {
    std::ifstream file("../d7/input.txt");
    string line;

    std::map<long, std::vector<long>> cals; // Cals is short for calibration guys, im just using slang

    while (std::getline(file, line)) {
        //cout << line << endl;
        long result = stol(line.substr(0, line.find(':')));
        string rest = line.substr(line.find(':') + 2);
        std::istringstream iss(rest);
        string token;
        while (iss >> token) {
            cals[result].push_back(stol(token));
        }
    }

    long sum = 0;

    for (auto [target, values] : cals) {
        std::vector<std::vector<Operator>> perms;
        generatePermutationsp1(perms, values.size() - 1);
        for (auto &perm : perms) {
            long result = values[0];
            for (int i = 0; i < values.size() - 1; i++) {
                if (perm[i] == MUL) {
                    result *= values[i + 1];
                } else {
                    result += values[i + 1];
                }
            }

            if (result == target) {
                sum += target;
                break;
            }
        }
    }
    cout << sum << endl;
    return 0;
}

int part2() {
    std::ifstream file("../d7/input.txt");
    string line;

    std::map<long, std::vector<long>> cals; // Cals is short for calibration guys, im just using slang

    while (std::getline(file, line)) {
        //cout << line << endl;
        long result = stol(line.substr(0, line.find(':')));
        string rest = line.substr(line.find(':') + 2);
        std::istringstream iss(rest);
        string token;
        while (iss >> token) {
            cals[result].push_back(stol(token));
        }
    }

    long sum = 0;

    for (auto [target, values] : cals) {
        std::vector<std::vector<Operator>> perms;
        generatePermutationsp2(perms, values.size() - 1);
        for (auto &perm : perms) {
            long result = values[0];
            for (int i = 0; i < values.size() - 1; i++) {
                if (perm[i] == MUL) {
                    result *= values[i + 1];
                } else if (perm[i] == ADD) {
                    result += values[i + 1];
                } else if (perm[i] == OR) {
                    string num1 = std::to_string(result);
                    string num2 = std::to_string(values[i + 1]);
                    string combined = num1 + num2;
                    result = stol(combined);
                }
            }

            if (result == target) {
                sum += target;
                break;
            }
        }
    }
    cout << sum << endl;
    return 0;
}

int main() {
    part1();
    part2();
    return 0;
}