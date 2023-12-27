#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <map>
#include <set>

using namespace std;
ifstream fin("p2.in");
ofstream fout("p2.out");
void proccessCombo(int &handType, int combo)
{
    // combo represents the number of identical cards in a row
    switch (combo)
    {
    case 1:
        // nothing happens if you get a new high card
        // as it's the lowest possible combo
        return;
    case 2:
        switch (handType)
        {
        case 1:
            handType = 2;
            return;
        case 2:
            handType = 3;
            return;
        case 4:
            handType = 5;
            return;
        default:
            // this should not be reached
            // as there are a maximum of 5 characters in a hand
            cout << "ERROR in proccessCombo: this should not be reached\n";
            return;
        }
    case 3:
        switch (handType)
        {
        case 1:
            handType = 4;
            return;
        case 2:
            handType = 5;
            return;
        default:
            // this should not be reached
            // as there are a maximum of 5 characters in a hand
            cout << "ERROR in proccessCombo: this should not be reached\n";
            return;
        }
    case 4:
        handType = 6; // four of a kind
        return;
    case 5:
        handType = 7; // five of a kind
        return;
    default:
        // this should not be reached
        // as there are a maximum of 5 characters in a hand
        cout << "ERROR in proccessCombo: this should not be reached\n";
        return;
    }
}
void proccessJokers(int &handType, int numberOfJokers)
{
    switch (numberOfJokers)
    {
    case 0:
        return;
    case 1:
        switch (handType)
        {
        case 1:
            handType = 2; // high card(1) becomes one pair(2)
            return;
        case 2:
            handType = 4; // one pair(2) becomes three of a kind(4)
            return;
        case 3:
            handType = 5; // two pair(3) becomes full house(5)
            return;
        case 4:
            handType = 6; // three of a kind(4) becomes four of a kind(6)
            return;
        case 6:
            handType = 7; // four of a kind(6) becomes five of a kind(7)
            return;
        default:
            cout << "ERROR in proccessJokers: this should not be reached\n";
            return;
        }
    case 2:
        switch (handType)
        {
        case 1:
            handType = 4; // high card(1) becomes three of a kind(4)
            return;
        case 2:
            handType = 6; // one pair(2) becomes four of a kind(6)
            return;
        case 4:
            handType = 7; // three of a kind(4) becomes five of a kind(7)
            return;
        default:
            cout << "ERROR in proccessJokers: this should not be reached\n";
            return;
        }
    case 3:
        switch (handType)
        {
        case 1:
            handType = 6; // high card(1) becomes four of a kind(6)
            return;
        case 2:
            handType = 7; // one pair(2) becomes five of a kind(7)
            return;
        default:
            cout << "ERROR in proccessJokers: this should not be reached\n";
            return;
        }
    case 4:
        switch (handType)
        {
        case 1:
            handType = 7; // high card(1) becomes five of a kind(7)
            return;
        default:
            cout << "ERROR in proccessJokers: this should not be reached\n";
            return;
        }

    case 5:
        switch (handType)
        {
        case 1:
            // since there are only five cards it means they are all jokers
            // so there should be no hand type before counting jokers
            // but the default is high card(1) that is why we use this as the only possible case
            // because there are five jokers, this is a five of a kind(7)
            handType = 7;
            return;
        default:
            cout << "ERROR in proccessJokers: this should not be reached\n";
            return;
        }
    }
}
string handTypeValueToText(int handType)
{
    switch (handType)
    {
    case 1:
        return "high card";
    case 2:
        return "one pair";
    case 3:
        return "two pair";
    case 4:
        return "three of a kind";
    case 5:
        return "full house";
    case 6:
        return "four of a kind";
    case 7:
        return "five of a kind";
    default:
        return "invalid handType";
    }
    return "invalid handType";
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
    int numberOfJokers = 0;
    for (int i = 0; i < sortedHand.length(); i++)
    {
        // count number of identical letters from i
        // and skip them so that you don't have to count them again
        int nr = 0, j;
        bool isJoker = sortedHand[i] == 'J';
        for (j = i; j < sortedHand.length(); j++)
        {
            if (sortedHand[i] != sortedHand[j])
                break;
            nr++;
        }
        if (isJoker)
        {
            // we don't proccess jokers yet
            // we will do that as a special case later
            // just remember how many of them are
            numberOfJokers = nr;
        }
        else
            proccessCombo(type, nr);
        i = j - 1;
    }
    proccessJokers(type, numberOfJokers);
    return type;
}
int cardToValue(const char &card)
{
    switch (card)
    {
    case 'A':
        return 13;
    case 'K':
        return 12;
    case 'Q':
        return 11;
    case 'J':
        return 0; // nice easy tweak lol
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
    cout << "ERROR in cardToValue: this should not be reached\n";
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
        // fout << "Round: " << roundNumber << ":\nhand:  " << round.first << "\nhandType: " << handType(round.first) << "\nbid: " << round.second << "\n\n";
        const int bid = round.second, rank = roundNumber;
        totalWinnings += rank * bid;
        roundNumber++;
    }
    fout << totalWinnings;
    return 0;
}