#include <iostream>
#include <fstream>
#include <string>
#include <regex>

using namespace std;
ifstream fin("p2.in");
ofstream fout("p2.out");

int calculateGameNumber(const string &input, const regex &gameRegex)
{
    smatch matches;
    regex_search(input, matches, gameRegex);
    return stoi(matches[0]);
}
int calculateColorSum(const string &input, const regex &colorRegex)
{
    smatch matches;
    int sum = 0;
    auto matches_begin = sregex_iterator(input.begin(), input.end(), colorRegex);
    auto matches_end = sregex_iterator();
    for (sregex_iterator i = matches_begin; i != matches_end; i++)
    {
        smatch match = *i;
        const int number = stoi(match.str());
        sum += number;
    }
    return sum;
}
int gamePower(const string &input, const regex &blue_regex, const regex &red_regex, const regex &green_regex)
{
    smatch matches;
    regex bagRegex("[0-9][0-9a-z, ]+(?=;|$)");
    int maxBlue = 0, maxRed = 0, maxGreen = 0;
    auto matches_begin = sregex_iterator(input.begin(), input.end(), bagRegex);
    auto matches_end = sregex_iterator();
    for (sregex_iterator i = matches_begin; i != matches_end; i++)
    {
        smatch match = *i;
        string bag = match.str();
        const int blueSum = calculateColorSum(bag, blue_regex);
        const int redSum = calculateColorSum(bag, red_regex);
        const int greenSum = calculateColorSum(bag, green_regex);
        maxBlue = max(maxBlue, blueSum);
        maxRed = max(maxRed, redSum);
        maxGreen = max(maxGreen, greenSum);
    }
    return maxBlue * maxRed * maxGreen;
}
int main()
{
    string input_row;
    regex game_regex("[0-9]+");
    regex blue_regex("[0-9]+(?= blue)");
    regex red_regex("[0-9]+(?= red)");
    regex green_regex("[0-9]+(?= green)");
    int gamePowerSum = 0;
    while (getline(fin, input_row))
    {
        input_row.replace(input_row.begin(), input_row.begin() + input_row.find(":") + 2, ""); // remove game part
        gamePowerSum += gamePower(input_row, blue_regex, red_regex, green_regex);
    }
    fout << gamePowerSum;
}