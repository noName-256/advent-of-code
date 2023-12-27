#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <regex>

using namespace std;
ifstream fin("p2.in");
ofstream fout("p2.out");
struct Part
{
    int x, m, a, s;
    int ratingSum() const
    {
        return x + m + a + s;
    }
    Part(int _x, int _m, int _a, int _s)
    {
        x = _x;
        m = _m;
        a = _a;
        s = _s;
    }
    Part(string partInputRow)
    {
        x = -1;
        m = -1;
        a = -1;
        s = -1;
        regex numberRegex("[0-9]+");
        auto numbersMatchesBegin = sregex_iterator(partInputRow.begin(), partInputRow.end(), numberRegex);
        auto numbersMatchesEnd = sregex_iterator();
        for (auto it = numbersMatchesBegin; it != numbersMatchesEnd; it++)
        {
            int number = stoi(it->str());
            if (x == -1)
                x = number;
            else if (m == -1)
                m = number;
            else if (a == -1)
                a = number;
            else if (s == -1)
                s = number;
            else
                cout << "ERROR: too many number matches\n";
        }
    }
};
struct Condition
{
    char category;  // x / m / a / s
    char operation; // < / >
    int value;
    bool isSatisfied(const Part &part) const
    {
        switch (category)
        {
        case 'x':
            switch (operation)
            {
            case '<':
                return part.x < value;
            case '>':
                return part.x > value;
            default:
                cout << "ERROR: invalid operator: " << operation << endl;
                return false;
            }
        case 'm':
            switch (operation)
            {
            case '<':
                return part.m < value;
            case '>':
                return part.m > value;
            default:
                cout << "ERROR: invalid operator: " << operation << endl;
                return false;
            }
        case 'a':
            switch (operation)
            {
            case '<':
                return part.a < value;
            case '>':
                return part.a > value;
            default:
                cout << "ERROR: invalid operator: " << operation << endl;
                return false;
            }
        case 's':
            switch (operation)
            {
            case '<':
                return part.s < value;
            case '>':
                return part.s > value;
            default:
                cout << "ERROR: invalid operator: " << operation << endl;
                return false;
            }
        default:
            cout << "ERROR: invalid category: " << category << endl;
            return false;
        }
    }
    Condition(char c = ' ', char o = ' ', int v = 0)
    {
        category = c;
        operation = o;
        value = v;
    }
};
struct Rule
{
    Condition condition;
    string nextWorkflowID;
    void display() const
    {
        cout << condition.category << condition.operation << condition.value << ":" << nextWorkflowID;
    }
    Rule(string ruleString)
    {
        char category, operation;
        int value;
        category = ruleString[0];
        operation = ruleString[1];

        regex valueRegex("[0-9]+");
        smatch valueMatch;
        regex_search(ruleString, valueMatch, valueRegex);
        value = stoi(valueMatch.str());

        regex nextIDRegex("[a-zA-Z]+$");
        smatch nextIDMatch;
        regex_search(ruleString, nextIDMatch, nextIDRegex);
        nextWorkflowID = nextIDMatch.str();

        condition = Condition(category, operation, value);
    }
};
struct Workflow
{
    string id;
    vector<Rule> rules;
    string defaultNextID;
    void display() const
    {
        cout << id << " { ";
        for (const Rule &rule : rules)
        {
            rule.display();
            cout << ",";
        }
        cout << defaultNextID << " }\n";
    }
    string next(const Part &part) const
    {
        string nextID;
        for (const Rule &rule : rules)
            if (rule.condition.isSatisfied(part))
                return rule.nextWorkflowID;
        return defaultNextID;
    }
    Workflow(string workflowInputRow)
    {
        regex idRegex("[a-zA-Z]+(?=\\{)");
        smatch idMatch;
        regex_search(workflowInputRow, idMatch, idRegex);
        id = idMatch.str();

        regex ruleRegex("(x|m|a|s)(<|>)[0-9]+:[a-zA-Z]+(?=,)");
        auto rulesMatchesBegin = sregex_iterator(workflowInputRow.begin(), workflowInputRow.end(), ruleRegex);
        auto rulesMatchesEnd = sregex_iterator();
        for (auto it = rulesMatchesBegin; it != rulesMatchesEnd; it++)
            rules.push_back(Rule(it->str()));

        regex defaultNextIDRegex("[a-zA-Z]+(?=}$)");
        smatch defaultNextIDMatch;
        regex_search(workflowInputRow, defaultNextIDMatch, defaultNextIDRegex);
        defaultNextID = defaultNextIDMatch.str();
    }
};
struct Range
{
    int xFrom, xTo;
    int mFrom, mTo;
    int aFrom, aTo;
    int sFrom, sTo;
    long long combinations() const
    {
        return ((long long)xTo - xFrom + 1) *
               ((long long)mTo - mFrom + 1) *
               ((long long)aTo - aFrom + 1) *
               ((long long)sTo - sFrom + 1);
    }
    Range rangeAfterCondition(const Condition &condition) const
    {
        Range newRange = *this;
        switch (condition.category)
        {
        case 'x':
            switch (condition.operation)
            {
            case '<':
                newRange.xTo = min(xTo, condition.value - 1);
                return newRange;
            case '>':
                newRange.xFrom = max(xFrom, condition.value + 1);
                return newRange;
            default:
                cout << "ERROR: invalid operator: " << condition.operation << endl;
                return newRange;
            }
        case 'm':
            switch (condition.operation)
            {
            case '<':
                newRange.mTo = min(mTo, condition.value - 1);
                return newRange;
            case '>':
                newRange.mFrom = max(mFrom, condition.value + 1);
                return newRange;
            default:
                cout << "ERROR: invalid operator: " << condition.operation << endl;
                return newRange;
            }
        case 'a':
            switch (condition.operation)
            {
            case '<':
                newRange.aTo = min(aTo, condition.value - 1);
                return newRange;
            case '>':
                newRange.aFrom = max(aFrom, condition.value + 1);
                return newRange;
            default:
                cout << "ERROR: invalid operator: " << condition.operation << endl;
                return newRange;
            }
        case 's':
            switch (condition.operation)
            {
            case '<':
                newRange.sTo = min(sTo, condition.value - 1);
                return newRange;
            case '>':
                newRange.sFrom = max(sFrom, condition.value + 1);
                return newRange;
            default:
                cout << "ERROR: invalid operator: " << condition.operation << endl;
                return newRange;
            }
        default:
            cout << "ERROR: invalid category: " << condition.category << endl;
            return false;
        }
    }
    Range(int xf = 1, int xt = 4000,
          int mf = 1, int mt = 4000,
          int af = 1, int at = 4000,
          int sf = 1, int st = 4000)
    {
        xFrom = xf;
        xTo = xt;
        mFrom = mf;
        mTo = mt;
        aFrom = af;
        aTo = at;
        sFrom = sf;
        sTo = st;
    }
};
long long acceptedCombinations(const map<string, Workflow> &workflows, string workflowID, Range range)
{
    if (workflowID == "A")
        return range.combinations();
    if (workflowID == "R")
        return 0;
    const Workflow &workflow = workflows.find(workflowID)->second;
    long long combinations = 0;
    for (const Rule &rule : workflow.rules)
    {
        combinations += acceptedCombinations(workflows, rule.nextWorkflowID, range.rangeAfterCondition(rule.condition));
        Condition invertedCondition = rule.condition;
        switch (invertedCondition.operation)
        {
        case '<':
            invertedCondition.operation = '>';
            invertedCondition.value--;
            break;
        case '>':
            invertedCondition.operation = '<';
            invertedCondition.value++;
        }
        range = range.rangeAfterCondition(invertedCondition);
    }
    combinations += acceptedCombinations(workflows, workflow.defaultNextID, range);
    return combinations;
}
bool partIsAccepted(const map<string, Workflow> &workflows, const Part &part)
{
    string workflowID = "in";
    while (workflowID != "A" && workflowID != "R")
        workflowID = workflows.find(workflowID)->second.next(part);
    return workflowID == "A";
}
void readWorkflows(map<string, Workflow> &workflows)
{
    string inputRow;
    while (getline(fin, inputRow))
    {
        if (inputRow.size() == 0)
            return;
        Workflow workflow(inputRow);
        workflows.insert(pair<string, Workflow>(workflow.id, workflow));
    }
}
void readParts(vector<Part> &parts)
{
    string inputRow;
    while (getline(fin, inputRow))
    {
        if (inputRow.size() == 0)
            return;
        Part part(inputRow);
        parts.push_back(part);
    }
}
int main()
{
    map<string, Workflow> workflows;
    vector<Part> parts;
    readWorkflows(workflows);
    readParts(parts);
    // for (auto p : workflows)
    // {
    //     Workflow workflow = p.second;
    //     workflow.display();
    // }
    long long totalAcceptedCombinations = acceptedCombinations(workflows, "in", Range());
    fout << totalAcceptedCombinations;
}