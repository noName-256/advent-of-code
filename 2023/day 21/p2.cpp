#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include <set>
#define STEPS 26501365
#define ARRAY_MAX_SIZE 1000

using namespace std;
ifstream fin("p2.in");
ofstream fout("p2.out");
struct Fill
{
    int row;
    int col;
    int stepsWalked;
    int gardenRow(int rows)
    {
        int gRow = row % rows;
        if (gRow < 0)
            gRow += rows;
        return gRow;
    }
    int gardenCol(int cols)
    {
        int gCol = col % cols;
        if (gCol < 0)
            gCol += cols;
        return gCol;
    }
    Fill(int r, int c, int sw)
    {
        row = r;
        col = c;
        stepsWalked = sw;
    }
};
struct FillCmp
{
    bool operator()(const Fill &f1, const Fill &f2) const
    {
        if (f1.row != f2.row)
            return f1.row < f2.row;
        if (f1.col != f2.col)
            return f1.col < f2.col;
        return false;
    }
};
int fillGarden(char garden[ARRAY_MAX_SIZE][ARRAY_MAX_SIZE], const int &rows, const int &cols, int startRow, int startCol, int maxSteps)
{
    // Garden has 4 codes:
    // . for passable and not reached
    // # for non-passable
    // ! for reached but that cannot be reached in the given steps (STEPS doesn't the same parity as stepsWalked)
    // * for reached but that can be reached in the given steps (STEPS has the same parity as stepsWalked)
    queue<Fill> fills;
    set<Fill, FillCmp> passedFills;
    fills.push(Fill(startRow, startCol, 0));
    int odd = 0, even = 0;
    while (!fills.empty())
    {
        Fill fill = fills.front();
        fills.pop();
        if (passedFills.find(fill) != passedFills.end())
            continue;
        if (fill.stepsWalked > maxSteps)
            continue;
        if (garden[fill.gardenRow(rows)][fill.gardenCol(cols)] == '#')
            continue;
        if (fill.stepsWalked % 2 == 0)
            even++;
        else
            odd++;

        passedFills.insert(fill);
        fills.push(Fill(fill.row - 1, fill.col, fill.stepsWalked + 1));
        fills.push(Fill(fill.row + 1, fill.col, fill.stepsWalked + 1));
        fills.push(Fill(fill.row, fill.col - 1, fill.stepsWalked + 1));
        fills.push(Fill(fill.row, fill.col + 1, fill.stepsWalked + 1));
    }
    if (maxSteps % 2 == 0)
        return even;
    return odd;
}
int gardenSpotsReachable(const char garden[ARRAY_MAX_SIZE][ARRAY_MAX_SIZE], const int &rows, const int &cols, char check = '*')
{
    int count = 0;
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            if (garden[i][j] == check)
                count++;
        }
    }
    return count;
}
void copyGarden(const char garden[ARRAY_MAX_SIZE][ARRAY_MAX_SIZE], char newGarden[ARRAY_MAX_SIZE][ARRAY_MAX_SIZE], const int &rows, const int &cols)
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
            }
            cols++;
        }
        rows++;
    }
}
int main()
{
    char garden[ARRAY_MAX_SIZE][ARRAY_MAX_SIZE];
    int rows, cols, startRow, startCol;
    readGarden(garden, rows, cols, startRow, startCol);

    const long long length = rows,
                    distanceFromCenter = (length - 1) / 2;
    long long fullOdd = 0, fullEven = 0, cornerOdd = 0, cornerEven = 0;
    long long extraFullOdd = 0, extraFullEven = 0, extraCornerOdd = 0, extraCornerEven = 0;

    long long p1 = fillGarden(garden, rows, cols, startRow, startCol, distanceFromCenter);
    long long p2 = fillGarden(garden, rows, cols, startRow, startCol, distanceFromCenter + length);
    long long p3 = fillGarden(garden, rows, cols, startRow, startCol, distanceFromCenter + 2 * length);
    long long a, b, c;
    long long n = (STEPS - distanceFromCenter) / length;

    fout << p1 / 2 * (n * n - 3 * n + 2) - p2 * (n * n - 2 * n) + p3 / 2 * (n * n - n);
}