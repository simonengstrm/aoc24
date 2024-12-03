#include <fstream>
#include <iostream>
#include <regex>


using namespace std;

int part1() {
    ifstream file("../d3/input.txt");

    int sum = 0;
    string line;
    while(getline(file, line)) {
        std::regex reg("mul\\((\\d{1,3}),(\\d{1,3})\\)");
        std::smatch match;

        while (std::regex_search(line, match, reg)) {
            int n1 = stoi(match[1]);
            int n2 = stoi(match[2]);
            sum += n1 * n2;

            line.erase(0, match.position() + match.length());
        }
    }
    cout << sum << endl;
    return 0;
}

int part2() {
    ifstream file("../d3/input.txt");
    
    int sum = 0;
    string line;
    bool enabled = true;
    while(getline(file, line)) {
        // Match only mul(X,Y)
        std::regex doreg("do\\(\\)");
        std::regex dontreg("don't\\(\\)");
        std::regex reg("mul\\((\\d{1,3}),(\\d{1,3})\\)");
        std::smatch domatch;
        std::smatch dontmatch;
        std::smatch regmatch;

        while (std::regex_search(line, regmatch, reg)) {
            // Go through do and donts in the substring
            string substr = line.substr(0, regmatch.position());
            //cout << "Searching dos and donts in " << substr << endl;
            int latestdo = -1, latestdont = -1;
            while (std::regex_search(substr, domatch, doreg)) {
                //cout << "found do at " << domatch.position() << endl;
                latestdo = domatch.position();
                substr.erase(0, domatch.position()+ domatch.length());
            }
            substr = line.substr(0, regmatch.position());
            while (std::regex_search(substr, dontmatch, dontreg)) {
                //cout << "found dont at " << dontmatch.position() << endl;
                latestdont = dontmatch.position();
                substr.erase(0, dontmatch.position()+ dontmatch.length());
            }
            
            //cout << "Do: " << latestdo << ", Dont: " << latestdont << endl;
            if (latestdont == -1 && latestdo == -1) {

            } else {
                if (latestdont > latestdo) {
                    enabled = false;
                } else {
                    enabled = true;
                }
            }


            if (enabled) {
                int n1 = stoi(regmatch[1]);
                int n2 = stoi(regmatch[2]);
                //cout << "multiplying " << n1 << " " << n2 << endl;
                sum += n1 * n2;
            }
            line.erase(0, regmatch.position() + regmatch.length());

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