#include <iostream>
#include <fstream>
#include <string>
#include <regex>

using namespace std;
ifstream fin("p1.in");
ofstream fout("p1.out");

const int MAX_RED_CUBES = 12, MAX_GREEN_CUBES = 13, MAX_BLUE_CUBES = 14;
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
bool stringIsPossible(const string &input, const regex &blue_regex, const regex &red_regex, const regex &green_regex)
{
    smatch matches;
    regex bagRegex("[0-9][0-9a-z, ]+(?=;|$)");
    auto matches_begin = sregex_iterator(input.begin(), input.end(), bagRegex);
    auto matches_end = sregex_iterator();
    for (sregex_iterator i = matches_begin; i != matches_end; i++)
    {
        smatch match = *i;
        string bag = match.str();
        const int blueSum = calculateColorSum(bag, blue_regex);
        const int redSum = calculateColorSum(bag, red_regex);
        const int greenSum = calculateColorSum(bag, green_regex);
        if (blueSum > MAX_BLUE_CUBES)
            return false;
        if (redSum > MAX_RED_CUBES)
            return false;
        if (greenSum > MAX_GREEN_CUBES)
            return false;
    }
    return true;
}
int main()
{
    string input_row;
    regex game_regex("[0-9]+");
    regex blue_regex("[0-9]+(?= blue)");
    regex red_regex("[0-9]+(?= red)");
    regex green_regex("[0-9]+(?= green)");
    int gameNumber, gameNumberSum = 0;
    while (getline(fin, input_row))
    {
        gameNumber = calculateGameNumber(input_row, game_regex);
        input_row.replace(input_row.begin(), input_row.begin() + input_row.find(":") + 2, ""); // remove game part
        if (stringIsPossible(input_row, blue_regex, red_regex, green_regex))
            gameNumberSum += gameNumber;
    }
    fout << gameNumberSum;
}