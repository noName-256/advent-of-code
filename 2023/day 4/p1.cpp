#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <math.h>
#include <set>

using namespace std;
ifstream fin("p1.in");
ofstream fout("p1.out");

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
int cardValue(string card)
{
    size_t colonPosition = card.find(":"), barPosition = card.find("|");
    string winningNumbers = card.substr(colonPosition + 2, barPosition - colonPosition - 3);
    string numbers = card.substr(barPosition + 2);
    return pow(2, countWinningNumbers(winningNumbers, numbers) - 1);
}
int readCardsAndGetValueSum()
{
    string card;
    int valueSum = 0;
    while (getline(fin, card))
    {
        valueSum += cardValue(card);
    }
    return valueSum;
}
int main()
{
    fout << readCardsAndGetValueSum();
    return 0;
}