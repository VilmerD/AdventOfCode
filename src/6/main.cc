#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <thread>

using namespace std;

#define NUMBERS "0123456789"

void Concatenate(const vector<long> &nums, long &num) {
    num = nums.back();
    long base0 = 1L, vi, vim1;
    for (int i = nums.size()-1; i >= 0; i--) {
        cout << num << endl;
        vi = nums[i];
        vim1 = nums[i-1];

        // Get closest exponent
        long base = 1L;
        while (base < vi) {
            base*=10;
            cout << base << endl;
        }
        base0*=base;
        num += vim1 * base0;
    }
}

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

    const void GetChargeTimes(const long &T, const long &D, long &t1, long &t2) const {
        double _T, _D, _t1, _t2, _disc;
        _T = (double)T;
        _D = (double)D;
        _disc = _T * _T - 4.0*_D;

        if (_disc < 0) {
            cout << "Negative discriminant!" << endl;
            exit(-1);
        } else {
            _t1 = (1.0/2.0) * (_T - sqrt(_disc));
            _t2 = (1.0/2.0) * (_T + sqrt(_disc));

            cout << "t1 = " << _t1 << " t2 = " << _t2 << endl;
            t1 = (long)ceil(_t1);
            t2 = (long)floor(_t2);
            
            cout << "t1 = " << t1 << " t2 = " << t2 << endl;
        }

        // Check that the times are ok
        long Dk;
        Dk = t1 * (T - t1);
        while (Dk <= D) {
            t1++;
            Dk = t1 * (T - t1);
        }
        Dk = t2 * (T - t2);
        while (Dk <= D) {
            t2--;
            Dk = t2 * (T - t2);
        }
    }

    void GetScoreA()
    {
        int TS, DS;
        TS = times.size();
        DS = distances.size();
        if (TS != DS) {
            cout << "Expected number of times = number of distances." << endl;
            exit(-1);
        }

        // Loop over races
        scoreA = 1L;
        long T, D, t1, t2;
        for (int i = 0; i < TS; i++) {
            T = times[i], D = distances[i];
            GetChargeTimes(T, D, t1, t2);

            scoreA *= (t2 - t1) + 1;
            cout << scoreA << endl;
        }

        cout << "Score of task A: " << scoreA << endl;
    }

    void GetScoreB()
    {
        long T, D, t1, t2;
        Concatenate(times, T);
        Concatenate(distances, D);

        GetChargeTimes(T, D, t1, t2);
        scoreA = (t2 - t1) + 1;

        cout << "Score of task A: " << scoreA << endl;
    }

private:
    string input;
    ifstream file;

    long scoreA = 0, scoreB = 0;
    vector<long> times;
    vector<long> distances;

    void ParseHeader()
    {
        int start;
        string line;

        // Get times
        getline(file, line);
        start = line.find_first_of(NUMBERS, 0);
        ParseLineOfNumbers(line.substr(start), times);

        // Get distances
        getline(file, line);
        start = line.find_first_of(NUMBERS, 0);
        ParseLineOfNumbers(line.substr(start), distances);
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