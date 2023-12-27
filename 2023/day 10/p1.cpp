#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;
ifstream fin("p1.in");
ofstream fout("p1.out");

struct
{
    bool down(const char c)
    {
        switch (c)
        {
        case '|':
        case 'F':
        case '7':
            return true;
        default:
            return false;
        }
    }
    bool up(const char c)
    {
        switch (c)
        {
        case '|':
        case 'L':
        case 'J':
            return true;
        default:
            return false;
        }
    }
    bool left(const char c)
    {
        switch (c)
        {
        case '-':
        case 'J':
        case '7':
            return true;
        default:
            return false;
        }
    }
    bool right(const char c)
    {
        switch (c)
        {
        case '-':
        case 'L':
        case 'F':
            return true;
        default:
            return false;
        }
    }

} connects;

void getInputMap(char inputMap[200][200], int &rows, int &cols)
{
    string inputRow;
    rows = 0;
    cols = 0;
    while (getline(fin, inputRow))
    {
        cols = 0;
        for (const char &c : inputRow)
        {
            inputMap[rows][cols] = c;
            cols++;
        }
        rows++;
    }
}
void getStartingCoords(char inputMap[200][200], int rows, int cols, int &startingRow, int &startingCol)
{
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            if (inputMap[i][j] == 'S')
            {
                startingRow = i;
                startingCol = j;
                return;
            }
}
struct Node
{
    char value;
    int row, col;
    string connectedFrom;
    Node(int r, int c, char v, string connectFrom)
    {
        row = r;
        col = c;
        value = v;
        connectedFrom = connectFrom;
    }
};
Node getNextNodeFromStart(Node &n, char inputMap[200][200], int rows, int cols)
{
    if (connects.down(inputMap[n.row - 1][n.col]))
        return Node(n.row - 1, n.col, inputMap[n.row - 1][n.col], "down");
    else if (connects.up(inputMap[n.row + 1][n.col]))
        return Node(n.row + 1, n.col, inputMap[n.row + 1][n.col], "up");
    else if (connects.left(inputMap[n.row][n.col + 1]))
        return Node(n.row, n.col + 1, inputMap[n.row][n.col + 1], "left");
    else if (connects.right(inputMap[n.row][n.col - 1]))
        return Node(n.row, n.col - 1, inputMap[n.row][n.col - 1], "right");
    else
        cout << "ERROR: starting node does not have any connection";
}
Node getNextNode(Node &n, char inputMap[200][200], int rows, int cols)
{
    switch (n.value)
    {
    case '|':
        if (n.connectedFrom == "up")
            return Node(n.row + 1, n.col, inputMap[n.row + 1][n.col], "up"); // return node from down
        else
            return Node(n.row - 1, n.col, inputMap[n.row - 1][n.col], "down"); // return node from up
    case '-':
        if (n.connectedFrom == "left")
            return Node(n.row, n.col + 1, inputMap[n.row][n.col + 1], "left"); // return node from right
        else
            return Node(n.row, n.col - 1, inputMap[n.row][n.col - 1], "right"); // return node from left
    case 'L':
        if (n.connectedFrom == "up")
            return Node(n.row, n.col + 1, inputMap[n.row][n.col + 1], "left"); // return node from right
        else
            return Node(n.row - 1, n.col, inputMap[n.row - 1][n.col], "down"); // return node from up
    case 'J':
        if (n.connectedFrom == "up")
            return Node(n.row, n.col - 1, inputMap[n.row][n.col - 1], "right"); // return node from left
        else
            return Node(n.row - 1, n.col, inputMap[n.row - 1][n.col], "down"); // return node from up
    case '7':
        if (n.connectedFrom == "left")
            return Node(n.row + 1, n.col, inputMap[n.row + 1][n.col], "up"); // return node from down
        else
            return Node(n.row, n.col - 1, inputMap[n.row][n.col - 1], "right"); // return node from left
    case 'F':
        if (n.connectedFrom == "right")
            return Node(n.row + 1, n.col, inputMap[n.row + 1][n.col], "up"); // return node from down
        else
            return Node(n.row, n.col + 1, inputMap[n.row][n.col + 1], "left"); // return node from right
    case 'S':
        return getNextNodeFromStart(n, inputMap, rows, cols);
    }
}
vector<Node> getNodeVector(char inputMap[200][200], int rows, int cols)
{
    int startingRow, startingCol;
    getStartingCoords(inputMap, rows, cols, startingRow, startingCol);
    vector<Node> nodeVector;
    Node startingNode(startingRow, startingCol, inputMap[startingRow][startingCol], "");
    nodeVector.push_back(startingNode);
    while (true)
    {
        Node &n = nodeVector.back();
        Node next = getNextNode(n, inputMap, rows, cols);
        if (next.value == 'S')
            return nodeVector;
        nodeVector.push_back(next);
    }
}
int main()
{
    char inputMap[200][200];
    int rows, cols;
    getInputMap(inputMap, rows, cols);
    vector<Node> nv = getNodeVector(inputMap, rows, cols);
    fout << nv.size() / 2;
    return 0;
}