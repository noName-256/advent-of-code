#include <iostream>
#include <map>
#include <string>
#include <regex>
#include <fstream>
using namespace std;
ifstream fin("p1.in");
ofstream fout("p1.out");
struct Node
{
    string left, right;
    Node(const string &l, const string &r)
    {
        left = l;
        right = r;
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
            regex originRegex("[A-Z]{3}(?= )");
            regex leftRegex("[A-Z]{3}(?=,)");
            regex rightRegex("[A-Z]{3}(?=\\))");
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
void moveLocation(const char &dir, string &currentLocation, const map<string, Node> &m)
{
    Node n = m.find(currentLocation)->second;
    if (dir == 'L')
        currentLocation = n.left;
    else
        currentLocation = n.right;
}
int getNumberOfSteps(const string &directions, const map<string, Node> &m)
{
    int steps = 0;
    string currentLocation = "AAA";
    string toLocation = "ZZZ";
    while (currentLocation != toLocation)
    {
        for (const char &dir : directions)
        {
            if (currentLocation == toLocation)
                return steps;
            else
            {
                moveLocation(dir, currentLocation, m);
                steps++;
            }
        }
    }
    return steps;
}
int main()
{
    const string directions = getDirections();
    const map<string, Node> m = getMap();
    fout << getNumberOfSteps(directions, m);
    return 0;
}