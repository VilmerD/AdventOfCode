#include <fstream>
#include <iostream>
#include <iomanip>

#include <string>
#include <vector>

using namespace std;

const string NUMBERS = "-0123456789";

const int FIELDWIDTH = 8;
void PrintLine(vector<long> &line, int off)
{
    for (auto i = 0; i < off; i++) cout << setw(FIELDWIDTH) << "";

    for (auto it = line.begin(); it != line.end(); it++)
    {
        cout << setw(FIELDWIDTH) << *it << setw(FIELDWIDTH) << " ";
    }
    cout << endl;
}

vector<long> ParseLineOfNumbers(const string &line)
{
    int first = 0, last;
    long num;
    vector<long> nums;

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

    return nums;
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
        long sum = 0L;
        for (auto it = next.begin(); it < next.end(); it++) sum += *it;
        cout << "Sum of next numbers: " << sum << endl;
    }

    void GetScoreB()
    {

    }

private:
    string input;
    ifstream file;

    long scoreA = 0, scoreB = 0;

    vector<long> next;

    void ParseLine()
    {
        // Get next line of numbers
        string line;
        getline(file, line);
        vector<long> nums = ParseLineOfNumbers(line);

        // Process numbers
        PrintLine(nums, 0);

        next.push_back(ProcessNumbers(nums));

        cout << string(80, '=') << endl << endl;
    }

    long ProcessNumbers(vector<long> &nums)
    {
        bool iszeros = false;
        vector<long> line = nums;
        vector<vector<long>> lines = {line};

        // While next line is not zeros 
        // Get next line and push to list
        while (!IsZeros(line))
            lines.push_back(line = GetNextLine(line));
        

        cout << "Processed Lines: " << endl;
        for (auto it = lines.begin(); it != lines.end(); it++)
        {
           PrintLine(*it, distance(lines.begin(), it));
        }

        // Compute next value based on line
        long below = 0L, behind;
        for (auto it = lines.rbegin(); it != lines.rend(); ++it)
        {
            (*it).emplace((*it).begin(), below=(*it).front()-below);
        }

        cout << "Appended Lines: " << endl;
        for (auto it = lines.begin(); it != lines.end(); it++)
        {
           PrintLine(*it, distance(lines.begin(), it));
        }

        // Last below is the new value
        return below;
    }

    vector<long> GetNextLine(const vector<long> &line)
    {
        vector<long> next = {};
        for (auto itm1 = line.begin(), it = line.begin()+1; it != line.end(); it++, itm1++)
        {
            next.push_back(*it - *itm1);
        }
        return next;
    }

    bool IsZeros(const vector<long> &line)
    {
        for (auto it = line.begin(); it != line.end(); it++)
            if ((*it) != 0) return false;
        return true;
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

    Parser parser = Parser(input);
    parser.ParseInput();
    parser.GetScoreA();
    parser.GetScoreB();

    return 0;
}