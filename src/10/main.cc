#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <thread>

using namespace std;

typedef enum _Key
{
    S = 'S',
    G = '.',
    V = '|',
    H = '-',
    NE = 'L',
    SE = 'F',
    SW = '7',
    NW = 'J',
} Key;

typedef enum _Direction
{
    MISSING = 'X',
    NN = 'N',
    EE = 'E',
    SS = 'S',
    WW = 'W',
} Direction;

typedef struct _point2d
{
    int r, c;
} point2d;

ostream &operator<<(ostream &os, const point2d &p)
{
    os << "point2d: (r, c) = (" << p.r << ", " << p.c << ")" << endl;
    return os;
}

class PipeMap
{
public:
    PipeMap(const vector<string> &_input) : input(_input)
    {
        SetUpOutput();
        SetUpTokens();
        rows = input.size();
        cols = input[0].size();
    };
    ~PipeMap(){};

    int FindLoop()
    {
        // Find starting point
        int d = 0;
        point2d start, p, next, prev;

        start = GetStarting();

        // Iterate through map following rules
        p = start;
        prev = start;

        do
        {
            // Put value of output key
            PutPointOutput(p);

            // Get next point
            next = GetNext(p, prev);

            // Increment and update
            prev = p;
            p = next;
            d++;
        } while (!IsDone(p));

        return d;
    }
    void PutTokens()
    {
        // Find starting point
        int d = 0;
        point2d start, p, next, prev;

        start = GetStarting();

        // Iterate through map following rules
        p = start;
        prev = start;

        do
        {            
            // Get next point
            next = GetNext(p, prev);

            // Increment and update
            prev = p;
            p = next;
            d++;

            // Put token after next
            tokens[p.r][p.c] = ' ';
            PutToken(p, prev);
        } while (!IsDone(p));
    }

    void CountTokens(int &R, int& L)
    {
        R = 0;
        L = 0;

        for (auto it = tokens.begin(); it != tokens.end(); it++)
        {
            for (auto itt = (*it).begin(); itt != (*it).end(); itt++)
            {
                if ((*itt) == 'R') R++;
                else if ((*itt) == 'L') L++;
            }
        } 
    }

    friend ostream &operator<<(ostream &os, const PipeMap &map)
    {
        cout << "Input map:" << endl;
        for (auto it = map.input.begin(); it != map.input.end(); it++)
        {
            cout << (*it) << endl;
        }
        cout << endl;

        cout << "Output map:" << endl;
        for (auto it = map.output.begin(); it != map.output.end(); it++)
        {
            cout << (*it) << endl;
        }
        cout << endl;

        cout << "Token map:" << endl;
        for (auto it = map.tokens.begin(); it != map.tokens.end(); it++)
        {
            cout << (*it) << endl;
        }

        return os;
    }

private:
    vector<string> input, output, tokens;
    int rows, cols;

    point2d GetStarting()
    {
        for (auto it = input.begin(); it != input.end(); it++)
        {
            for (auto itt = (*it).begin(); itt != (*it).end(); itt++)
            {
                if (*itt == Key::S)
                {
                    return {
                        static_cast<int>(distance(input.begin(), it)),
                        static_cast<int>(distance((*it).begin(), itt))};
                }
            }
        }

        return {0, 0};
    }
    point2d GetNext(const point2d &p, const point2d &pm1)
    {
        Key k;
        k = GetPointInput(p);

        // Loop over connected points.
        // Starting from north and ending in south
        if (k == Key::S)
        {
            Key n;
            point2d pp1 = p;

            pp1.r++;
            n = GetPointInput(pp1);
            // For north: a vertical or a south key fits
            if (n == Key::V || n == Key::SE || n == Key::SW)
                return pp1;

            pp1.r--;
            pp1.c++;
            n = GetPointInput(pp1);
            // For east: a horizontal or a west key fits
            if (n == Key::H || n == Key::NW || n == Key::SW)
                return pp1;

            pp1.r--;
            n = GetPointInput(pp1);
            // For south: a vertical or north key fits
            if (n == Key::V || n == Key::NE || n == Key::NW)
                return pp1;

            pp1.r++;
            pp1.c--;
            n = GetPointInput(pp1);
            // For a west: a hirizontal or an east key fits
            if (n == Key::H || n == Key::NE || n == Key::SE)
                return pp1;
        }

        // Otherwise get next according to rules
        int dr, dc;
        dr = p.r - pm1.r;
        dc = p.c - pm1.c;
        switch (k)
        {
        case Key::V:
        {
            return {p.r + dr, p.c};
        }
        case Key::H:
        {
            return {p.r, p.c + dc};
        }
        case Key::NE:
        {
            if (dc == 0)
                return {p.r, p.c + 1};
            else
                return {p.r - 1, p.c};
        }
        case Key::SE:
        {
            if (dc == 0)
                return {p.r, p.c + 1};
            else
                return {p.r + 1, p.c};
        }
        case Key::SW:
        {
            if (dc == 0)
                return {p.r, p.c - 1};
            else
                return {p.r + 1, p.c};
        }
        case Key::NW:
        {
            if (dc == 0)
                return {p.r, p.c - 1};
            else
                return {p.r - 1, p.c};
        }
        default:
            return p;
        }

        return p;
    }
    bool IsDone(const point2d &p)
    {
        return GetPointInput(p) == Key::S;
    }

