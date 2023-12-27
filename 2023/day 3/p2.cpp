#include <iostream>
#include <fstream>
#include <string.h>

using namespace std;
ifstream fin("p2.in");
ofstream fout("p2.out");

bool isDigit(const char &c);
struct engine
{
    char schema[200][200];
    int lines;
    int cols;
    engine()
    {
        lines = 0;
        cols = 0;
    }
};
bool validCoords(const int &line, const int &col, const engine &e);
int getOuterNumber(const int &digitLine, const int &digitCol, const engine &e);
struct gear
{
    int line, col;
    engine *parentEngine;
    gear(const int &l, const int &c, engine &e)
    {
        line = l;
        col = c;
        parentEngine = &e;
    }
};
struct parts
{
    // structure that remembers which adjacent characters of a possible gear are digits/engine parts
    engine *parentEngine;
    gear *parentGear;
    bool upperLeft = false;
    bool upperMiddle = false;
    bool upperRight = false;
    bool left = false;
    bool right = false;
    bool lowerLeft = false;
    bool lowerMiddle = false;
    bool lowerRight = false;
    parts(engine &e, gear &g)
    {
        parentEngine = &e;
        parentGear = &g;
        // check for each adjacent character and if it is a digit(engine part) mark it as true
        if (validCoords(g.line - 1, g.col - 1, e) &&
            isDigit(e.schema[g.line - 1][g.col - 1]))
            upperLeft = true;

        if (validCoords(g.line - 1, g.col, e) &&
            isDigit(e.schema[g.line - 1][g.col]))
            upperMiddle = true;

        if (validCoords(g.line - 1, g.col + 1, e) &&
            isDigit(e.schema[g.line - 1][g.col + 1]))
            upperRight = true;

        if (validCoords(g.line, g.col - 1, e) &&
            isDigit(e.schema[g.line][g.col - 1]))
            left = true;

        if (validCoords(g.line, g.col + 1, e) &&
            isDigit(e.schema[g.line][g.col + 1]))
            right = true;

        if (validCoords(g.line + 1, g.col - 1, e) &&
            isDigit(e.schema[g.line + 1][g.col - 1]))
            lowerLeft = true;

        if (validCoords(g.line + 1, g.col, e) &&
            isDigit(e.schema[g.line + 1][g.col]))
            lowerMiddle = true;

        if (validCoords(g.line + 1, g.col + 1, e) &&
            isDigit(e.schema[g.line + 1][g.col + 1]))
            lowerRight = true;
    }
    int numberOfAdjacentParts()
    {
        // this is actually a bit more complicated than apparent
        // because if two digits are touching horizontally they count only as one part
        // for example the following gear and adjacent parts:
        // 8 3 5
        // . * .
        // . . .
        // counts only as one adjacent part
        // but this:
        // 2 . 6
        // . * .
        // . . .
        // counts as two
        int nr = 0;
        if (upperLeft) // if upperLeft is a digit
        {
            nr++;
            if (!upperMiddle && upperRight) // check if it is a separate digit from upperRight
                nr++;                       // if it is, count it
        }
        else if (upperMiddle) // upperMiddle will count as 1 even if upperRight is also a digit
            nr++;
        else if (upperRight)
            nr++;

        // left and right are separate anyways so we just have to check if they are digits
        if (left)
            nr++;
        if (right)
            nr++;

        // on the lower side it is the same as on the upper, so we will apply the same checks
        if (lowerLeft)
        {
            nr++;
            if (!lowerMiddle && lowerRight) // check if it is a separate digit from lowerRight
                nr++;                       // if it is, count it
        }
        else if (lowerMiddle) // lowerMiddle will count as 1 even if lowerRight is also a digit
            nr++;
        else if (lowerRight)
            nr++;
        return nr;
    }
    int gearRatio()
    {
        // same principle as the numberOfAdjacentParts function, but now we have to get the outer number of the digit
        // for example, consider the following schema:
        // 1 0 2 .
        // . . * .
        // . . . .
        // the digits that we found are 0 and 2, but the number that we actually need is 102
        const engine &e = *parentEngine;
        const gear &g = *parentGear;
        int p = 1;
        if (upperLeft)
        {
            p *= getOuterNumber(g.line - 1, g.col - 1, e);
            if (!upperMiddle && upperRight)
                p *= getOuterNumber(g.line - 1, g.col + 1, e);
        }
        else if (upperMiddle)
            p *= getOuterNumber(g.line - 1, g.col, e);
        else if (upperRight)
            p *= getOuterNumber(g.line - 1, g.col + 1, e);

        if (left)
            p *= getOuterNumber(g.line, g.col - 1, e);
        if (right)
            p *= getOuterNumber(g.line, g.col + 1, e);

        if (lowerLeft)
        {
            p *= getOuterNumber(g.line + 1, g.col - 1, e);
            if (!lowerMiddle && lowerRight)
                p *= getOuterNumber(g.line + 1, g.col + 1, e);
        }
        else if (lowerMiddle)
            p *= getOuterNumber(g.line + 1, g.col, e);
        else if (lowerRight)
            p *= getOuterNumber(g.line + 1, g.col + 1, e);

        return p;
    }
};
engine getEngineFromInput()
{
    engine e;
    char row[200];
    int i = 0, j;
    while (fin.getline(row, 200))
    {
        j = 0;
        while (row[j])
        {
            e.schema[i][j] = row[j];
            j++;
        }
        i++;
    }
    e.lines = i;
    e.cols = j;
    return e;
}
bool validCoords(const int &line, const int &col, const engine &e)
{
    if (line < 0)
        return false;
    if (line >= e.lines)
        return false;
    if (col < 0)
        return false;
    if (col >= e.cols)
        return false;
    return true;
}
bool isDigit(const char &c)
{
    return ((c >= '0') && (c <= '9'));
}
int getOuterNumber(const int &digitLine, const int &digitCol, const engine &e)
{
    int startCol = digitCol, endCol = digitCol;
    while (isDigit(e.schema[digitLine][startCol]))
        startCol--;
    startCol++;
    while (isDigit(e.schema[digitLine][endCol]))
        endCol++;
    endCol--;
    char outerNumber[20];
    strncpy(outerNumber, e.schema[digitLine] + startCol, endCol - startCol + 1);
    outerNumber[endCol - startCol + 1] = '\0';
    return atoi(outerNumber);
}
int gearRatiosSum(engine &e)
{
    int i, j, sum = 0;
    for (i = 0; i < e.lines; i++)
        for (j = 0; j < e.cols; j++)
            if (e.schema[i][j] == '*')
            {
                // found a possible gear
                gear g(i, j, e);
                parts p(e, g);
                if (p.numberOfAdjacentParts() == 2)
                {
                    // found a gear
                    sum += p.gearRatio();
                }
            }
    return sum;
}
int main()
{
    engine e = getEngineFromInput();
    fout << gearRatiosSum(e);
}