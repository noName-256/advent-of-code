#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <regex>
#include <vector>

using namespace std;
ifstream fin("p2.in");
ofstream fout("p2.out");

struct Node
{
    string left, right;
    Node(const string &_left, const string &_right)
    {
        left = _left;
        right = _right;
    }
};
struct Ghost
{
    // one ghost that can traverse nodes
    string startingLocation, location;
    int index;
    int steps;
    int cycleSteps;
    void moveLocation(const string &directions, const map<string, Node> &m)
    {
        char dir = directions[index];
        Node n = m.find(location)->second;
        if (dir == 'L')
            location = n.left;
        else
            location = n.right;
        steps++;
        index++;
        if (index == directions.length())
            index = 0; // go back to index 0 when reaching the end of directions
        if (goodLocation() && !hasCycle())
            cycleSteps = steps;
    }
    bool goodLocation() const
    {
        return location[2] == 'Z';
    }
    bool hasCycle() const
    {
        return cycleSteps != -1;
    }
    Ghost(string _startingLocacion, int _index = 0)
    {
        startingLocation = _startingLocacion;
        location = startingLocation;
        index = _index;
        steps = 0;
        cycleSteps = -1; // by default it does not have a cycle
    }
};
string getDirections()
{
    string directions;
    getline(fin, directions);
    return directions;
}
map<string, Node> getMap()
{
    map<string, Node> m;
    string inputRow;
    while (getline(fin, inputRow))
    {
        if (inputRow.length() != 0)
        {
            // found map node
            regex originRegex("[A-Z0-9]{3}(?= )");
            regex leftRegex("[A-Z0-9]{3}(?=,)");
            regex rightRegex("[A-Z0-9]{3}(?=\\))");
            smatch originMatch;
            smatch leftMatch;
            smatch rightMatch;
            regex_search(inputRow, originMatch, originRegex);
            regex_search(inputRow, leftMatch, leftRegex);
            regex_search(inputRow, rightMatch, rightRegex);

            string origin = originMatch.str();
            string left = leftMatch.str();
            string right = rightMatch.str();
            Node newNode(left, right);
            m.insert(pair<string, Node>(origin, newNode));
        }
    }
    return m;
}
vector<string> getStartingLocations(const map<string, Node> &m)
{
    vector<string> startingLocations;
    for (const auto &i : m)
    {
        string location = i.first;
        if (location[2] == 'A')
            startingLocations.push_back(location);
    }
    return startingLocations;
}
vector<Ghost> getGhosts(const map<string, Node> &m)
{
    vector<string> startingLocations = getStartingLocations(m);
    vector<Ghost> ghosts;
    for (const string &startingLocation : startingLocations)
    {
        Ghost newGhost(startingLocation);
        ghosts.push_back(newGhost);
    }
    return ghosts;
}
bool allGhostsOnGoodLocations(const vector<Ghost> &ghosts)
{
    for (const Ghost &ghost : ghosts)
        if (!ghost.goodLocation())
            return false;
    return true;
}
bool allGhostsHaveCycle(const vector<Ghost> &ghosts)
{
    for (const Ghost &ghost : ghosts)
        if (!ghost.hasCycle())
            return false;
    return true;
}
long long greatestCommonDivisor(long long x, long long y)
{
    return y == 0 ? x : greatestCommonDivisor(y, x % y);
}
long long leastCommonMultiple(long long x, long long y)
{
    if (y > x)
        swap(x, y);
    return x / greatestCommonDivisor(x, y) * y;
}
long long getNumberOfSteps(const string &directions, const map<string, Node> &m)
{
    vector<Ghost> ghosts = getGhosts(m);
    int steps = 0;
    while (!allGhostsOnGoodLocations(ghosts) && !allGhostsHaveCycle(ghosts))
    {
        for (Ghost &ghost : ghosts)
            ghost.moveLocation(directions, m);
        steps++;
    }
    if (allGhostsOnGoodLocations(ghosts))
        return steps;
    else
    {
        // we have to calculate the smallest common multiple of all cycles
        long long lcm = 1;
        for (const Ghost &ghost : ghosts)
            lcm = leastCommonMultiple(lcm, (long long)ghost.cycleSteps);
        return lcm;
    }
}
int main()
{
    const string directions = getDirections();
    const map<string, Node> m = getMap();
    fout << getNumberOfSteps(directions, m);
    return 0;
}