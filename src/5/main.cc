#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <thread>

using namespace std;
using namespace std::chrono_literals;

#define NUMBERS "0123456789"
long SOURCE_NOT_FOUND = -1;

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

void PrintVector(const vector<long> &v)
{
    for (const auto &vi : v)
        cout << vi << " ";
    cout << endl;
}

typedef struct _Interval
{
    long start, length;
} Interval;

void IntervalPrint(const Interval &intr)
{
    cout << "[" << intr.start << ", " << intr.start + intr.length - 1L << "]" << endl;
}

class SeedMap
{
public:
    class SubMap
    {
    public:
        long destination, source, length;

        SubMap(long _source, long _destination, long _length)
        {
            source = _source;
            destination = _destination;
            length = _length;
        };
        ~SubMap(){};

        void Map(const long &_source, long &_destination) const
        {
            long d = _source - source;
            if (d >= 0 && d < length)
            {
                _destination = destination + d;
            }
            else
            {
                _destination = SOURCE_NOT_FOUND;
            }
        }
    };

    vector<SubMap> maps;

    inline void MapInterval(const Interval &source, vector<Interval> &destination)
    {
        Interval interval_from, interval_to;
        long d, l, s;

        interval_from = source;
        destination.clear();
        // Find next submap which contains start
        for (const SubMap &submap : maps)
        {
            // See if start of interval is within the bounds of the submap
            d = interval_from.start - submap.source;

            if (d >= 0 && d < submap.length)
            {
                // Compute number of elements that can be mapped
                l = min(submap.length - d, interval_from.length);
                if (l == 0) return;
                s = submap.destination + d;

                interval_to.start = s;
                interval_to.length = l;
                destination.push_back(interval_to);

                // Get new start
                interval_from.start += l;
                interval_from.length -= l;
            }
        }
    }

    void Print() const
    {
        cout << "Destination, Source, Length" << endl;
        for (const SubMap &submap : maps)
        {
            cout << submap.destination << " " << submap.source << " " << submap.length << endl;
        }
    }

    void Fill()
    {
        // Fill voids in seedmap
        vector<SubMap> newmaps = {};
        SubMap newmap = SubMap(0L, 0L, 0L);

        // Sort seedmap by submap starts
        sort(maps.begin(), maps.end(), [](SubMap &a, SubMap &b)
             { return a.source < b.source; });

        int nmaps = maps.size();
        for (int i = 0; i < nmaps; i++)
        {
            if (i == 0)
            {
                newmap.source = 0L;
            }
            else
            {
                newmap.source = maps[i - 1].source + maps[i - 1].length + 1;
            }
            newmap.destination = newmap.source;
            newmap.length = maps[i].source - newmap.source;

            if (newmap.length > 0)
            {
                newmaps.push_back(newmap);
            }
        }
        // Get last map
        newmap.source = maps[nmaps - 1].source + maps[nmaps - 1].length;
        newmap.destination = newmap.source;
        newmap.length = LONG_MAX - newmap.source;
        newmaps.push_back(newmap);

        for (auto it = newmaps.begin(); it < newmaps.end(); it++)
        {
            maps.push_back(*it);
        }

        sort(maps.begin(), maps.end(), [](SubMap &a, SubMap &b)
             { return a.source < b.source; });
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

    void PrintMaps()
    {
        for (const SeedMap &smap : maps)
        {
            smap.Print();
        }
    }

    void GetScoreA()
    {
        vector<Interval> seed_inters, source_intervals, destination_intervals;

        cout << "Seeds: " << endl;
        for (int i = 0; i < seeds.size(); i++)
        {
            Interval ii = {seeds[i], 1};
            seed_inters.push_back(ii);
            IntervalPrint(ii);
        }

        source_intervals = seed_inters;
        /* Loop over maps */
        for (int i = 0; i < maps.size(); i++)
        {   
            /* Get current map */
            SeedMap smapi = maps[i];
            destination_intervals.clear();

            /* Loop over intervals */
            for (int j = 0; j < source_intervals.size(); j++)
            {
                vector<Interval> destj;
                smapi.MapInterval(source_intervals[j], destj);
                for (const Interval &djk : destj)
                    destination_intervals.push_back(djk);
            }

            /* Update sources */
            source_intervals = destination_intervals;
            sort(source_intervals.begin(), source_intervals.end(),
                [](Interval &a, Interval &b){return a.start < b.start;});
        }

        /* Get minimum value */
        scoreB = LONG_MAX;
        for (const Interval &ii : destination_intervals) {
            scoreB = min(scoreB, ii.start);
        }
        printf("Score B: %ld.\n", scoreB);
    }

    void GetScoreB()
    {
        vector<Interval> seed_inters, source_intervals, destination_intervals;

        cout << "Seeds: " << endl;
        for (int i = 0; i < seeds.size(); i += 2)
        {
            Interval ii = {seeds[i], seeds[i + 1]};
            seed_inters.push_back(ii);
            IntervalPrint(ii);
        }

        source_intervals = seed_inters;
        /* Loop over maps */
        for (int i = 0; i < maps.size(); i++)
        {   
            /* Get current map */
            SeedMap smapi = maps[i];
            destination_intervals.clear();

            /* Loop over intervals */
            for (int j = 0; j < source_intervals.size(); j++)
            {
                vector<Interval> destj;
                smapi.MapInterval(source_intervals[j], destj);
                for (const Interval &djk : destj)
                    destination_intervals.push_back(djk);
            }

            /* Update sources */
            source_intervals = destination_intervals;
            sort(source_intervals.begin(), source_intervals.end(),
                [](Interval &a, Interval &b){return a.start < b.start;});
        }

        /* Get minimum value */
        scoreB = LONG_MAX;
        for (const Interval &ii : destination_intervals) {
            scoreB = min(scoreB, ii.start);
        }
        printf("Score B: %ld.\n", scoreB);
    }

private:
    string input;
    ifstream file;

    vector<long> seeds;
    vector<SeedMap> maps;
    long scoreA = 0, scoreB = 0;

    void ParseHeader()
    {
        string header;
        getline(file, header);

        seeds = {};
        ParseLineOfNumbers(header.substr(5), seeds);

        // Throw away empty line
        getline(file, header);
    }

    void ParseLine()
    {
        SeedMap map = SeedMap();
        SeedMap::SubMap smap = SeedMap::SubMap(0L, 0L, 0L);

        string line;
        vector<long> nums;

        // Remove header
        getline(file, line);

        // Parse map data
        while (file.good())
        {
            getline(file, line);
            if (line.empty())
                break;

            ParseLineOfNumbers(line, nums);

            smap = SeedMap::SubMap(nums[1], nums[0], nums[2]);

            map.maps.push_back(smap);
        }

        map.Fill();
        map.Print();
        maps.push_back(map);
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
    gp.PrintMaps();
    gp.GetScoreA();
    gp.GetScoreB();

    return 0;
}