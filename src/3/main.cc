#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#define CHAR0 48

using namespace std;

int strToNum(string numStr)
{
    int num = 0, exp = 1;
    for (int i = numStr.size() - 1; i >= 0; i--)
    {
        int di = (int)numStr[i] - CHAR0;
        if (di < 0 || di > 9)
            return -1;
        num += di * exp;
        exp *= 10;
    }
    return num;
}

class Parser
{
public:
    Parser(const string &_input)
    {
        input = _input;
    }

    void Parse()
    {
        // Open file
        ifstream file;
        file.open(input.c_str(), ios::out);

        if (!file.is_open())
        {
            cout << "Could not open file.\n";
            return;
        }

        // Read lines
        string line;
        while (file.good())
        {
            getline(file, line);
            ParseLine(line);
        }

        // Clean up
        file.close();
    }

    void Print()
    {
        cout << "Sum of okey games: " << sum << endl;
        cout << "Sum of gear ratios: " << sumGear << endl;
    }

    void ProcessData()
    {
        // For each line
        string lines[3], numStr;
        int num = 0;

        // Pad data with lines of dots
        string dots = string(data[0].size(), '.');
        data.push_back(dots);
        data.emplace(data.begin(), dots);
        for (int i = 1; i < data.size() - 1; i++)
        {
            // Get line
            lines[0] = data[i - 1];
            lines[1] = data[i];
            lines[2] = data[i + 1];

            // Find next digit
            int p = 0, e = 0, s;
            while (e != -1)
            {
                p = lines[1].find_first_of("0123456789", e);
                e = lines[1].find_first_not_of("0123456789", p);

                if (p == -1 || e == -1)
                    break;

                // Get number
                numStr = lines[1].substr(p, e - p);
                num = strToNum(numStr);

                // Search neighbhouring lines for a non-point
                for (int j = 0; j < 3; j++)
                {
                    s = lines[j].find_first_not_of(".0123456789", p - 1);
                    if (s != -1 && s < e + 1)
                    {
                        sum += num;
                        break;
                    }
                }
            }
        }
        return;
    }
    void ProcessGears()
    {
        // For each line
        string lines[3], numStr;
        vector<int> gears;
        int num = 0, gear, foundGears;

        // Pad data with lines of dots
        string dots = string(data[0].size(), '.');
        data.push_back(dots);
        data.emplace(data.begin(), dots);

        // Loop over lines
        for (int i = 1; i < data.size() - 1; i++)
        {
            // Print
            cout << string(60, '=').c_str() << endl;
            cout << i << "\t" << endl;

            // Get line
            lines[0] = data[i - 1];
            lines[1] = data[i];
            lines[2] = data[i + 1];

            // Find next digit
            int p = 0, e = 1, first, last;
            while (e != -1)
            {
                // Find astetrix
                foundGears = 0;
                gears.clear();

                p = lines[1].find('*', e);
                e = p + 1;
                if (p == -1)
                    break;

                cout << "\t" << p << endl << "\t";

                // Search previous line
                for (int j = 0; j < 3; j ++)
                {
                    first = 0;
                    last = p;
                    while (last != -1)
                    {
                        NextNumberInterval(lines[j], first, lines[j].size()-1, first, last);
                        if (first > p + 1 || last < p)
                        {
                            first = last+1;
                            continue;
                        }
                        else
                        {
                            num = strToNum(lines[j].substr(first, last - first));
                            cout << "\t " << num;
                            gears.push_back(num);
                            first = last+1;
                            foundGears++;
                        }
                    }
                }

                if (foundGears == 2) {
                    gear = gears[0] * gears[1];
                    cout << "\t +=" << gear;
                    sumGear += gear;
                }

                cout << endl;
            }
        }
        return;
    }

private:
    string input;
    vector<string> data;
    int sum = 0, sumGear = 0;

    void ParseLine(const string &line)
    {
        string _line = line;
        _line.insert(0, ".", 1);
        _line.push_back('.');
        data.push_back(_line);
        return;
    }

    void NextNumberInterval(const string &line, const int &start, const int &end,
                            int &first, int &last)
    {
        first = line.find_first_of("0123456789", start);
        last = line.find_first_not_of("0123456789", first);
        return;
    }
};

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

    Parser gp = Parser(input);
    gp.Parse();
    gp.ProcessData();
    gp.ProcessGears();
    gp.Print();

    return 0;
}