#include <fstream>
#include <iostream>

#include <string>
#include <vector>
#include <map>

#include <thread>

#include <numeric>

using namespace std;

typedef unordered_map<string, vector<string>> keymap;

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

    int TraverseMap()
    {
        // Starting with the first key, follow the rule
        const string firstKey = "AAA", lastKey = "ZZZ";
        string key = firstKey;
        vector<string> value;

        int ruleNbr = 0, numberRules = rule.size(), steps = 0;
        char ruleChar;
        while (key != lastKey)
        {
            // Get value of key
            value = map[key];

            // Get next rule
            ruleChar = rule[ruleNbr];

            switch (ruleChar)
            {
            case 'L':
                key = value[0];
                break;
            case 'R':
                key = value[1];
                break;
            }

            // Update rule number
            ruleNbr++;
            ruleNbr = ruleNbr % numberRules;

            // Update number of steps
            steps++;
        }
        return steps;
    }

    int TraverseMapParallel()
    {
        // Find keys that end with A
        vector<string> keys = GetStartingKeys();
        vector<int> zsteps;

        // Create a map of keys and at which steps they were visited

        for (const auto k : keys)
            cout << k << " ";
        cout << endl;

        // Check number of steps until keys loop
        int numberRules = rule.size();
        char ruleChar;
        for (const string &startKey : keys)
        {
            string key = startKey, lastkey = "___";
            int ruleNbr = 0, steps = 0, zstep;
            int numz = 0;
            bool keyOkey = false;

            while (!keyOkey)
            {
                // Update step number
                steps++;

                // Get next rule
                ruleChar = rule[ruleNbr];

                // Update last key
                lastkey = key;

                // Get next value
                vector<string> vals = map[key];
                switch (ruleChar)
                {
                case 'L':
                    key = vals[0];
                    break;
                case 'R':
                    key = vals[1];
                    break;
                }

                // Check if key has a z
                if (key[2] == 'Z')
                {
                    zsteps.push_back(steps);
                    keyOkey = true;
                }

                // Update rule
                ruleNbr++;
                ruleNbr = ruleNbr % numberRules;
            }
        }

        // Update user
        cout << "Steps to z: ";
        for (const auto & s : zsteps) cout << s << " ";
        cout << endl;

        long s = GetMapResults(zsteps);
        cout << "First step where all seeds lands in a z: " << s << endl;
        return 0;
    }

    long GetMapResults(const vector<int> &steps)
    {
        vector<int> _steps = steps;

        // Compute gcd of all numbers.
        // It is the smallest gcd of all pairs of numbers
        int _gcd = INT_MAX;
        for (int i = 1; i < steps.size(); i++)
        {
            _gcd = min(gcd(steps[i-1], steps[i]), _gcd);
        }

        cout << "Greatest common divisor among all numbers: " << _gcd << endl;
        
        // The least common multiple of all numbers is
        long _lcm = (long)_gcd;
        for (const auto & s : steps)
        {
            long div = (long)((long)s/(long)(_gcd));
            _lcm *= (long)div;
        }

        cout << "Least common multiple among all numbers: " << _lcm << endl;
        return _lcm;
    }

    vector<string> GetStartingKeys()
    {
        const char start = 'A';
        vector<string> startingKeys;
        for (const auto &imap : map)
        {
            string key = imap.first;
            if (key[2] == start)
                startingKeys.push_back(key);
        }
        return startingKeys;
    }

    long GetSmallestIntersection(const vector<int> &N, const vector<int> &L)
    {
        long t, tj, k = 0L;
        int nums = N.size(), j;
        while (true)
        {
            t = N[0] + k * L[0];
            cout << "t = " << t << " ";

            for (j = 1; j < nums; j++)
            {
                tj = N[j] + k * L[j];
                cout << tj << " ";
                if (tj != t)
                    break;
                else if (j == nums - 1)
                    return tj;
            }
            cout << endl;

            cout << "k = " << k << endl;
            k++;

            if (t < 0 || k == 1e3)
            {
                cout << "Could not find a solution." << endl;
                exit(-1);
            }
        }
    }

    void PrintKeys(vector<string> keys)
    {
        cout << "Current keys: ";
        for (const auto k : keys)
        {
            cout << k << " ";
        }
        cout << endl;
    }

    void GetScoreA()
    {
        int steps = TraverseMap();
        cout << "Took " << steps << " steps to reach ZZZ" << endl;
    }

    void GetScoreB()
    {
        int steps = TraverseMapParallel();
        cout << "Took " << steps << " steps to reach ZZZ" << endl;
    }

    void PrintMap()
    {
        for (const auto &[key, val] : map)
        {
            cout << key << " = "
                 << "[" << val[0] << ", " << val[1] << "]" << endl;
        }
    }

private:
    string input;
    ifstream file;

    string rule;
    keymap map;

    long scoreA = 0, scoreB = 0;

    void ParseHeader()
    {
        getline(file, rule);

        // Get empty line
        string _;
        getline(file, _);
    }

    void ParseLine()
    {
        string line;

        getline(file, line);

        // Get next key - value pair
        string key;
        vector<string> value;

        key = line.substr(0, 3);

        value.push_back(line.substr(7, 3));
        value.push_back(line.substr(12, 3));

        // Add key-value pair to map
        map[key] = value;
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
    // gp.PrintMap();
    gp.GetScoreA();
    gp.GetScoreB();

    return 0;
}