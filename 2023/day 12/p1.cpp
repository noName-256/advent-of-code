#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include <sstream>

using namespace std;
ifstream fin("p1.in");
ofstream fout("p1.out");

bool validCombination(string &s, vector<int> &requirements)
{
    regex springRegex("#+");
    auto springLengthRequirementIterator = requirements.begin();
    auto springsBegin = sregex_iterator(s.begin(), s.end(), springRegex);
    auto springsEnd = sregex_iterator();
    for (auto i = springsBegin; i != springsEnd; i++)
    {
        int springLength = i->str().length();
        int springLengthRequirement = *springLengthRequirementIterator;
        if (springLengthRequirementIterator == requirements.end())
            return false;
        if (springLength != springLengthRequirement)
            return false;
        springLengthRequirementIterator++;
    }
    if (springLengthRequirementIterator != requirements.end())
        return false;
    return true;
}
int possibleCombinations(string &s, vector<int> &requirements)
{
    int i = 0;
    while (s[i] != '?')
    {
        if (s[i] == '\0')
            return validCombination(s, requirements) ? 1 : 0;
        i++;
    }
    s[i] = '#';
    int possibleCombinationsWithChoice1 = possibleCombinations(s, requirements);
    s[i] = '.';
    int possibleCombinationsWithChoice2 = possibleCombinations(s, requirements);
    s[i] = '?';
    return possibleCombinationsWithChoice1 + possibleCombinationsWithChoice2;
}
bool readInputLine(string &s, vector<int> &requirements)
{
    string line;
    if (!getline(fin, line))
        return false;
    stringstream lineStream(line);
    char comma;
    int springLengthRequirement;
    lineStream >> s;
    requirements.clear();
    do
    {
        lineStream >> springLengthRequirement;
        requirements.push_back(springLengthRequirement);
    } while (lineStream >> comma);
    return true;
}
int main()
{
    string s;
    vector<int> requirements;
    int possibleCombinationsSum = 0;
    while (readInputLine(s, requirements))
        possibleCombinationsSum += possibleCombinations(s, requirements);
    fout << possibleCombinationsSum;
}