#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#define MAX_LINE_LENGTH 4096

int getFirstDigit(const std::string &line) {
    for (int i = 0; i < line.size(); i++) {
        int dc = (int)line[i] - 48;
        if (dc >= 0 && dc < 10) return dc;
    }
    return -1;
}

int getLastDigit(const std::string &line) {
    for (int i = line.size(); i >= 0; i--) {
        int dc = (int)line[i] - 48;
        if (dc >= 0 && dc < 10) return dc;
    }
    return -1;
}

int getDigit(const std::string &line) {
    return 10*getFirstDigit(line) + getLastDigit(line);
}

// Parse the input string. Storing calibrated values in private 
// variable vals
void ParseCalibration(const std::string &inpt, std::vector<int> &vals) {
    // Open file
    std::ifstream file;
    file.open(inpt.c_str(), std::ios::out);
    vals.clear();

    if (!file.is_open()) {
        std::cout << "Could not open file.\n";
        return;
    }

    // Read lines
    std::string line;
    while(file.good()) {
        std::getline(file, line);
        vals.push_back(getDigit(line));
    }

    file.close();
}

void PrintCalibration(const std::vector<int> &vals) {
    int sum = 0;
    for (const int & v : vals) {
        std::cout << v << std::endl;
        sum += v;
    }
    std::cout << "Sum: " << sum << std::endl;
}

int main(int argc, char *argv[]) {
    std::string input = "";
    std::vector<int> vals;

    if (argc < 2) {
        std::cout << "Not enough arguments were provided.\n";
        std::cout << "Usage: ./a.x <input_file>.\n";
        return -1;
    } else {
        input = argv[1];
    }

    ParseCalibration(input, vals);
    PrintCalibration(vals);

    return 0;
}