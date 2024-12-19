#include <iostream>
#include <fstream>
#include <string>
#include <unordered_set>
#include <vector>

using std::cout, std::endl;

int64_t can_construct(std::unordered_set<std::string> &choices, std::string design) {
    std::vector<int64_t> dp(design.size() + 1, 0);
    dp[0] = 1;
    
    for (size_t i = 0; i < design.size(); i++) {
        if (dp[i] > 0) {
            for (size_t len = 1; len <= design.size()-i; len++) {
                std::string sub = design.substr(i, len);
                if (choices.find(sub) != choices.end()) {
                    dp[i + len] += dp[i];
                }
            }
        }
    }

    return dp[design.size()];
}

int part2(std::unordered_set<std::string> choices, std::vector<std::string> designs) {
    // Check how we can construct design[i] using choices, count every design that is possible
    int64_t possible = 0;
    for (auto &d : designs) {
        possible += can_construct(choices, d);
    }

    cout << "Part 2: " << possible << endl;
    return 0;
}

int part1(std::unordered_set<std::string> choices, std::vector<std::string> designs) {
    // Check how we can construct design[i] using choices, count every design that is possible
    int64_t possible = 0;
    for (auto &d : designs) {
        if (can_construct(choices, d)) {
            possible++;
        }
    }

    cout << "Part 1: " << possible << endl;
    return 0;
}

int main() {
    std::ifstream file("../d19/input.txt");
    std::string line;
    std::unordered_set<std::string> choices;
    std::vector<std::string> designs;

    // First line is comma separated
    std::getline(file, line);
    while(line.find(',') != std::string::npos) {
        std::string choice = line.substr(0, line.find(','));
        choices.insert(choice);
        line = line.substr(line.find(',') + 2);
    }

    // Get the last one
    choices.insert(line);

    // Remove empty line
    std::getline(file, line);

    while (std::getline(file, line)) {
        designs.push_back(line);
    }

    part1(choices, designs);
    part2(choices, designs);
}