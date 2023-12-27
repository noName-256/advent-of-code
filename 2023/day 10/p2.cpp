#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;
ifstream fin("p2.in");
ofstream fout("p2.out");

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
        {
            // reassign start so that we can get the connectedFrom for it
            nodeVector.front() = next;
            return nodeVector;
        }
        nodeVector.push_back(next);
    }
}
void addBorder(char fillMap[400][400], int &fillMapRows, int &fillMapCols)
{
    char fillMapCopy[400][400];
    for (int i = 0; i < fillMapRows; i++)
        for (int j = 0; j < fillMapCols; j++)
            fillMapCopy[i][j] = fillMap[i][j];
    for (int i = 0; i < fillMapRows; i++)
    {
        fillMap[i][0] = ',';
        fillMap[i][fillMapCols + 1] = ',';
    }
    for (int j = 0; j < fillMapCols; j++)
    {
        fillMap[0][j] = ',';
        fillMap[fillMapRows + 1][j] = ',';
    }
    for (int i = 0; i < fillMapRows; i++)
        for (int j = 0; j < fillMapCols; j++)
            fillMap[i + 1][j + 1] = fillMapCopy[i][j];
    fillMapRows += 2;
    fillMapCols += 2;
}
void getInitialFillMap(char fillMap[400][400], int &fillMapRows, int &fillMapCols,
                       int originalRows, int originalCols,
                       vector<Node> nodeVector)
{
    /*
    This adds a row and column between each two rows and columns to simulate gaps
    The gaps that connect pipes become pipes as well
     */
    fillMapRows = originalRows * 2 - 1;
    fillMapCols = originalCols * 2 - 1;
    for (int i = 0; i < fillMapRows; i++)
        for (int j = 0; j < fillMapCols; j++)
            fillMap[i][j] = '.'; // these are the normal cells in the map
    for (int i = 1; i < fillMapRows; i += 2)
        for (int j = 0; j < fillMapCols; j++)
            fillMap[i][j] = ','; // these are the cells that we filled the map with, aka the gap cells
    for (int i = 0; i < fillMapRows; i++)
        for (int j = 1; j < fillMapCols; j += 2)
            fillMap[i][j] = ','; // these are the cells that we filled the map with, aka the gap cells
    for (Node &n : nodeVector)
    {
        fillMap[n.row * 2][n.col * 2] = '='; // these are the cells that are pipes
        // the following are the cells that are gaps filled by pipes
        if (n.connectedFrom == "down")
            fillMap[n.row * 2 + 1][n.col * 2] = '-';
        else if (n.connectedFrom == "up")
            fillMap[n.row * 2 - 1][n.col * 2] = '-';
        else if (n.connectedFrom == "left")
            fillMap[n.row * 2][n.col * 2 - 1] = '-';
        else if (n.connectedFrom == "right")
            fillMap[n.row * 2][n.col * 2 + 1] = '-';
        else
            cout << "ERROR: node does not have valid connection( " << n.connectedFrom << " )" << endl;
    }
}
void getFillMap(char fillMap[400][400], int &fillMapRows, int &fillMapCols,
                int originalRows, int originalCols,
                vector<Node> nodeVector)
{
    /*
    Map gets filled as follows:
    - '.' for original dots
    - ',' for gaps that were inserted
    - '=' for pipes
    - '-' for pipe connections in gaps

    After flood fill:
    - ' ' for the original dots that can be accessed from outside
    - ' ' as well for the gaps that were inserted
     */
    getInitialFillMap(fillMap, fillMapRows, fillMapCols, originalRows, originalCols, nodeVector);
    addBorder(fillMap, fillMapRows, fillMapCols);
}
void showFillMap(const char fillMap[400][400], int fillMapRows, int fillMapCols)
{
    for (int i = 0; i < fillMapRows; i++)
    {
        for (int j = 0; j < fillMapCols; j++)
            fout << fillMap[i][j];
        fout << endl;
    }
    fout << endl;
}
void floodFill(int row, int col,
               char fillMap[400][400],
               int fillMapRows, int fillMapCols)
{
    if (fillMap[row][col] != '.' && fillMap[row][col] != ',')
        return;

    fillMap[row][col] = ' ';
    floodFill(row - 1, col, fillMap, fillMapRows, fillMapCols);
    floodFill(row + 1, col, fillMap, fillMapRows, fillMapCols);
    floodFill(row, col - 1, fillMap, fillMapRows, fillMapCols);
    floodFill(row, col + 1, fillMap, fillMapRows, fillMapCols);
}
int countInnerTiles(const char fillMap[400][400], int fillMapRows, int fillMapCols)
{
    int count = 0;
    for (int i = 0; i < fillMapRows; i++)
        for (int j = 0; j < fillMapCols; j++)
            if (fillMap[i][j] == '.')
                count++;
    return count;
}
int main()
{
    char inputMap[200][200];
    int rows, cols;
    getInputMap(inputMap, rows, cols);
    vector<Node> nv = getNodeVector(inputMap, rows, cols);
    char fillMap[400][400];
    int fillMapRows, fillMapCols;
    getFillMap(fillMap, fillMapRows, fillMapCols, rows, cols, nv);
    floodFill(0, 0, fillMap, fillMapRows, fillMapCols);
    fout << countInnerTiles(fillMap, fillMapRows, fillMapCols);
    return 0;
}