#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <set>

#include <thread>

using namespace std;

#define USEJOKER 1
const string NUMBERS = "0123456789";

#if USEJOKER
const string CARDS = "J23456789TQKA";
#else
const string CARDS = "23456789TJQKA";
#endif

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

class Card
{
public:
    Card()
    {
        val = 0;
    }
    Card(const char &_val)
    {
        val = _val;
    }
    friend bool operator<(const Card &lhs, const Card &rhs)
    {
        return lhs.CardToValue() < rhs.CardToValue();
    }
    friend ostream &operator<<(ostream &os, const Card &out)
    {
        os << out.val;
        return os;
    }
    friend bool operator==(const Card &lhs, const Card &rhs)
    {
        return lhs.val == rhs.val;
    }

private:
    char val;
    const int CardToValue() const { return CARDS.find(val); };
};

typedef enum _CARD_HAND
{
    HIGH_CARD,
    ONE_PAIR,
    TWO_PAIR,
    THREE_OF_KIND,
    FULL_HOUSE,
    FOUR_OF_KIND,
    FIVE_OF_KIND,
} CARD_HAND;

class Hand
{
public:
    Hand(const string &_cards, long _bet)
    {
        cardsStr = _cards;
        bet = _bet;

        // Get vector of cards
        cards.clear();
        for (const char &c : cardsStr)
            cards.push_back(Card(c));
        sort(cards.begin(), cards.end());

        // Get number of cards of each type
        for (int i = 0; i < CARDS.size(); i++)
        {
            const Card ci = Card(CARDS[i]);
            counts.push_back(count(cards.begin(), cards.end(), ci));
        }

        handType = HandGetHandType();
    }

    inline long GetBet() const { return bet; }

    friend ostream &operator<<(ostream &os, const Hand &obj)
    {
        os << "Cards: " << obj.cardsStr << " Bet: " << obj.bet;
        os << " Counts: ";
        for (const int &c : obj.counts)
            os << c << " ";
        os << " Hand type: " << obj.handType;
        return os;
    }

    friend bool operator<(const Hand &lhs, const Hand &rhs)
    {
        if (lhs.handType != rhs.handType)
        {
            return lhs.handType > rhs.handType;
        }
        else
        {
            return HighestCard(lhs, rhs);
        }
    }

    static inline bool HighestCard(const Hand &lhs, const Hand &rhs)
    {
        Card cl, cr;
        for (int i = 0; i < lhs.cardsStr.size(); i++)
        {
            cl = Card(lhs.cardsStr[i]);
            cr = Card(rhs.cardsStr[i]);
            if (!(cl == cr))
                return !(cl < cr);
        }
        return true;
    }

    template <int N>
    bool HasNOfKind() const
    {
        for (int i = 0; i < counts.size(); i++)
        {
            int ci = counts[i];
#if USEJOKER
            if ((i == 0) && (ci == N))
                return true;
            else if ((i != 0) && (ci + counts[0] >= N))
                return true;
#else
            if (ci == N)
                return true;
#endif
        }
        return false;
    }
    bool HasFiveOfKind() const { return HasNOfKind<5>(); }
    bool HasFourOfKind() const { return HasNOfKind<4>(); }
    bool HasThreeOfKind() const { return HasNOfKind<3>(); }
    bool HasPair() const { return HasNOfKind<2>(); }

    bool HasFullHouse() const
    {
        bool hasTrips = HasThreeOfKind();
        bool hasPair = HasPair();

#if USEJOKER
        // Check that joker was not used for both trips and pair
        if (hasTrips && hasPair)
        {
            if (counts[0] == 0)
            {
                auto p1 = find(counts.begin(), counts.end(), 2);
                if (p1 == counts.end()) return false;
                auto p2 = find(counts.begin(), counts.end(), 3);
                if (p2 == counts.end()) return false;
                return true;
            }
            else if (counts[0] == 1)
            {
                // normal three of a kind
                if (find(counts.begin(), counts.end(), 3) != counts.end())
                    return true;

                // or normal two pair
                auto p1 = find(counts.begin(), counts.end(), 2);
                if (p1 == counts.end())
                    return false;
                auto p2 = find(p1+1, counts.end(), 2);
                if (p2 == counts.end())
                    return false;

                return true;
            }
            else if (counts[0] == 2)
            {
                if (find(counts.begin() + 1, counts.end(), 2) != counts.end())
                    return true;
                return false;
            }
            return true;
        }
        return false;
#else
        return hasTrips && hasPair;
#endif
    }
    bool HasTwoPair() const
    {
        int pairs = 0;
        for (int i = 0; i < counts.size(); i++)
        {
            int ci = counts[i];
#if USEJOKER
            if (ci + counts[0] == 2)
                pairs++;
#else
            if (ci == 2)
                pairs++;
#endif
        }

#if USEJOKER
        bool _hasPair = find(counts.begin(), counts.end(), 2) != counts.end();
        return pairs >= 2 && _hasPair;
#else
        return pairs == 2;
#endif
    }

private:
    int bet;
    string cardsStr;
    vector<Card> cards;
    vector<int> counts;

    CARD_HAND handType;

    CARD_HAND HandGetHandType() const
    {
        if (HasFiveOfKind())
            return FIVE_OF_KIND;
        else if (HasFourOfKind())
            return FOUR_OF_KIND;
        else if (HasFullHouse())
            return FULL_HOUSE;
        else if (HasThreeOfKind())
            return THREE_OF_KIND;
        else if (HasTwoPair())
            return TWO_PAIR;
        else if (HasPair())
            return ONE_PAIR;
        else
            return HIGH_CARD;
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
        // Sort hands based on score
        auto srt = [](const Hand &a, const Hand &b)
        { return !(a < b); };
        sort(hands.begin(), hands.end(), srt);
        for (const Hand &h : hands)
            cout << h << endl;

        // Compute score based on bets
        scoreA = 0;

        for (long i = 0; i < hands.size(); i++)
        {
            scoreA += hands[i].GetBet() * (i + 1L);
        }

        cout << "Score of task A: " << scoreA << endl;
    }

    void GetScoreB()
    {
    }

private:
    string input;
    ifstream file;

    vector<Hand> hands;

    long scoreA = 0, scoreB = 0;

    void ParseHeader()
    {
    }

    void ParseLine()
    {
        string line;
        getline(file, line);

        // First five characters is hand
        string hand = line.substr(0, 5);
        long bet = stol(line.substr(6));
        hands.push_back(Hand(hand, bet));
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