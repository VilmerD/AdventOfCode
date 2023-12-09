#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <map>

#define MAX_LINE_LENGTH 4096

const std::map<std::string, int> CharToNumber = {
    {"zero", 0},
    {"one", 1},
    {"two", 2},
    {"three", 3},
    {"four", 4},
    {"five", 5},
    {"six", 6},
    {"seven", 7},
    {"eight", 8},
    {"nine", 9}};

#define MAX_STRINGIZED_NUMBER_LENGTH 5
#define ccmp(a, b) ((int)a - (int)b) == 0

inline int char2Number(char c)
{
    int dc = (int)c - 48;
    return (dc >= 0 && dc < 10) ? dc : -1;
}

int getFirstDigit(const std::string &line)
{
    for (int i = 0; i < line.size(); ++i)
    {
        // std::cout << "\tAt " << line[i] << std::endl;

        // Check if current character is a number
        int dc = char2Number(line[i]);
        if (dc != -1)
            return dc;

        // Compare with keys
        for (auto &&[key, val] : CharToNumber)
        {
            int L = key.size();

            // Loop over size of key
            for (int j = 0; j < L && i + j < line.size(); ++j)
            {
                bool match = ccmp(key[j], line[i + j]);
                if (!match) break;
                else if (j == L - 1) return val;
            }
        }
    }
    return -1;
}

int getLastDigit(const std::string &line)
{
    for (int i = line.size() - 1; i >= 0; --i)
    {
        std::cout << "\tAt " << line[i] << std::endl;

        // Check if current character is a number
        int dc = char2Number(line[i]);
        if (dc != -1)
            return dc;

        // Compare with keys
        for (auto &&[key, val] : CharToNumber)
        {
            int L = key.size();

            // Loop over size of key
            // std::cout << "\tKey: " << key << "(" << L << ")" << std::endl;
            for (int j = 0; j < L && i - j >= 0; ++j)
            {
                std::cout << key[L - j - 1] << ", " << line[i - j] << std::endl;
                bool match = ccmp(key[L - j - 1], line[i - j]);
                if (!match)
                {
                    // If character doesnt match stop
                    // std::cout << "\t\tno match (" << j << ")" << std::endl;
                    break;
                }
                else if (j == L - 1)
                {
                    // Else if full match return true and value
                    return val;
                }
            }
        }
    }
    return -1;
}

int getDigit(const std::string &line)
{
    // std::cout << "Checking line: " << line << std::endl;
    int res = 10 * getFirstDigit(line) + getLastDigit(line);
    // std::cout << "Result: " << res << std::endl;
    return res;
}

// Parse the input string. Storing calibrated values in private
// variable vals
void ParseCalibration(const std::string &inpt, std::vector<int> &vals)
{
    // Open file
    std::ifstream file;
    file.open(inpt.c_str(), std::ios::out);
    vals.clear();

    if (!file.is_open())
    {
        std::cout << "Could not open file.\n";
        return;
    }

    // Read lines
    std::string line;
    while (file.good())
    {
        std::getline(file, line);
        vals.push_back(getDigit(line));
    }

    file.close();
}

void PrintCalibration(const std::vector<int> &vals)
{
    int sum = 0;
    for (const int &v : vals)
    {
        std::cout << v << std::endl;
        sum += v;
    }
    std::cout << "Sum: " << sum << std::endl;
}

int main(int argc, char *argv[])
{
    std::string input = "";
    std::vector<int> vals;

    if (argc < 2)
    {
        std::cout << "Not enough arguments were provided.\n";
        std::cout << "Usage: ./a.x <input_file>.\n";
        return -1;
    }
    else
    {
        input = argv[1];
    }

    ParseCalibration(input, vals);
    PrintCalibration(vals);

    return 0;
}