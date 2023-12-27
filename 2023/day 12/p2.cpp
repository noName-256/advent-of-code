#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include <sstream>
#include <map>

using namespace std;
ifstream fin("p2.in");
ofstream fout("p2.out");
struct DataPoint
{
    vector<int> currentRequirements;
    string currentS;
    DataPoint(string &s, vector<int> &requirements, int sIndex, int requirementsIndex)
    {
        for (int i = requirementsIndex; i < requirements.size(); i++)
            currentRequirements.push_back(requirements.at(i));
        currentS = s.substr(sIndex);
    }
};
int vectorCompare(const vector<int> &v1, const vector<int> &v2)
{
    int sizeDiff = v1.size() - v2.size();
    if (sizeDiff)
        return sizeDiff;
    for (int i = 0; i < v1.size(); i++)
        if (v1.at(i) < v2.at(i))
            return -1;
        else if (v1.at(i) > v2.at(i))
            return 1;
    return 0;
}
struct
{
    struct DataPointCmp
    {
        bool operator()(DataPoint d1, DataPoint d2) const
        {
            int currentSComparison = d1.currentS.compare(d2.currentS);
            if (currentSComparison)
                return currentSComparison < 0;
            int vectorComparison = vectorCompare(d1.currentRequirements, d2.currentRequirements);
            return vectorComparison < 0;
        }
    };
    map<DataPoint, long long, DataPointCmp> m;
    long long getCachedValue(DataPoint d)
    {
        auto it = m.find(d);
        if (it != m.end())
            return it->second;
        return -1;
    }
    void add(DataPoint d, long long value)
    {
        m.insert(pair<DataPoint, long long>(d, value));
    }
} cache;

bool indexHasPossibleSpring(string &s, int index, int springLength)
{
    if (index + springLength - 1 >= s.length())
        return false;
    if (s[index - 1] == '#')
        return false;
    if (s[index + springLength] == '#')
        return false;
    for (int i = 0; i < springLength; i++)
        if (s[index + i] == '.')
            return false;
    return true;
}
bool indexHasForcedSpring(string &s, int index, int springLength)
{
    for (int i = 0; i < springLength; i++)
        if (s[index + i] != '#')
            return false;
    return true;
}
int findFirstForcedSpring(string &s, int start, int springLength)
{
    int forcedSpringIndex = start;
    while (s[forcedSpringIndex] != '#' && forcedSpringIndex < s.length())
        forcedSpringIndex++;
    return forcedSpringIndex;
}
vector<int> findIndexesWithPossibleSpring(string &s, int start, int springLength)
{
    vector<int> indexes;
    int firstForcedSpringIndex = findFirstForcedSpring(s, start, springLength);
    while (start + springLength - 1 < s.length())
    {
        if (start > firstForcedSpringIndex)
            return indexes;
        if (indexHasPossibleSpring(s, start, springLength))
            indexes.push_back(start);
        start++;
    }
    return indexes;
}
bool combinationHasSpringsRemaining(string &s, int sIndex)
{
    while (sIndex < s.length())
    {
        if (s[sIndex] == '#')
            return true;
        sIndex++;
    }
    return false;
}
int findSpringSpaceRequirements(vector<int> &requirements, int fromWhatRequirement)
{
    int spaceForSpringRequirements = 0;
    for (int i = fromWhatRequirement; i < requirements.size(); i++)
        spaceForSpringRequirements += 1 + requirements.at(i);
    return spaceForSpringRequirements;
}
long long possibleCombinations(string &s, vector<int> &requirements, int sIndex = 0, int requirementsIndex = 0)
{
    if (requirementsIndex >= requirements.size())
        if (combinationHasSpringsRemaining(s, sIndex))
            return 0;
        else
            return 1;
    if (sIndex >= s.length())
        return 0;
    DataPoint d(s, requirements, sIndex, requirementsIndex);
    long long cachedResult = cache.getCachedValue(d);
    if (cachedResult != -1)
        return cachedResult;
    int currentRequirement = requirements.at(requirementsIndex);
    long long nr = 0;
    vector<int> validSpringIndexes = findIndexesWithPossibleSpring(s, sIndex, currentRequirement);
    int springSpaceRequirements = findSpringSpaceRequirements(requirements, requirementsIndex + 1);
    int maximumSpringIndex = s.length() - springSpaceRequirements;
    for (int validSpringIndex : validSpringIndexes)
    {
        if (validSpringIndex <= maximumSpringIndex)
        {
            int nextIndex = validSpringIndex + currentRequirement + 1;
            nr += possibleCombinations(s, requirements, nextIndex, requirementsIndex + 1);
        }
    }
    cache.add(d, nr);
    return nr;
}
void unfoldInputLine(string &s, vector<int> &requirements)
{
    string sCopy = s;
    vector<int> requirementsCopy = requirements;
    for (int i = 1; i < 5; i++)
    {
        s.insert(s.end(), 1, '?');
        s.insert(s.end(), sCopy.begin(), sCopy.end());
        requirements.insert(requirements.end(), requirementsCopy.begin(), requirementsCopy.end());
    }
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
    unfoldInputLine(s, requirements);
    return true;
}
int main()
{
    string s;
    vector<int> requirements;
    long long possibleCombinationsSum = 0;
    while (readInputLine(s, requirements))
        possibleCombinationsSum += possibleCombinations(s, requirements);
    fout << possibleCombinationsSum;
}