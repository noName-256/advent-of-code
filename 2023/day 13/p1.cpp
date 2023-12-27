#include <iostream>
#include <fstream>
#include <string>

using namespace std;
ifstream fin("p1.in");
ofstream fout("p1.out");
bool identicalRows(char pattern[200][200], int row1, int row2, int cols)
{
    for (int j = 0; j < cols; j++)
        if (pattern[row1][j] != pattern[row2][j])
            return false;
    return true;
}
bool rowIsMirror(char pattern[200][200], int rows, int cols, int checkedRow)
{
    int i1 = checkedRow, i2 = checkedRow - 1;
    while (i1 < rows && i2 >= 0)
    {
        if (!identicalRows(pattern, i1, i2, cols))
            return false;
        i1++;
        i2--;
    }
    return true;
}
int findRowMirror(char pattern[200][200], int rows, int cols)
{
    for (int i = 1; i < rows; i++)
        if (rowIsMirror(pattern, rows, cols, i))
            return i;
    return -1;
}
bool identicalCols(char pattern[200][200], int col1, int col2, int rows)
{
    for (int i = 0; i < rows; i++)
        if (pattern[i][col1] != pattern[i][col2])
            return false;
    return true;
}
bool colIsMirror(char pattern[200][200], int rows, int cols, int checkedCol)
{
    int j1 = checkedCol, j2 = checkedCol - 1;
    while (j1 < cols && j2 >= 0)
    {
        if (!identicalCols(pattern, j1, j2, rows))
            return false;
        j1++;
        j2--;
    }
    return true;
}
int findColMirror(char pattern[200][200], int rows, int cols)
{
    for (int j = 1; j < cols; j++)
        if (colIsMirror(pattern, rows, cols, j))
            return j;
    return -1;
}
void outputPattern(char pattern[200][200], int rows, int cols, ostream &out = cout)
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
            out << pattern[i][j];
        out << endl;
    }
    out << endl;
}
int patternScore(char pattern[200][200], int rows, int cols)
{
    int rowMirror = findRowMirror(pattern, rows, cols);
    int colMirror = findColMirror(pattern, rows, cols);
    int score = 0;
    if (rowMirror != -1)
        score += 100 * rowMirror;
    if (colMirror != -1)
        score += colMirror;
    // if (rowMirror == -1 && colMirror == -1)
    // outputPattern(pattern, rows, cols);
    // if (rowMirror != -1 && colMirror != -1)
    //     fout << "." << endl;
    return score;
}
bool readPattern(char pattern[200][200], int &rows, int &cols)
{
    string intputRow;
    do
    {
        if (!getline(fin, intputRow))
            return false;
    } while (intputRow.length() == 0);
    rows = 0;
    cols = 0;
    do
    {
        cols = 0;
        for (char c : intputRow)
        {
            pattern[rows][cols] = c;
            cols++;
        }
        rows++;
        if (!getline(fin, intputRow))
            break;
    } while (intputRow.length() != 0);
    return true;
};
int main()
{
    char pattern[200][200];
    int rows, cols;
    int patternScoreSum = 0;
    while (readPattern(pattern, rows, cols))
        patternScoreSum += patternScore(pattern, rows, cols);
    fout << patternScoreSum;
}