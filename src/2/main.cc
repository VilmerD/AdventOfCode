#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#define CHAR0 48
using namespace std;
const vector<string> colors = {"red", "green", "blue"};
const int target[3] = {12, 13, 14};

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

    void Print() {
        cout << "Sum of okey games: " << sum << endl;
        cout << "Powers of smallest possible number of balls: " << powers << endl;
    }

private:
    string input;
    int sum = 0, powers = 0;
    const char *gameHeader = " ";
    const char headerSep = ':';
    const char pullSep = ';';
    const char colorSep = ',';
    const char numberSep = ' ';

    void ParseLine(const string &line)
    {
        // Chop line up into pulls by separator ";"
        string header;
        vector<string> pulls;
        int gameID, values[3], maxvalues[3], minvalues[3];

        for (int i = 0; i < 3; i++) {
            minvalues[i] = INT_MAX;
            maxvalues[i] = 0;
        }

        ParseHeader(header, gameID);
        ChopLine(line, header, pulls);
        for (const auto &p : pulls)
        {
            for (int i = 0; i < 3; i++) values[i] = -1;
            ParsePull(p, values);

            // check if numbers are less than or equal to target
            for (int i = 0; i < 3; i++)
            {
                if (values[i] == -1) continue;
                maxvalues[i] = max(values[i], maxvalues[i]);
                minvalues[i] = min(values[i], minvalues[i]);
            }
        }
        
        // Check if the maximum number of balls for each color is less than
        // target and compute the power
        int power = 1;
        bool ok = true;
        cout << "1";
        for (int i = 0; i < 3; i++) {
            if (maxvalues[i] > target[i]) ok = false;
            if (minvalues[i] > 0) {
                power *= maxvalues[i];
                cout << "*" << maxvalues[i];
            }
        }
        cout << "=" << power << endl;
        if (ok) sum += gameID;
        powers += power;
        return;
    }

    void ChopLine(const string &line, string &header, vector<string> &pulls)
    {
        // Chop off game number
        int chopHeader = line.find(headerSep);
        header = line.substr(0, chopHeader);

        // Chop line into pulls
        int chopBegin = chopHeader + 2;
        int chopEnd = chopBegin + 1;
        pulls.clear();
        while (chopBegin + 1 < line.size() && chopEnd > 0)
        {
            chopEnd = line.find(pullSep, chopBegin + 1);
            pulls.push_back(line.substr(chopBegin, chopEnd - chopBegin));
            chopBegin = chopEnd + 2;
        }
    }

    void ParseHeader(const string &header, int &gameID)
    {
        int IDBegin = header.find(gameHeader);
        string numStr = header.substr(IDBegin + 1);
        gameID = strToNum(numStr);
    }

    void ParsePull(const string &pull, int values[3])
    {
        vector<string> chunks;
        int ib = 0, ie = 1, L = pull.size();

        // Split by comma
        while (ib < L && ie != -1)
        {
            ie = pull.find(colorSep, ib);
            chunks.push_back(pull.substr(ib, ie - ib));
            ib = ie + 2;
        }

        // Parse each substring
        for (int i = 0; i < chunks.size(); i++)
        {
            // Separate substring into number and color
            string chunk = chunks[i];
            ib = chunk.find(numberSep);

            // Convert str to number
            string numStr = chunk.substr(0, ib);
            string color = chunk.substr(ib + 1);
            int vali = strToNum(numStr);
            
            // Try to find color string
            for (int i = 0; i < 3; i++)
            {
                if (!strcmp(colors[i].c_str(), color.c_str()))
                {
                    values[i] = vali;
                    break;
                }
            }
        }
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
    gp.Print();

    return 0;
}