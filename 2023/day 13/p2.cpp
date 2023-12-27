#include <iostream>
#include <fstream>
#include <string>
#include <map>

using namespace std;
ifstream fin("p2.in");
ofstream fout("p2.out");
struct SavedScores
{
    map<int, int> s;
    bool contains(int n) const
    {
        return s.find(n) != s.end();
    }
    void insert(int n)
    {
        if (!contains(n))
            s.insert(pair<int, int>(n, 1));
        else
            s[n]++;
    }
    int uniqueScore()
    {
        // cout << "\n\n\n";
        // for (auto scoreDataPoint : s)
        //     cout << scoreDataPoint.first << " " << scoreDataPoint.second << endl;
        for (auto scoreDataPoint : s)
            if (scoreDataPoint.second == 2)
                return scoreDataPoint.first;
        return -1;
    }
    SavedScores()
    {
    }
};
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
int findRowMirror(char pattern[200][200], int rows, int cols, int bannedRow)
{
    for (int i = 1; i < rows; i++)
        if (rowIsMirror(pattern, rows, cols, i) && i != bannedRow)
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
int findColMirror(char pattern[200][200], int rows, int cols, int bannedCol)
{
    for (int j = 1; j < cols; j++)
        if (colIsMirror(pattern, rows, cols, j) && j != bannedCol)
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
int patternScore(char pattern[200][200], int rows, int cols, int bannedRow = -1, int bannedCol = -1)
{
    int rowMirror = findRowMirror(pattern, rows, cols, bannedRow);
    int colMirror = findColMirror(pattern, rows, cols, bannedCol);
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
bool validPatternScore(int patternScore)
{
    if (patternScore == 0)
        return false;
    if (patternScore % 100 != 0 && patternScore / 100 != 0)
        return false;
    return true;
}
void proccessPatternScore(SavedScores &ss, int patternScore)
{
    if (validPatternScore(patternScore))
    {
        ss.insert(patternScore);
        // cout << ".";
    }
}
int truePatternScore(char pattern[200][200], int rows, int cols)
{
    int originalPatternScore = patternScore(pattern, rows, cols);
    int originalMirrorRow = originalPatternScore / 100;
    int originalMirrorCol = originalPatternScore % 100;
    // cout << originalPatternScore << endl;
    SavedScores savedScores;

    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
        {
            if (pattern[i][j] == '#')
                pattern[i][j] = '.';
            else
                pattern[i][j] = '#';

            int newPatternScore = patternScore(pattern, rows, cols, originalMirrorRow, originalMirrorCol);
            // cout << newPatternScore << " " << i << " " << j;
            proccessPatternScore(savedScores, newPatternScore);
            // cout << endl;

            if (pattern[i][j] == '.')
                pattern[i][j] = '#';
            else
                pattern[i][j] = '.';
        }
    // cout << endl;
    int trueScore = savedScores.uniqueScore();
    return trueScore;
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
        patternScoreSum += truePatternScore(pattern, rows, cols);
    fout << patternScoreSum;
}