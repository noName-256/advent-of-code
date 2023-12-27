#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <regex>
#include <queue>

#define FIN_EDGES_COUNT 4

using namespace std;
ifstream fin("p2.in");
ofstream fout("p2.out");
struct Broadcaster
{
    string id;
    vector<string> nextModulesIDs;
    Broadcaster(string inputRow = "")
    {
        if (inputRow.size() == 0)
            return;
        regex idRegex("[a-zA-Z]+");
        auto idMatchesBegin = sregex_iterator(inputRow.begin(), inputRow.end(), idRegex);
        auto idMatchesEnd = sregex_iterator();
        id = idMatchesBegin->str();
        idMatchesBegin++;
        for (auto it = idMatchesBegin; it != idMatchesEnd; it++)
            nextModulesIDs.push_back(it->str());
    }
};
struct FlipFlop
{
    string id;
    vector<string> nextModulesIDs;
    bool isOn; // false for off, true for on
    FlipFlop(string inputRow = "")
    {
        if (inputRow.size() == 0)
            return;
        regex idRegex("[a-zA-Z]+");
        auto idMatchesBegin = sregex_iterator(inputRow.begin(), inputRow.end(), idRegex);
        auto idMatchesEnd = sregex_iterator();
        id = idMatchesBegin->str();
        idMatchesBegin++;
        for (auto it = idMatchesBegin; it != idMatchesEnd; it++)
            nextModulesIDs.push_back(it->str());
        isOn = false;
    }
};
struct LastPassedPulses
{
    map<string, bool> lastPulses; // false for low pulse, true for high pulse
    void set(string id, bool pulse)
    {
        if (lastPulses.find(id) != lastPulses.end())
            lastPulses[id] = pulse;
        else
            lastPulses.insert(pair<string, bool>(id, pulse));
    }
    bool allPulsesHigh()
    {
        for (auto p : lastPulses)
            if (p.second == false)
                return false;
        return true;
    }
};
struct Conjunction
{
    string id;
    vector<string> nextModulesIDs;
    LastPassedPulses lastPassedPulses;
    void addPulseSource(string id)
    {
        lastPassedPulses.set(id, false);
    }
    Conjunction(string inputRow = "")
    {
        if (inputRow.size() == 0)
            return;
        regex idRegex("[a-zA-Z]+");
        auto idMatchesBegin = sregex_iterator(inputRow.begin(), inputRow.end(), idRegex);
        auto idMatchesEnd = sregex_iterator();
        id = idMatchesBegin->str();
        idMatchesBegin++;
        for (auto it = idMatchesBegin; it != idMatchesEnd; it++)
            nextModulesIDs.push_back(it->str());
    }
};
struct Pulse
{
    string originID;
    string id;
    bool highPulse;
    Pulse(string oi, string i, bool hp)
    {
        originID = oi;
        id = i;
        highPulse = hp;
    }
};
struct AllModules
{
    map<string, Broadcaster> broadcasters;
    map<string, FlipFlop> flipFlops;
    map<string, Conjunction> conjunctions;
    set<string> nulls; // these are elements that are called but do not lead to anything
    int lowPulses, highPulses;
    int buttonPresses;
    vector<int> reachedFinalEdges;
    bool rxReached;
    queue<Pulse> pulses;
    char typeOfModule(string id)
    {
        if (broadcasters.find(id) != broadcasters.end())
            return 'B';
        if (flipFlops.find(id) != flipFlops.end())
            return 'F';
        if (conjunctions.find(id) != conjunctions.end())
            return 'C';
        if (nulls.find(id) != nulls.end())
            return 'N';
        cout << "ERROR: invalid module id(" << id << "), cannot find any module with corresponding id\n";
        return ' ';
    }
    bool contains(string id)
    {
        for (auto p : broadcasters)
            if (id == p.first)
                return true;
        for (auto p : flipFlops)
            if (id == p.first)
                return true;
        for (auto p : conjunctions)
            if (id == p.first)
                return true;
        for (auto p : nulls)
            if (id == p)
                return true;
        return false;
    }
    void readFromInput()
    {
        string inputRow;
        while (getline(fin, inputRow))
        {
            if (inputRow.size() == 0)
                return;
            Broadcaster broadcaster;
            FlipFlop flipFlop;
            Conjunction conjunction;
            switch (inputRow[0])
            {
            case 'b':
                broadcaster = Broadcaster(inputRow);
                broadcasters.insert(pair<string, Broadcaster>(broadcaster.id, broadcaster));
                break;
            case '%':
                flipFlop = FlipFlop(inputRow);
                flipFlops.insert(pair<string, FlipFlop>(flipFlop.id, flipFlop));
                break;
            case '&':
                conjunction = Conjunction(inputRow);
                conjunctions.insert(pair<string, Conjunction>(conjunction.id, conjunction));
                break;
            default:
                cout << "ERROR: invalid inputRow first character(" << inputRow[0] << "), cannot choose module type\n";
            }
        }
    }
    void addNullModules()
    {
        for (auto &p : broadcasters)
            for (const string &nextModuleID : p.second.nextModulesIDs)
            {
                if (!contains(nextModuleID))
                    nulls.insert(nextModuleID);
            }
        for (auto &p : flipFlops)
            for (const string &nextModuleID : p.second.nextModulesIDs)
            {
                if (!contains(nextModuleID))
                    nulls.insert(nextModuleID);
            }
        for (auto &p : conjunctions)
            for (const string &nextModuleID : p.second.nextModulesIDs)
            {
                if (!contains(nextModuleID))
                    nulls.insert(nextModuleID);
            }
    }
    void setConjunctionsPreviousModules()
    {
        for (auto &p : broadcasters)
            for (const string &nextModuleID : p.second.nextModulesIDs)
            {
                auto nextConjunctionIterator = conjunctions.find(nextModuleID);
                if (nextConjunctionIterator != conjunctions.end())
                    nextConjunctionIterator->second.addPulseSource(p.second.id);
            }
        for (auto &p : flipFlops)
            for (const string &nextModuleID : p.second.nextModulesIDs)
            {
                auto nextConjunctionIterator = conjunctions.find(nextModuleID);
                if (nextConjunctionIterator != conjunctions.end())
                    nextConjunctionIterator->second.addPulseSource(p.second.id);
            }
        for (auto &p : conjunctions)
            for (const string &nextModuleID : p.second.nextModulesIDs)
            {
                auto nextConjunctionIterator = conjunctions.find(nextModuleID);
                if (nextConjunctionIterator != conjunctions.end())
                    nextConjunctionIterator->second.addPulseSource(p.second.id);
            }
    }
    void proccessPulse()
    {
        Pulse pulse = pulses.front();
        pulses.pop();
        if (pulse.id == "kc" && pulse.highPulse)
        {
            // fout << "On press " << buttonPresses << " " << pulse.originID << endl;
            reachedFinalEdges.push_back(buttonPresses);
        }
        if ((pulse.id == "rx") && (!pulse.highPulse))
            rxReached = true;
        if (pulse.highPulse)
            highPulses++;
        else
            lowPulses++;
        char moduleType = typeOfModule(pulse.id);
        if (moduleType == 'N')
        {
        }
        else if (moduleType == 'B')
        {
            auto &broadcaster = broadcasters[pulse.id];
            for (string &nextModuleID : broadcaster.nextModulesIDs)
                pulses.push(Pulse(pulse.id, nextModuleID, pulse.highPulse));
        }
        else if (moduleType == 'F')
        {
            if (pulse.highPulse)
                return;
            auto &flipFlop = flipFlops[pulse.id];
            flipFlop.isOn = !flipFlop.isOn;
            for (string &nextModuleID : flipFlop.nextModulesIDs)
                pulses.push(Pulse(pulse.id, nextModuleID, flipFlop.isOn));
        }
        else if (moduleType == 'C')
        {
            auto &conjunction = conjunctions[pulse.id];
            conjunction.lastPassedPulses.set(pulse.originID, pulse.highPulse);
            bool nextPulse = !conjunction.lastPassedPulses.allPulsesHigh();
            for (string &nextModuleID : conjunction.nextModulesIDs)
                pulses.push(Pulse(pulse.id, nextModuleID, nextPulse));
        }
        else
            cout << "ERROR: invalid type of module\n";
    }
    void startSendingPulses()
    {
        buttonPresses++;
        pulses.push(Pulse("button", "broadcaster", false)); // send low pulse from button to broadcaster
        while (pulses.size())
            proccessPulse();
    }
    void displayGraph()
    {
        for (auto p : broadcasters)
            for (auto nextID : p.second.nextModulesIDs)
            {
                cout << p.first << " -> ";
                switch (typeOfModule(nextID))
                {
                case 'F':
                    cout << "F";
                    break;
                case 'C':
                    cout << "C";
                    break;
                }
                cout << nextID << endl;
            }
        for (auto p : flipFlops)
            for (auto nextID : p.second.nextModulesIDs)
            {
                cout << "F" << p.first << " -> ";
                switch (typeOfModule(nextID))
                {
                case 'F':
                    cout << "F";
                    break;
                case 'C':
                    cout << "C";
                    break;
                }
                cout << nextID << endl;
            }
        for (auto p : conjunctions)
            for (auto nextID : p.second.nextModulesIDs)
            {
                cout << "C" << p.first << " -> ";
                switch (typeOfModule(nextID))
                {
                case 'F':
                    cout << "F";
                    break;
                case 'C':
                    cout << "C";
                    break;
                }
                cout << nextID << endl;
            }
    }
    AllModules()
    {
        readFromInput();
        addNullModules();
        setConjunctionsPreviousModules();
        lowPulses = 0;
        highPulses = 0;
        buttonPresses = 0;
        rxReached = false;
    }
};
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
int main()
{
    AllModules allModules;
    // allModules.displayGraph();
    while (allModules.reachedFinalEdges.size() < 4)
        allModules.startSendingPulses();
    long long lcm = 1;
    for (long long finalEdge : allModules.reachedFinalEdges)
    {
        // fout << finalEdge << " ";
        lcm = leastCommonMultiple(lcm, finalEdge);
    }
    fout << lcm;
}