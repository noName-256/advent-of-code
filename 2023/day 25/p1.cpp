#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include <set>
#include <map>

#define SAMPLE_INPUT_PAIRS_TO_REMOVE make_pair("hfx", "pzl"), make_pair("bvb", "cmg"), make_pair("nvd", "jqt")
#define REAL_INPUT_PAIRS_TO_REMOVE make_pair("fht", "vtt"), make_pair("bbg", "kbr"), make_pair("czs", "tdk")

using namespace std;
ifstream fin("p1.in");
ofstream fout("p1.out");

const vector<pair<string, string>> pairsToRemoveFromGraph{REAL_INPUT_PAIRS_TO_REMOVE};

void readComponent(const string &inputRow, string &firstComponent, vector<string> &nextComponents)
{
    firstComponent = inputRow.substr(0, 3);
    nextComponents.clear();
    string next = inputRow.substr(5);
    regex componentRegex("[a-zA-Z]{3}(?=( |$))");
    auto component_matches_begin = sregex_iterator(next.begin(), next.end(), componentRegex);
    auto component_matches_end = sregex_iterator();
    for (auto it = component_matches_begin; it != component_matches_end; it++)
    {
        string nextComponent = it->str();
        nextComponents.push_back(nextComponent);
    }
}
void addComponentPairToGraph(map<string, vector<string>> &components, const string &c1, const string &c2)
{
    auto findC1 = components.find(c1);
    bool c1InGraph = findC1 != components.end();
    if (c1InGraph)
        findC1->second.push_back(c2);
    else
    {
        vector<string> nextForC1;
        nextForC1.push_back(c2);
        components.insert(make_pair(c1, nextForC1));
    }
}
void removeComponentPairFromGraph(map<string, vector<string>> &components, const string &c1, const string &c2)
{
    auto findC1 = components.find(c1);
    auto &c1Next = findC1->second;
    c1Next.erase(remove(c1Next.begin(), c1Next.end(), c2), c1Next.end());

    auto findC2 = components.find(c2);
    auto &c2Next = findC2->second;
    c2Next.erase(remove(c2Next.begin(), c2Next.end(), c1), c2Next.end());
}
void getConnectedComponent(const map<string, vector<string>> &graph, const string &node, set<string> &passedNodes, set<string> &connectedComponent)
{
    // use DFS to get connected component in graph
    bool passedCurrentNode = passedNodes.find(node) != passedNodes.end();
    if (passedCurrentNode)
        return;
    connectedComponent.insert(node);
    passedNodes.insert(node);
    const vector<string> &nextNodes = graph.find(node)->second;
    for (const string &nextNode : nextNodes)
        getConnectedComponent(graph, nextNode, passedNodes, connectedComponent);
}
vector<int> graphConnectedComponentSizes(const map<string, vector<string>> &graph)
{
    vector<set<string>> connectedComponents;
    set<string> passedNodes;
    for (const auto &nodePair : graph)
    {
        const string &node = nodePair.first;
        set<string> connectedComponent;
        bool passedCurrentNode = passedNodes.find(node) != passedNodes.end();
        if (passedCurrentNode)
            continue;
        getConnectedComponent(graph, node, passedNodes, connectedComponent);
        connectedComponents.push_back(connectedComponent);
    }
    vector<int> connectedComponentsSizes;
    for (const set<string> &connectedComponent : connectedComponents)
        connectedComponentsSizes.push_back(connectedComponent.size());
    return connectedComponentsSizes;
}
void readComponents(map<string, vector<string>> &components)
{
    string inputRow, firstComponent;
    vector<string> nextComponents;
    // fout << "Graph G{\n\n";
    while (getline(fin, inputRow))
    {
        if (inputRow.empty())
            break;
        readComponent(inputRow, firstComponent, nextComponents);
        for (const string &nextComponent : nextComponents)
        {
            addComponentPairToGraph(components, firstComponent, nextComponent);
            addComponentPairToGraph(components, nextComponent, firstComponent);
        }
        // fout << firstComponent << " -- " << nextComponent << endl;
    }
    // fout << "\n}\n";
}
int main()
{
    map<string, vector<string>> components;
    readComponents(components);
    for (const auto &pairToRemove : pairsToRemoveFromGraph)
        removeComponentPairFromGraph(components, pairToRemove.first, pairToRemove.second);
    vector<int> connectedComponentsSizes = graphConnectedComponentSizes(components);
    int sizesProduct = 1;
    for (const int &size : connectedComponentsSizes)
        sizesProduct *= size;
    fout << sizesProduct;
}
/*

Remove the following connections:
fht -- vtt
bbg -- kbr
czs -- tdk

 */