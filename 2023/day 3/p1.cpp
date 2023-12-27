#include <iostream>
#include <fstream>
#include <string.h>

using namespace std;
ifstream fin("p1.in");
ofstream fout("p1.out");

void readEngine(char engine[200][200], int &noLines, int &noCols)
{
    char row[200];
    noLines = 0;
    while (fin.getline(row, 200))
    {
        if (!row[0])
            return;
        noCols = 0;
        while (row[noCols])
        {
            engine[noLines][noCols] = row[noCols];
            noCols++;
        }
        noLines++;
    }
}
bool hasAdjacentSymbol(char engine[200][200], char *symbols,
                       const int &noLines, const int &noCols,
                       const int &line, const int &col1, const int &col2)
{
    int i, j;
    for (i = line - 1; i <= line + 1; i++)
        for (j = col1 - 1; j <= col2 + 1; j++)
        {
            if (i < 0)
                continue;
            if (j < 0)
                continue;
            if (i >= noLines)
                continue;
            if (j >= noCols)
                continue;
            if (strchr(symbols, engine[i][j]) != NULL)
                return true;
        }
    return false;
}
int enginePartNumberSum(char engine[200][200], const int &noLines, const int &noCols)
{
    int i, j, k, partNumberSum = 0;
    char symbols[30];
    strcpy(symbols, "!@#$%^&*()/-_+=\\");
    for (i = 0; i < noLines; i++)
        for (j = 0; j < noCols; j++)
        {
            if ((engine[i][j] >= '0') && (engine[i][j] <= '9'))
            {
                k = j + 1;
                while ((engine[i][k] >= '0') && (engine[i][k] <= '9'))
                    k++;
                k--;
                bool isEnginePart = hasAdjacentSymbol(engine, symbols, noLines, noCols, i, j, k);
                if (isEnginePart)
                {
                    char enginePart[20];
                    strncpy(enginePart, engine[i] + j, k - j + 1);
                    enginePart[k - j + 1] = '\0';
                    int partNumber = atoi(enginePart);
                    partNumberSum += partNumber;
                }
                j = k + 1;
            }
        }
    return partNumberSum;
}
int main()
{
    char engine[200][200];
    int noLines, noCols;
    readEngine(engine, noLines, noCols);
    fout << enginePartNumberSum(engine, noLines, noCols);
}