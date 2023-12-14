#include "../lib/CompetitiveCodingLibrary.hh"
#include <list>

using namespace std;

/*
    Strategy: Recursive
        Traverse the list of numbers.
        For each number find possible choices of ? that fullfil the list
        For each found permutation, continue.
        If last number, don't recurse -> return
*/

class SpringParser : public CCL::IO::Parser
{
public:
    SpringParser(const string _inpt) { input = _inpt; };
    void GetScore()
    {
        cout << "Score: " << score << endl;
    }

private:
    int score = 0;

    void ParseLine() override
    {
        string line;
        getline(file, line);
        TranslateLine(line);
    }

    void TranslateLine(const string &line)
    {
        int rep = 5;
        string springs = CCL::IO::GetFirstNotOf(line, ".#?");
        vector<int> nums = CCL::IO::SplitInts(line);
        
        string srep = springs;
        vector<int> numrep = nums;

        for (int i = 0; i < rep; i++) {
            srep += "?";
            srep += springs;

            for (int j = 0; j < nums.size(); j++) {
                numrep.push_back(nums[j]);
            }
        }
        cout << srep << endl;
        for (auto it = numrep.begin(); it < numrep.end(); it++) cout << *it << ", ";
        cout << endl;

        int sols = GetNext(srep, numrep);
        cout << "Line: " << srep << "" << endl;
        cout << "Solutions: " << sols << endl;
        score += sols;
    }

    /* Changes next ? */
    int GetNext(string line, const vector<int> &nums)
    {
        int sols = 0;

        // Find next ?
        auto first = find(line.begin(), line.end(), '?');
        if (first == line.end())
        {
            if (CheckLine(line, nums)) return 1;
        }
        else
        {
            int d = distance(line.begin(), first);

            /* Check if there are too many or too few possible chunks */
            if (!CheckFirstChunks(line, nums) ||
                !CheckFits(line, nums) ||
                !CheckEnoughSymbols(line, nums))
                return 0;

            /* If not recurse */
            line[d] = '.';
            sols += GetNext(line, nums);

            line[d] = '#';
            sols += GetNext(line, nums);
        }

        return sols;
    }

    bool EstimateMinMax(const string &line, const vector<int> &nums)
    {
        /* Divide line into chunks */
        vector<string> chunks = CCL::IO::SplitLine(line, "?#");

        /* The minimum number is simply number of nonzero chunks */
        /* The maximum number is the sum of maximum number among chunks*/
        int numValsMax = 0, chunkSize = 0;
        int chunkSizeMax = -1;
        for (int i = 0; i < chunks.size(); i++)
        {
            chunkSize = chunks[i].size();
            chunkSizeMax = max(chunkSize, chunkSizeMax);
            
            /* Estimate maximum number by just half the string size */
            numValsMax += static_cast<int>(((double)chunkSize / 2.0) + 0.5);
        }

        int n = nums.size(), maxVal = -1;
        for (auto it = nums.begin(); it != nums.end(); it++) maxVal = max(maxVal, *it);
        return (n <= numValsMax && maxVal <= chunkSizeMax);
    }
    bool CheckFirstChunks(const string &line, const vector<int> &nums)
    {
        // Split line at first ? Check if the sequence is ok until then at least
        string first = CCL::IO::GetFirstNotOf(line, "#.");
        vector<string> chunks = CCL::IO::SplitLine(first, "#");
        if (chunks.empty()) return true;

        int n = chunks.size();
        if (line[first.size()] == '?') n--;

        for (int i = 0; i < n; i++)
        {
            if (chunks[i].size() != nums[i]) return false;
        }
        
        return true;
    }
    bool CheckFits(const string &line, const vector<int> &nums)
    {
        int minsize = 0;
        for (auto it = nums.begin(); it != nums.end(); it++) minsize += ((*it) + 1);
        minsize--;
        
        int maxspace = line.size() - line.find_first_of("?#");
        return maxspace >= minsize;
    }
    bool CheckEnoughSymbols(const string &line, const vector<int> &nums)
    {
        string cpy = line;
        vector<string> chunks = CCL::IO::SplitLine(cpy, "?#");

        int existingSymbols, requiredSymbols;
        for (auto it = chunks.begin(); it != chunks.end(); it++) existingSymbols += (*it).size();
        for (auto it = nums.begin(); it != nums.end(); it++) requiredSymbols += *it;

        return existingSymbols >= requiredSymbols;
    }
    /* Check if line fits the description */
    bool CheckLine(const string &line, const vector<int> nums)
    {
        // Split line into chunks
        int first, last;
        vector<string> chunks = CCL::IO::SplitLine(line, "#");

        int j = 0;
        for (int i = 0; i < chunks.size(); i++)
        {
            if (chunks[i].size() != nums[j++])
                return false;
        }

        if (j < nums.size())
            return false;
        else
            return true;
    }

    /* Write solution to cout */
    void WriteLine(const string &line, const vector<string> &solutions)
    {
        cout << "Input: " << endl
             << line << endl;

        cout << "Solutions: " << endl;
        for (auto it = solutions.begin(); it != solutions.end(); it++)
            cout << *it << endl;
        cout << endl;
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

    SpringParser p = SpringParser(input);
    p.ParseInput();
    p.GetScore();

    return 0;
}