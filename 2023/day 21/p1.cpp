#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#define STEPS 100
#define ARRAY_MAX_SIZE 1180

using namespace std;
ifstream fin("p1.in");
ofstream fout("p1.out");
struct Fill
{
    int row;
    int col;
    int stepsWalked;
    Fill(int r, int c, int sw)
    {
        row = r;
        col = c;
        stepsWalked = sw;
    }
};
void fillGarden(char garden[ARRAY_MAX_SIZE][ARRAY_MAX_SIZE], const int &rows, const int &cols, int startRow, int startCol)
{
    // Garden has 4 codes:
    // . for passable and not reached
    // # for non-passable
    // ! for reached but that cannot be reached in the given steps (STEPS doesn't the same parity as stepsWalked)
    // * for reached but that can be reached in the given steps (STEPS has the same parity as stepsWalked)
    queue<Fill> fills;
    fills.push(Fill(startRow, startCol, 0));
    while (!fills.empty())
    {
        Fill fill = fills.front();
        fills.pop();
        if (fill.row < 0 || fill.col < 0 || fill.row >= rows || fill.col >= cols)
        {
            cout << "NOT good\n";
            continue;
        }
        if (fill.stepsWalked > STEPS)
            continue;
        if (garden[fill.row][fill.col] == '#' || garden[fill.row][fill.col] == '*' || garden[fill.row][fill.col] == '!')
            continue;
        if (fill.stepsWalked % 2 == STEPS % 2)
            garden[fill.row][fill.col] = '*'; // if can be reached in the given STEPS
        else
            garden[fill.row][fill.col] = '!'; // if cannot be reached in the given STEPS

        fills.push(Fill(fill.row - 1, fill.col, fill.stepsWalked + 1));
        fills.push(Fill(fill.row + 1, fill.col, fill.stepsWalked + 1));
        fills.push(Fill(fill.row, fill.col - 1, fill.stepsWalked + 1));
        fills.push(Fill(fill.row, fill.col + 1, fill.stepsWalked + 1));
    }
}
int gardenSpotsReachable(const char garden[ARRAY_MAX_SIZE][ARRAY_MAX_SIZE], const int &rows, const int &cols)
{
    int count = 0;
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            fout << garden[i][j];
            if (garden[i][j] == '*')
                count++;
        }
        fout << endl;
    }
    fout << endl;
    return count;
}
void copyGarden(char garden[ARRAY_MAX_SIZE][ARRAY_MAX_SIZE], char newGarden[ARRAY_MAX_SIZE][ARRAY_MAX_SIZE], int rows, int cols)
{
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            newGarden[i][j] = garden[i][j];
}
void readGarden(char garden[ARRAY_MAX_SIZE][ARRAY_MAX_SIZE], int &rows, int &cols, int &startRow, int &startCol)
{
    string inputRow;
    rows = 0;
    cols = 0;
    while (getline(fin, inputRow))
    {
        if (inputRow.size() == 0)
            return;
        cols = 0;
        for (const char &c : inputRow)
        {
            garden[rows][cols] = c;
            if (garden[rows][cols] == 'S')
            {
                startRow = rows;
                startCol = cols;
                garden[rows][cols] = '.';
            }
            cols++;
        }
        rows++;
    }
    int originalRows = rows, originalCols = cols;
    int distanceFromCenter = (cols - 1) / 2;
    if (rows / 2 < STEPS)
    {
        char extendedGarden[ARRAY_MAX_SIZE][ARRAY_MAX_SIZE];
        copyGarden(garden, extendedGarden, rows, cols);
        while (rows + distanceFromCenter - originalRows < STEPS)
        {
            for (int k = 0; k < 2; k++)
            {
                for (int j = 0; j < originalCols; j++)
                {
                    for (int i = 0; i < rows; i++)
                        extendedGarden[i][cols] = garden[i % originalRows][j];
                    cols++;
                }
                for (int i = 0; i < originalRows; i++)
                {
                    for (int j = 0; j < cols; j++)
                        extendedGarden[rows][j] = garden[i][j % originalCols];
                    rows++;
                }
            }
            startRow += originalRows;
            startCol += originalCols;
        }
        // for (int i = 0; i < rows; i++)
        // {
        // for (int j = 0; j < cols; j++)
        // fout << extendedGarden[i][j];
        // fout << endl;
        // }
        copyGarden(extendedGarden, garden, rows, cols);
    }
}
int main()
{
    char garden[ARRAY_MAX_SIZE][ARRAY_MAX_SIZE];
    int rows, cols, startRow, startCol;
    readGarden(garden, rows, cols, startRow, startCol);
    fillGarden(garden, rows, cols, startRow, startCol);
    fout << gardenSpotsReachable(garden, rows, cols);
}