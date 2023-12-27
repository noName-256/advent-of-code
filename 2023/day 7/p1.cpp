#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <map>
#include <set>

using namespace std;
ifstream fin("p1.in");
ofstream fout("p1.out");
void proccessCombo(int &handType, int combo)
{
    // combo represents the number of identical cards in a row
    switch (combo)
    {
    case 1:
        // nothing happens if you get a new high card
        // as it's the lowest possible combo
        break;
    case 2:
        switch (handType)
        {
        case 1:
            handType = 2;
            break;
        case 2:
            handType = 3;
            break;
        case 4:
            handType = 5;
            break;
        default:
            // this should not be reached
            // as there are a maximum of 5 characters in a hand
            cout << "ERROR: this should not be reached";
            break;
        }
        break;
    case 3:
        switch (handType)
        {
        case 1:
            handType = 4;
            break;
        case 2:
            handType = 5;
            break;
        default:
            // this should not be reached
            // as there are a maximum of 5 characters in a hand
            cout << "ERROR: this should not be reached";
            break;
        }
        break;
    case 4:
        handType = 6; // four of a kind
        break;
    case 5:
        handType = 7; // five of a kind
        break;
    default:
        // this should not be reached
        // as there are a maximum of 5 characters in a hand
        cout << "ERROR: this should not be reached";
        break;
    }
}
int handType(const string hand)
{
    int type = 1;
    // 1 = high card
    // 2 = one pair
    // 3 = two pair
    // 4 = three of a kind
    // 5 = full house
    // 6 = four of a kind
    // 7 = five of a kind
    string sortedHand = hand;
    sort(sortedHand.begin(), sortedHand.end());
    for (int i = 0; i < sortedHand.length(); i++)
    {
        // count number of identical letters from i
        // and skip them so that you don't have to count them again
        int nr = 0, j;
        for (j = i; j < sortedHand.length(); j++)
        {
            if (sortedHand[i] != sortedHand[j])
                break;
            nr++;
        }
        proccessCombo(type, nr);
        i = j - 1;
    }
    return type;
}
int cardToValue(const char &card1)
{
    switch (card1)
    {
    case 'A':
        return 13;
    case 'K':
        return 12;
    case 'Q':
        return 11;
    case 'J':
        return 10;
    case 'T':
        return 9;
    case '9':
        return 8;
    case '8':
        return 7;
    case '7':
        return 6;
    case '6':
        return 5;
    case '5':
        return 4;
    case '4':
        return 3;
    case '3':
        return 2;
    case '2':
        return 1;
    }
    cout << "ERROR: this should not be reached";
    return 1;
}
int compareCards(const char &card1, const char &card2)
{
    int card1Value = cardToValue(card1), card2Value = cardToValue(card2);
    return card1Value - card2Value;
}
struct compareHands
{
    bool operator()(const string &hand1, const string &hand2) const
    {
        int hand1Type = handType(hand1), hand2Type = handType(hand2);
        if (hand1Type > hand2Type)
            return false; // card1 is better
        else if (hand1Type < hand2Type)
            return true; // card2 is better
        else
        {
            for (int i = 0; i < hand1.length(); i++)
            {
                int cardComparison = compareCards(hand1[i], hand2[i]);
                if (cardComparison > 0)
                    return false; // card1 is better
                if (cardComparison < 0)
                    return true; // card2 is better
            }
            return false;
        }
        return false;
    }
};
map<string, int, compareHands> getRounds()
{
    string hand;
    int bid;
    map<string, int, compareHands> rounds;
    while (fin >> hand >> bid)
        rounds.insert(pair<string, int>(hand, bid));
    return rounds;
}
int main()
{
    map<string, int, compareHands> rounds = getRounds();
    int roundNumber = 1, totalWinnings = 0;
    for (const auto &round : rounds)
    {
        // fout << "Round: " << roundNumber << ":\nhand:  " << round.first << "\nbid: " << round.second << "\n\n";
        const int bid = round.second, rank = roundNumber;
        totalWinnings += rank * bid;
        roundNumber++;
    }
    fout << totalWinnings;
    return 0;
}