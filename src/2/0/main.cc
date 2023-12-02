#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#define CHAR0 48
using namespace std;

int strToNum(string numStr) {
    int num = 0, exp = 1;
    for (int i = numStr.size()-1; i >= 0; i--) {
        int di = (int)numStr[i] - CHAR0;
        if (di < 0 || di > 9) return -1;
        num += di * exp;
        exp *= 10;
    }
    return num;
}

class GameParser {
    public:
        GameParser(const string &_input) {
            input = _input;
        }

        void Parse() {
            // Open file
            ifstream file;
            file.open(input.c_str(), ios::out);

            if (!file.is_open()) {
                cout << "Could not open file.\n";
                return;
            }

            // Read lines
            string line;
            while(file.good()) {
                getline(file, line);
                ParseLine(line);
            }

            // Clean up
            file.close();
        }
    private:
        string input;
        vector<int> games;
        const char *gameHeader = " ";
        const char headerSep = ':';
        const char pullSep = ';';

        void ParseLine(const string &line) {
            // Chop line up into pulls by separator ";"
            string header;
            vector<string> pulls;
            int gameID;
            ChopLine(line, header, pulls);
            ParseHeader(header, gameID);
        }

        void ChopLine(const string &line, string &header, vector<string> &pulls) {
            // Chop off game number
            int chopHeader = line.find(headerSep);
            header = line.substr(0, chopHeader);

            // Chop line into pulls
            int chopBegin = chopHeader + 2;
            int chopEnd = chopBegin + 1;
            pulls.clear();
            while (chopBegin + 1 < line.size() && chopEnd > 0) {
                chopEnd = line.find(pullSep, chopBegin + 1);                
                pulls.push_back(line.substr(chopBegin, chopEnd-chopBegin));
                chopBegin = chopEnd + 2;
            }

            // View results
            for (const auto p : pulls) {
                cout << p << endl;
            }
        }

        void ParseHeader(const string &header, int &gameID) {
            int IDBegin = header.find(gameHeader);
            string numStr = header.substr(IDBegin+1);
            gameID = strToNum(numStr);
        }

        void ParseChop(const string &pull, int values[3]) {

        }
};


int main(int argc, char *argv[]) {
    std::string input = "";
    std::vector<int> vals;

    if (argc < 2) {
        std::cout << "Not enough arguments were provided.\n";
        std::cout << "Usage: ./a.x <input_file>.\n";
        return -1;
    } else {
        input = argv[1];
    }

    GameParser gp = GameParser(input);
    gp.Parse();

    return 0;
}