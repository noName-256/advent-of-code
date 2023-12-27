#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include <unordered_map>

using namespace std;
ifstream fin("p2.in");
ofstream fout("p2.out");

int hashString(const string &s)
{
    int hash = 0;
    for (const char &c : s)
        hash = (hash + (int)c) * 17 % 256;
    return hash;
}
struct stringHasher
{
    size_t operator()(const string &s) const
    {
        return hashString(s);
    }
};
bool contains(unordered_map<string, int, stringHasher> &m, string &s)
{
    return m.find(s) != m.end();
}
void proccessString(unordered_map<string, int, stringHasher> &m, string &s)
{
    bool isLensRemoval = s[s.length() - 1] == '-';
    string lensName = s.substr(0, s.length() - (isLensRemoval ? 1 : 2));
    if (isLensRemoval)
    {
        if (contains(m, lensName))
            m.erase(lensName);
    }
    else
    {
        int lensFocalPoint = s[s.length() - 1] - '0';
        if (contains(m, lensName))
            m[lensName] = lensFocalPoint;
        else
            m.insert(pair<string, int>(lensName, lensFocalPoint));
    }
}
int getTotalFocusingPower(unordered_map<string, int, stringHasher> &m)
{
    int total = 0;
    for (int i = 0; i < m.bucket_count(); i++)
    {
        int orderNumberInBox = 1;
        int boxElementsCount = m.bucket_size(i);
        for (auto it = m.begin(i); it != m.end(); it++)
        {
            total += (hashString(it->first) + 1) *
                     (boxElementsCount - orderNumberInBox + 1) *
                     (it->second);
            orderNumberInBox++;
        }
    }
    return total;
}
vector<string> readStrings()
{
    string fullInput;
    vector<string> strings;
    fin >> fullInput;
    regex stringRegex("[^,]+(?=(,|$))");
    auto strings_matches_begin = sregex_iterator(fullInput.begin(), fullInput.end(), stringRegex);
    auto strings_matches_end = sregex_iterator();
    for (auto i = strings_matches_begin; i != strings_matches_end; i++)
        strings.push_back(i->str());
    return strings;
}
int main()
{
    vector<string> inputs = readStrings();
    unordered_map<string, int, stringHasher> m;
    for (string &input : inputs)
        proccessString(m, input);
    int total = getTotalFocusingPower(m);
    fout << total;
}