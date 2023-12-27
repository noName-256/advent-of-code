#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#define START_ROW 250
#define START_COL 250
#define ROWS 500
#define COLS 500

using namespace std;
ifstream fin("p1.in");
ofstream fout("p1.out");

struct Instruction
{
    int steps;
    char direction;
    string color;
    Instruction(int s, char d, string col)
    {
        steps = s;
        direction = d;
        color = col;
    }
};
struct LavaPool
{
    char pool[ROWS][COLS];
    int currentRow, currentCol;
    void fill(int row, int col, char c)
    {
        if (row < 0 || col < 0 || row >= ROWS || col >= COLS)
            return;
        if (pool[row][col] == '#' || pool[row][col] == '.') // # for dug pool, . for outside after filling
            return;
        pool[row][col] = c;
        fill(row - 1, col, c);
        fill(row + 1, col, c);
        fill(row, col - 1, c);
        fill(row, col + 1, c);
    }
    int getPoolVolume()
    {
        int volume = 0;
        for (int i = 0; i < ROWS; i++)
            for (int j = 0; j < COLS; j++)
                if (pool[i][j] == '#')
                    volume++;
        return volume;
    }
    LavaPool(const vector<Instruction> &instructions)
    {
        currentRow = START_ROW;
        currentCol = START_COL;
        for (int i = 0; i < ROWS; i++)
            for (int j = 0; j < COLS; j++)
                pool[i][j] = ' ';
        for (Instruction instruction : instructions)
        {
            for (int step = 1; step <= instruction.steps; step++)
            {
                switch (instruction.direction)
                {
                case 'U':
                    currentRow--;
                    break;
                case 'D':
                    currentRow++;
                    break;
                case 'L':
                    currentCol--;
                    break;
                case 'R':
                    currentCol++;
                    break;
                default:
                    cout << "ERROR: invalid instruction direction: " << instruction.direction << endl;
                }
                pool[currentRow][currentCol] = '#';
            }
        }
        fill(0, 0, '.');
        for (int i = 0; i < ROWS; i++)
            for (int j = 0; j < COLS; j++)
                if (pool[i][j] == ' ')
                {
                    fill(i, j, '#');
                    goto end;
                }
    end:
        // for (int i = 0; i < ROWS; i++)
        // {
        //     for (int j = 0; j < COLS; j++)
        //         fout << pool[i][j];
        //     fout << endl;
        // }
        cout << "END of constructor";
    }
};
vector<Instruction> readInstructions()
{
    int steps;
    char direction;
    string color;
    vector<Instruction> instructions;
    while (fin >> direction >> steps >> color)
    {
        color = color.substr(2, 6);
        instructions.push_back(Instruction(steps, direction, color));
    }
    return instructions;
}
void getBoundaries(const vector<Instruction> &instructions, int &minRow, int &minCol, int &maxRow, int &maxCol)
{
    minRow = 0;
    minCol = 0;
    maxRow = 0;
    maxCol = 0;
    int currentRow = 0, currentCol = 0;
    for (const Instruction &instruction : instructions)
    {
        switch (instruction.direction)
        {
        case 'U':
            currentRow -= instruction.steps;
            break;
        case 'D':
            currentRow += instruction.steps;
            break;
        case 'L':
            currentCol -= instruction.steps;
            break;
        case 'R':
            currentCol += instruction.steps;
            break;
        default:
            cout << "ERROR: invalid instruction direction: " << instruction.direction << endl;
        }
        minRow = min(minRow, currentRow);
        maxRow = max(maxRow, currentRow);
        minCol = min(minCol, currentCol);
        maxCol = max(maxCol, currentCol);
    }
}
int main()
{
    vector<Instruction> instructions = readInstructions();
    int minRow = 0, maxRow = 0, minCol = 0, maxCol = 0;
    getBoundaries(instructions, minRow, minCol, maxRow, maxCol);
    cout << minRow << " " << minCol << endl;
    cout << maxRow << " " << maxCol << endl;
    LavaPool lavaPool(instructions);
    fout << lavaPool.getPoolVolume();
}