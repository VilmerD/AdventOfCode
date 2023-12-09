#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#define NUMBERS "0123456789"
#define NUMCARDS 220

using namespace std;

#define CHAR0 48
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
        for (int i = 0; i < NUMCARDS; i++) {
            tickets.push_back(1);
        }
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
        cout << "Sum of pts: " << sum << endl;
        for (const int & t : tickets) sumTickets += t;
        cout << "Sum of tickets: " << sumTickets << endl;
    }

private:
    string input;
    vector<int> tickets;
    int sum = 0, sumTickets = 0;

    void ParseLine(const string &line)
    {
        string headerStr, winningStr, pulledStr;
        vector<int> winningNbrs, pulledNbrs;
        int nbr, matches;

        // Chop line
        ChopLine(line, headerStr, winningStr, pulledStr);

        // Parse header
        ParseHeader(headerStr, nbr);

        // Parse winning numbers
        ParseNbrs(winningStr, winningNbrs);

        // Parse my numbers
        ParseNbrs(pulledStr, pulledNbrs);

        // Check matching
        CheckNumbers(winningNbrs, pulledNbrs, matches);

        // Get line score
        GetScoreLine(nbr, matches);

        PrintTickets();
    }

    void ChopLine(const string &line, string& header, string &winning, string &pulled) {
        int iheader = line.find(':', 0);
        header = line.substr(0, iheader);

        int idiv = line.find('|');
        winning = line.substr(iheader+2, idiv-iheader-3);
        pulled = line.substr(idiv+2, line.size() - (idiv - iheader + 4));
    }

    void ParseHeader(const string &header, int &nbr)
    {
        int ib, ie = header.size();
        ib = header.find_first_of(NUMBERS, 0);
        nbr = strToNum(header.substr(ib, ie - ib));
    }

    void ParseNbrs(const string &winning, vector<int> &nbrs) {
        int first, last, num;
        first = 0; last = 1;

        nbrs.clear();
        while (last != winning.size()) {
            first = winning.find_first_of(NUMBERS, first);
            last = winning.find_first_not_of(NUMBERS, first);
            last = (last == -1) ? winning.size() : last;

            num = strToNum(winning.substr(first, last-first));

            nbrs.push_back(num);
            first = last+1;
        }
    } 

    void CheckNumbers(const vector<int> &winning, const vector<int> &pulled, int &matches) {
        matches = 0;
        for (const int & w : winning) {
            for (const int & p : pulled) {
                if (w == p) {
                    matches++;
                    break;
                }
            }
        }
    }

    void GetScoreLine(const int& card, const int &matches) {
        cout << "Card " << card << " Matches " << matches << endl;
        for (int i = card; i < card+matches && i < tickets.size(); i++) {
            tickets[i] += tickets[card-1];
        }
        sum += (int)pow(2.0, (double)matches-1);
    }

    void PrintTickets() {
        for (int i = 0; i < tickets.size(); i++) {
            cout << tickets[i] << " ";
        }
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

    Parser gp = Parser(input);
    gp.Parse();
    gp.Print();

    return 0;
}