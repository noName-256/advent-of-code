#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <math.h>
#include <set>

using namespace std;
ifstream fin("p2.in");
ofstream fout("p2.out");
int cardCopies[500]; // counts the number of copies of the card at index
int countWinningNumbers(string winningNumbers, string numbers)
{
    regex digitRegex("[0-9]+");
    set<int> winningNumbersSet;
    int nr = 0;
    auto matches_begin = sregex_iterator(winningNumbers.begin(), winningNumbers.end(), digitRegex);
    auto matches_end = sregex_iterator();
    for (auto i = matches_begin; i != matches_end; i++)
    {
        smatch match = *i;
        winningNumbersSet.insert(stoi(match.str()));
    }
    matches_begin = sregex_iterator(numbers.begin(), numbers.end(), digitRegex);
    matches_end = sregex_iterator();
    for (auto i = matches_begin; i != matches_end; i++)
    {
        smatch match = *i;
        int number = stoi(match.str());
        bool numberIsWinning = winningNumbersSet.find(number) != winningNumbersSet.end();
        if (numberIsWinning)
            nr++;
    }
    return nr;
}
void proccessCard(string card)
{
    regex digitRegex("[0-9]+");
    smatch cardIDMatch;
    size_t colonPosition = card.find(":"), barPosition = card.find("|");
    regex_search(card, cardIDMatch, digitRegex);

    int cardID = stoi(cardIDMatch.str());
    string winningNumbers = card.substr(colonPosition + 2, barPosition - colonPosition - 3);
    string numbers = card.substr(barPosition + 2);
    int numberOfWinningNumbers = countWinningNumbers(winningNumbers, numbers);

    for (int i = cardID + 1; i <= cardID + numberOfWinningNumbers; i++)
        cardCopies[i] += cardCopies[cardID];
}
int readCardsAndGetNumberOfCopies()
{
    string card;
    int numberOfCopies = 0, numberOfCards = 0;
    while (getline(fin, card))
    {
        numberOfCards++;
        cardCopies[numberOfCards]++;
        proccessCard(card);
    }
    for (int i = 1; i <= numberOfCards; i++)
        numberOfCopies += cardCopies[i];

    return numberOfCopies;
}
int main()
{
    fout << readCardsAndGetNumberOfCopies();
    return 0;
}