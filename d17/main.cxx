#include <chrono>
#include <cstdint>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_set>
#include <vector>
#include <sstream>
#include <cmath>
#include <algorithm>

using std::cout, std::endl;

enum Op {
    ADV, // 0
    BXL, // 1
    BST, // 2
    JNZ, // 3
    BXC, // 4
    OUT, // 5
    BDV, // 6
    CDV  // 7
};

std::string opToString(Op op) {
    switch (op) {
        case ADV:
            return "ADV";
        case BXL:
            return "BXL";
        case BST:
            return "BST";
        case JNZ:
            return "JNZ";
        case BXC:
            return "BXC";
        case OUT:
            return "OUT";
        case BDV:
            return "BDV";
        case CDV:
            return "CDV";
        default:
            return "Invalid";
    }
}

struct Ins {
    Op op;
    int64_t operand;
};

int64_t toCombo(int64_t A, int64_t B, int64_t C, int64_t operand) {
    if (operand < 4) {
        return operand; // Literal
    } else if (operand == 4) {
        return A;
    } else if (operand == 5) {
        return B;
    } else if (operand == 6) {
        return C;
    } else {
        return -1;
    }
}

int64_t adv(int64_t A, int64_t B, int64_t C, int64_t operand) {
    int64_t numerator = A;
    int64_t combo = toCombo(A, B, C, operand);
    // denom is 2^combo
    int64_t denom = 1;
    for (int64_t i = 0; i < combo; i++) {
        denom *= 2;
    }
    return A / denom;
}

int64_t bxl(int64_t A, int64_t B, int64_t C, int64_t operand) {
    return B ^ operand; // Bitwise XOR
}

int64_t bst(int64_t A, int64_t B, int64_t C, int64_t operand) {
    int64_t combo = toCombo(A, B, C, operand);
    return combo % 8;
}

int64_t bxc (int64_t A, int64_t B, int64_t C, int64_t operand) {
    return B ^ C; // Bitwise AND
}

int64_t out (int64_t A, int64_t B, int64_t C, int64_t operand) {
    return toCombo(A, B, C, operand) % 8;
}

int64_t bdv (int64_t A, int64_t B, int64_t C, int64_t operand) {
    int64_t numerator = A;
    int64_t combo = toCombo(A, B, C, operand);
    // denom is 2^combo
    int64_t denom = 1;
    for (int64_t i = 0; i < combo; i++) {
        denom *= 2;
    }
    return A / denom;
}

int64_t cdv (int64_t A, int64_t B, int64_t C, int64_t operand) {
    int64_t numerator = A;
    int64_t combo = toCombo(A, B, C, operand);
    // denom is 2^combo
    int64_t denom = 1;
    for (int64_t i = 0; i < combo; i++) {
        denom *= 2;
    }
    return A / denom;
}

std::vector<int64_t> runProgram(int64_t A, int64_t B, int64_t C, std::vector<Ins> program) {
    std::vector<int64_t> output;
    for (int64_t i = 0; i < program.size(); i++) {
        int64_t res = 0;
        switch (program[i].op) {
            case ADV:
                res = adv(A, B, C, program[i].operand);
                A = res;
                break;
            case BXL:
                res = bxl(A, B, C, program[i].operand);
                B = res;
                break;
            case BST:
                res = bst(A, B, C, program[i].operand);
                B = res;
                break;
            case JNZ:
                if (A == 0) {
                    continue;
                } else {
                    i = program[i].operand - 1;
                }
                break;
            case BXC:
                res = bxc(A, B, C, program[i].operand);
                B = res;
                break;
            case OUT:
                res = out(A, B, C, program[i].operand);
                output.push_back(res);
                break;
            case BDV:
                res = bdv(A, B, C, program[i].operand);
                B = res;
                break;
            case CDV:
                res = cdv(A, B, C, program[i].operand);
                C = res;
                break;
            default:
                cout << "Invalid instruction" << endl;
                break;
        }
    }
    return output;
}

int64_t part1(int64_t A, int64_t B, int64_t C, std::vector<Ins> program) {
    std::vector<int64_t> output = runProgram(A, B, C, program);
    cout << "Output: ";
    for (auto i : output) {
        cout << i << ", ";
    }
    cout << endl;
    return 0;
}

// Recursive function that starts by setting the correct value for the last digit, then goes to the left until it finds the right value
// or it needs to backtrack
bool findDigit(int64_t A, std::vector<Ins> program, std::vector<int64_t> original_program, int64_t dig_index) {
    if (dig_index == -1) {
        cout << endl;
        cout << "Found all digits" << endl;
        cout << "A: " << A << endl;
        return true;
    }

    int i = 0;
    while (i < 8) {
        int64_t res = runProgram(A + i*pow(8, dig_index), 0, 0, program)[dig_index];

        if (res == original_program[dig_index]) {
            if (findDigit(A + i*pow(8, dig_index), program, original_program, dig_index-1)) {
                return true;
            }
        }
        i++;
    }
    return false;
}

int64_t part2(int64_t A, int64_t B, int64_t C, std::vector<Ins> program, std::vector<int64_t> original_program) {
    // The minimum value that will make the program output the right number of digits is 8^15, the max is 8^16
    // We initialize A to 8^15 

    // By incrementing A by 8^X where X is the character in the output we want to set, we only affect the output of that character
    // and all the ones to the left of it, the right ones are not affected

    // To find the right value we need to start by setting the last character by finding the that value of A (A^15 + i*8^14)
    // Then we keep going until we are either done or we need to backtrack cause we cannot find the correct value

    findDigit(pow(8,15), program, original_program, 15);

    return 0;
}

int main() {
    std::ifstream file("../d17/input.txt");
    int64_t A;
    int64_t B;
    int64_t C;

    std::vector<Ins> program;
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) {
            break;
        }

        std::string reg;
        // Register A: 729
        std::stringstream ss(line);
        ss >> reg;
        if (reg == "Register") {
            ss >> reg;
            if (reg == "A:") {
                ss >> A;
            } else if (reg == "B:") {
                ss >> B;
            } else if (reg == "C:") {
                ss >> C;
            }
        }

        int64_t val = 0;
        std::string token;
        while (ss >> token) {
            if (token == ":") {
                continue;
            }
            val = std::stoi(token);
        }
    }

    std::getline(file, line);
    line = line.erase(0, 9); // Remove "Program: "
    std::vector<int64_t> original_program;
    std::stringstream ss(line);
    std::string token;
    // Get every 2 items from the list and create an instruction (delim ,)
    while (std::getline(ss, token, ',')) {
        Op op = static_cast<Op>(std::stoi(token));
        std::getline(ss, token, ',');
        int64_t operand = static_cast<int64_t>(std::stoi(token));
        program.push_back({op, operand});
        original_program.push_back(op);
        original_program.push_back(operand);
    }

    // cout << "A: " << A << endl;
    // cout << "B: " << B << endl;
    // cout << "C: " << C << endl;

    // cout << "Program: " << endl;
    // for (auto i : program) {
    //     cout << i.op << ", " << i.operand << endl;
    // }

    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    part1(A, B, C, program);
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    part2(A, B, C, program, original_program);
    std::chrono::steady_clock::time_point end2 = std::chrono::steady_clock::now();
    cout << "p1 = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[µs]" << endl;
    cout << "p2 = " << std::chrono::duration_cast<std::chrono::microseconds>(end2 - end).count() << "[µs]" << endl;

    return 0;
}