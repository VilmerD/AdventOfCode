#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <thread>

using namespace std;

typedef struct _point2d
{
    int x, y;
} point2d;
ostream &operator<<(ostream &os, const point2d &p)
{
    os << "point2d: (r, c) = (" << p.x << ", " << p.y << ")";
    return os;
};
bool operator==(const point2d &lhs, const point2d &rhs)
{
    return (lhs.x == rhs.x) && (lhs.y == rhs.y);
}
bool operator!=(const point2d &lhs, const point2d &rhs)
{
    return !(lhs == rhs);
}
point2d operator-(const point2d &lhs, const point2d &rhs)
{
    return {lhs.x - rhs.x, lhs.y - rhs.y};
}

typedef struct _pointpair
{
    point2d p1, p2;
} pointpair;
ostream &operator<<(ostream &os, const pointpair &p)
{
    os << "P1: " << p.p1 << endl
       << "P2: " << p.p2;
    return os;
}
bool operator==(const pointpair &lhs, const pointpair &rhs)
{
    return ((lhs.p1 == rhs.p1) && (lhs.p2 == rhs.p2) || (lhs.p1 == rhs.p2) && (lhs.p2 == rhs.p1));
}
bool operator!=(const pointpair &lhs, const pointpair &rhs)
{
    return !(lhs == rhs);
}

class GalaxyMap
{
public:
    GalaxyMap(const vector<string> &_map)
    {
        mapR = _map;
        rows = mapR.size();
        cols = mapR[0].size();

        GetColMap();
        pairs = {};
        distances = {};
    };

    void FindDistances()
    {
        ExpandSpace();
        GetPoints();
        GetPairs();
        ComputeDistances();
    }

    vector<long> GetDistances()
    {
        return distances;
    }

    friend ostream &operator<<(ostream &os, const GalaxyMap &map)
    {
        cout << "Row Map" << endl;
        for (auto it = map.mapR.begin(); it != map.mapR.end(); it++)
            cout << *it << endl;
        cout << endl;

        cout << "Col Map" << endl;
        for (auto it = map.mapC.begin(); it != map.mapC.end(); it++)
            cout << *it << endl;
        cout << endl;

        cout << "Points (" << map.points.size() << ")" << endl;
        cout << "Pairs (" << map.pairs.size() << ")" << endl;
        return os;
    }

private:
    vector<string> mapR, mapC;
    vector<point2d> points;
    vector<pointpair> pairs;

    vector<long> distances;
    vector<int> emptyRows, emptyCols;

    int rows, cols;

    void ExpandSpace()
    {
        // Expand first in rows
        emptyRows.clear();
        for (auto it = mapR.begin(); it != mapR.end(); it++)
        {
            if (!LineHasGalaxy(*it))
            {
                emptyRows.push_back(distance(mapR.begin(), it));
            }
        }
        cout << "Empty Rows:" << endl;
        for (auto it = emptyRows.begin(); it != emptyRows.end(); it++)
        {
            cout << "[" << *it << "]" << mapR[*it] << endl;
        }

        // Expand next in cols
        emptyCols.clear();
        for (auto it = mapC.begin(); it != mapC.end(); it++)
        {
            if (!LineHasGalaxy(*it))
            {
                emptyCols.push_back(distance(mapC.begin(), it));
            }
        }
        for (auto it = emptyCols.begin(); it != emptyCols.end(); it++)
        {
            cout << "[" << *it << "]" << mapC[*it] << endl;
        }
        cout << endl;

    }
    void GetPoints()
    {
        for (auto it = mapR.begin(); it != mapR.end(); it++)
        {
            for (auto itt = (*it).begin(); itt != (*it).end(); itt++)
            {
                if (*itt == '#')
                {
                    points.push_back({(int)distance(mapR.begin(), it),
                                      (int)distance((*it).begin(), itt)});
                }
            }
        }
    }
    void GetPairs()
    {
        for (auto it = points.begin(); it != points.end(); it++)
        {
            for (auto itt = points.begin(); itt != points.end(); itt++)
            {
                if (*it != *itt)
                {
                    pointpair pp = pointpair({*it, *itt});
                    auto ittt = find(pairs.begin(), pairs.end(), pp);
                    if (ittt == pairs.end())
                        pairs.push_back(pp);
                }
            }
        }
    }
    void ComputeDistances()
    {
        for (auto it = pairs.begin(); it != pairs.end(); it++)
            distances.push_back(ComputeDistance((*it).p1, (*it).p2));
    }
    #define sign(a) a / abs(a)
    long ComputeDistance(const point2d &p1, const point2d &p2)
    {
        // Get distance disregarding empty lines
        point2d d = p2 - p1;

        // Compute number of empty cols or rows between p1 and p2
        long d0 = 0L;
        for (int i = 0, x = p1.x; i < abs(d.x); i++, x+=sign(d.x))
        {
            d0 += 1L;
            
            if (find(emptyRows.begin(), emptyRows.end(), x) != emptyRows.end())
            {
                d0 += 999999L;
            }
        }
        for (int i = 0, y = p1.y; i < abs(d.y); i++, y+=sign(d.y))
        {
            d0 += 1L;
            if (find(emptyCols.begin(), emptyCols.end(), y) != emptyCols.end())
            {
                d0 += 999999L;
            }
        }

        return d0;
    }

    bool LineHasGalaxy(const string &line)
    {
        auto it = find(line.begin(), line.end(), '#');
        return it != line.end();
    }
    void GetColMap()
    {
        mapC = {};
        for (int c = 0; c < cols; c++)
        {
            mapC.push_back("");
        }
        for (int r = 0; r < rows; r++)
        {
            string row = mapR[r];
            for (int c = 0; c < cols; c++)
            {
                mapC[c].push_back(row[c]);
            }
        }
    }
    void GetRowMap()
    {
        mapR = {};
        for (int r = 0; r < rows; r++)
        {
            mapR.push_back("");
        }
        for (int c = 0; c < cols; c++)
        {
            string col = mapC[c];
            for (int r = 0; r < rows; r++)
            {
                mapR[r].push_back(col[r]);
            }
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
        GalaxyMap map = GalaxyMap(lines);
        map.FindDistances();
        cout << map << endl;

        vector<long> dis = map.GetDistances();
        long distot;

        for (auto it = dis.begin(); it != dis.end(); it++)
        {
            distot += *it;
        };
        cout << "Total distance: " << distot << endl;
    }

    void GetScoreB()
    {
    }

private:
    string input;
    ifstream file;

    vector<string> lines;

    long scoreA = 0, scoreB = 0;

    void ParseHeader()
    {
    }

    void ParseLine()
    {
        string line;
        getline(file, line);
        lines.push_back(line);
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