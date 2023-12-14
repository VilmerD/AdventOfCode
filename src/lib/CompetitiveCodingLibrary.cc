#include "CompetitiveCodingLibrary.hh"

using namespace std;


std::string CCL::IO::GetFirstNotOf(const std::string &line, const char *ID = CCL::IO::NUMBERS)
{
    return line.substr(0, line.find_first_not_of(ID, 0));
}
std::vector<std::string> CCL::IO::SplitLine(const std::string &line, const char *ID = CCL::IO::NUMBERS)
{
    int first = 0, last;
    std::vector<std::string> chunks = {};

    while (last != line.size())
    {
        first = line.find_first_of(ID, first);
        last = line.find_first_not_of(ID, first);
        last = (last == -1) ? line.size() : last;

        if (first == -1)
            break;
        chunks.push_back(line.substr(first, last - first));

        first = last + 1;
    }

    return chunks;
}
std::vector<long> CCL::IO::SplitLongs(const std::string &line)
{
    std::vector<std::string> chunks = CCL::IO::SplitLine(line);
    std::vector<long> nums = {};
    for (auto it = chunks.begin(); it != chunks.end(); it++)
        nums.push_back(std::stol(*it));
    return nums;
}
std::vector<int> CCL::IO::SplitInts(const std::string &line)
{
    std::vector<std::string> chunks = CCL::IO::SplitLine(line);
    std::vector<int> nums = {};
    for (auto it = chunks.begin(); it != chunks.end(); it++)
        nums.push_back(std::stoi(*it));
    return nums;
}

CCL::IO::Parser::Parser(const string &_input) { input = _input; };
void CCL::IO::Parser::ParseInput()
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

    ParseFooter();

    // Clean up
    file.close();
}