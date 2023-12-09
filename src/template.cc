#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <thread>

using namespace std;

#define NUMBERS "0123456789"

void ParseLineOfNumbers(const string &line, vector<long> &nums)
{
    int first = 0, last;
    long num;

    nums.clear();
    while (last != line.size())
    {
        first = line.find_first_of(NUMBERS, first);
        last = line.find_first_not_of(NUMBERS, first);
        last = (last == -1) ? line.size() : last;

        num = stol(line.substr(first, last - first));
        nums.push_back(num);

        first = last + 1;
    }
}


class Parser
{
public:
    Parser(const string &_input)
    {
        input = _input;
    }

    void ParseInput()
    {
        // Open file
        file.open(input.c_str(), ios::out);

        if (!file.is_open())
        {
            cout << "Could not open file.\n";
            return;
        }

        // Parse header
        ParseHeader();

        // Parse lines
        while (file.good())
        {
            ParseLine();
        }

        // Clean up
        file.close();
    }

    void GetScoreA()
    {

    }

    void GetScoreB()
    {

    }

private:
    string input;
    ifstream file;

    long scoreA = 0, scoreB = 0;

    void ParseHeader()
    {

    }

    void ParseLine()
    {

    }
};

int main(int argc, char *argv[])
{
    std::string input = "";

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

    Parser gp = Parser(input);
    gp.ParseInput();
    gp.GetScoreA();
    gp.GetScoreB();

    return 0;
}