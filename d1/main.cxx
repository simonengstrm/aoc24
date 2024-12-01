#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <map>

using namespace std;

int part1() {
    ifstream file("../d1/input.txt");

    priority_queue<int, std::vector<int>, std::greater<int>> l1;
    priority_queue<int, std::vector<int>, std::greater<int>> l2;

    string line;
    while(getline(file, line)) {
        // Read tokens into a stringstream
        stringstream ss;
        ss << line;
        string n1, n2;
        ss >> n1 >> n2;
        l1.push(stoi(n1));
        l2.push(stoi(n2));
    }

    int sum = 0;
    while (!l1.empty()) {
        // Pop from both, compare and add to sum
        int n1 = l1.top(); l1.pop();
        int n2 = l2.top(); l2.pop();
        //cout << n1 << "-" << n2 << endl;
        sum += abs(n1-n2);
    }

    cout << "Distance sum: " << sum << endl;

    return 0;
}

int part2() {
    ifstream file("../d1/input.txt");

    vector<int> l1;
    map<int, int> map;

    string line;
    while (getline(file, line)) {
        stringstream ss;
        ss << line;
        string n1, n2;
        ss >> n1 >> n2;
        l1.push_back(stoi(n1));
        map[stoi(n2)]++;
    }
    
    int similarity_score = 0;
    for (auto n : l1) {
        int occ = map[n];
        similarity_score += n * occ;
    }

    cout << "Similarity score: " << similarity_score << endl;

    return 0;
}
int main() {    
    part1();
    part2();
    return 0;
}