#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>

using namespace std;
ifstream fin("p1.in");
ofstream fout("p1.out");

int hashString(string s)
{
    int hash = 0;
    for (char &c : s)
        hash = (hash + (int)c) * 17 % 256;
    return hash;
}
vector<string> readStrings()
{
    string fullInput;
    vector<string> strings;
    fin >> fullInput;
    regex stringRegex("[^,]*(?=(,|$))");
    auto strings_matches_begin = sregex_iterator(fullInput.begin(), fullInput.end(), stringRegex);
    auto strings_matches_end = sregex_iterator();
    for (auto i = strings_matches_begin; i != strings_matches_end; i++)
        strings.push_back(i->str());
    return strings;
}

int main()
{
    vector<string> inputs = readStrings();
    int sum = 0;
    for (string &input : inputs)
        sum += hashString(input);
    fout << sum;
}