#include <iostream>
#include <regex>

using namespace std;

constexpr const char input[] {
    #embed "../d3/input.txt"
};

int part1() {
    int sum = 0;
    string line = string(input);
    std::regex reg("mul\\((\\d{1,3}),(\\d{1,3})\\)");
    std::smatch match;

    while (std::regex_search(line, match, reg)) {
        int n1 = stoi(match[1]);
        int n2 = stoi(match[2]);
        sum += n1 * n2;

        line.erase(0, match.position() + match.length());
    }

    cout << sum << endl;
    return 0;
}

int part2() {
    string line = string(input);
    int sum = 0;
    bool enabled = true;

    std::regex reg("don't\\(\\)|do\\(\\)|mul\\((\\d{1,3}),(\\d{1,3})\\)");
    std::smatch match;
    while (std::regex_search(line, match, reg)) {
        if (match[0].str() == "do()") {
            enabled = true;
        } else if (match[0].str() == "don't()") {
            enabled = false;
        } else {
            if (enabled) {
                sum += stoi(match[1]) * stoi(match[2]);
            }
        }
        line = match.suffix().str();
    }

    cout << sum << endl;
    return 0;
}

int main() {
    part1();
    part2();
    return 0;
}