    void SetUpOutput()
    {
        int cols, rows;
        cols = input[0].size();
        rows = input.size();

        string ground = string(cols, Key::G);
        for (int i = 0; i < rows; i++)
            output.push_back(ground);
    }
    void SetUpTokens()
    {
        int cols, rows;
        cols = input[0].size();
        rows = input.size();

        string ground = string(cols, Key::G);
        for (int i = 0; i < rows; i++)
            tokens.push_back(ground);
    }
    Key GetPointInput(const point2d &p)
    {
        return static_cast<Key>(input[p.r][p.c]);
    }
    void PutPointOutput(const point2d &p)
    {
        if ((p.c >= 0 && p.c < cols) && (p.r >= 0 && p.r < rows))
        {
            output[p.r][p.c] = input[p.r][p.c];
        }
    }
    Direction GetDirection(const point2d &p, const point2d &pm1)
    {
        int dr, dc;
        dr = p.r - pm1.r;
        dc = p.c - pm1.c;

        if (dr == 0)
            return ((dc > 0) ? Direction::EE : Direction::WW);
        else if (dc == 0)
            return ((dr > 0) ? Direction::SS : Direction::NN);
        else return Direction::MISSING;
    }
    void PutToken(const point2d &p, const point2d &pm1)
    {
        // Get direction
        Direction d = GetDirection(p, pm1);
        int i;

        switch (d)
        {
        case Direction::NN:
            i = 0; while(PutToken({p.r, p.c + ++i}, 'R'));
            i = 0; while(PutToken({pm1.r, pm1.c + ++i}, 'R'));
            i = 0; while(PutToken({p.r, p.c + --i}, 'L'));
            i = 0; while(PutToken({pm1.r, pm1.c + --i}, 'L'));
            return;
        case Direction::EE:
            i = 0; while(PutToken({p.r + ++i, p.c}, 'R'));
            i = 0; while(PutToken({pm1.r + ++i, pm1.c}, 'R'));
            i = 0; while(PutToken({p.r + --i, p.c}, 'L'));
            i = 0; while(PutToken({pm1.r + --i, pm1.c}, 'L'));
            return;
        case Direction::SS:
            i = 0; while(PutToken({p.r, p.c + --i}, 'R'));
            i = 0; while(PutToken({pm1.r, pm1.c + --i}, 'R'));
            i = 0; while(PutToken({p.r, p.c + ++i}, 'L'));
            i = 0; while(PutToken({pm1.r, pm1.c + ++i}, 'L'));
            return;
        case Direction::WW:
            i = 0; while(PutToken({p.r + --i, p.c}, 'R'));
            i = 0; while(PutToken({pm1.r + --i, pm1.c}, 'R'));
            i = 0; while(PutToken({p.r + ++i, p.c}, 'L'));
            i = 0; while(PutToken({pm1.r + ++i, pm1.c}, 'L'));
            return;
        default:
            return;
        }
    }
    bool PutToken(const point2d &p, const char &t)
    {
        bool rowOk = (p.r >= 0 && p.r < rows);
        bool colOk = (p.c >= 0 && p.c < cols);
        if (rowOk && colOk && output[p.r][p.c] == Key::G)
        {
            tokens[p.r][p.c] = t;
            return true;
        } else {
            return false;
        }
    }
};

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
        PipeMap map = PipeMap(data);
        int d = map.FindLoop();

        cout << "Total length of loop: " << d << endl;
        cout << "Half length of loop: " << d / 2 << endl;
    }

    void GetScoreB()
    {
        PipeMap map = PipeMap(data);
        int r, l;
        map.FindLoop();
        map.PutTokens();
        map.CountTokens(r, l);

        cout << "Number of right tokens " << r << endl;
        cout << "Number of left tokens " << l << endl;

        cout << map << endl;
    }

private:
    string input;
    ifstream file;

    vector<string> data;

    long scoreA = 0, scoreB = 0;

    void ParseHeader()
    {
    }

    void ParseLine()
    {
        string line;
        getline(file, line);
        data.push_back(line);
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