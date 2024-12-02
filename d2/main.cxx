#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

using namespace std;

bool check_safe(const vector<int> &report) {
    int init_diff = signbit(report[0] - report[1]);
    for (int i = 0; i < report.size()-1; i++) {
        int diff = abs(report[i] - report[i+1]);

        if (!(diff >= 1 && diff <= 3)) {
            return false;
        }

        if (signbit(report[i] - report[i+1]) != init_diff) {
            return false;
        }
    }
    return true;
}

int part2() {
    ifstream file("../d2/input.txt");
    
    vector<vector<int>> reports;

    string line;
    while (getline(file, line)) {
        vector<int> report;
        stringstream ss(line);
        
        int n;
        while (ss >> n) {
            report.push_back(n);
        }

        reports.push_back(report);
    }
    int safe_reports = 0;

    for (auto report : reports) {
        // If we are here then the report is safe
        if (check_safe(report)) {
            safe_reports++;
        } else {
            // Check if removal of some level results in safe report
            for (int remove = 0; remove < report.size(); remove++) {
                // Clone the report and remove removal index
                vector<int> reportClone(report);
                reportClone.erase(reportClone.begin() + remove);
                if (check_safe(reportClone)) {
                    safe_reports++;
                    break;
                }
            }
        }
    }

    cout << safe_reports << endl;
    return 0;
}

int part1() {
    ifstream file("../d2/input.txt");
    
    vector<vector<int>> reports;

    string line;
    while (getline(file, line)) {
        vector<int> report;
        stringstream ss(line);
        
        int n;
        while (ss >> n) {
            report.push_back(n);
        }

        reports.push_back(report);
    }
    int safe_reports = 0;

    for (auto report : reports) {
        if (check_safe(report)) {
            safe_reports++;
        }
    }
    cout << safe_reports << endl;
    return 0;
}

int main() {
    part1();
    part2();
    return 0;
}