#include <cstdint>
#include <iostream>
#include <fstream>
#include <string>
#include <utility>
#include <vector>

using std::cout, std::endl, std::ifstream;

struct Machine {
    std::pair<uint64_t, uint64_t> A;
    std::pair<uint64_t, uint64_t> B;
    std::pair<uint64_t, uint64_t> prize;
};

uint64_t part1(std::vector<Machine>& machines) {
    // Figure out how to get exactly to the prize by pressing B the least amount of times
    // No more presses than 100
    // Equation: aP * A.first + bP * B.first = prize.first
    //          aP * A.second + bP * B.second = prize.second
    uint64_t tokens = 0;
    for (auto &m : machines) {
        // Solve equation using cramers rule
        int64_t det = m.A.first * m.B.second - m.A.second * m.B.first;
        int64_t detA = m.prize.first * m.B.second - m.prize.second * m.B.first;
        int64_t detB = m.A.first * m.prize.second - m.A.second * m.prize.first;

        int64_t aP = detA / det;
        int64_t bP = detB / det;
        
        if (aP * m.A.first + bP * m.B.first == m.prize.first &&
            aP * m.A.second + bP * m.B.second == m.prize.second) {
            tokens += aP*3 + bP;
            continue;
        }
    }

    cout << "Tokens: " << tokens << endl;
    return 0;
}

uint64_t part2(std::vector<Machine>& machines) {
    uint64_t tokens = 0;
    // Equation: aP * A.first + bP * B.first = prize.first
    //          aP * A.second + bP * B.second = prize.second

    for (auto &m : machines) {
        m.prize.first += 10000000000000;
        m.prize.second += 10000000000000;
        // Solve equation using cramers rule
        int64_t det = m.A.first * m.B.second - m.A.second * m.B.first;
        int64_t detA = m.prize.first * m.B.second - m.prize.second * m.B.first;
        int64_t detB = m.A.first * m.prize.second - m.A.second * m.prize.first;

        int64_t aP = detA / det;
        int64_t bP = detB / det;
        
        if (aP * m.A.first + bP * m.B.first == m.prize.first &&
            aP * m.A.second + bP * m.B.second == m.prize.second) {
            tokens += aP*3 + bP;
            continue;
        }
    }

    cout << "Tokens: " << tokens << endl;

    return 0;
}

int main() {
    ifstream file("../d13/input.txt");
    std::string line;

    std::vector<Machine> machines;

    Machine m{};
    while (getline(file, line)) {
        if (line.empty()) {
            machines.push_back(m);
            m = Machine{};
            continue;
        }

        if (line.find("A") != std::string::npos) {
            int x, y;
            sscanf(line.c_str(), "Button A: X+%d, Y+%d", &x, &y);
            m.A = {x, y};
        } else if (line.find("B") != std::string::npos) {
            int x, y;
            sscanf(line.c_str(), "Button B: X+%d, Y+%d", &x, &y);
            m.B = {x, y};
        } else {
            uint64_t x, y;
            sscanf(line.c_str(), "Prize: X=%lld, Y=%lld", &x, &y);
            m.prize = {x, y};
        }
    }
    machines.push_back(m);

    part1(machines);
    part2(machines);

}