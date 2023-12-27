/*
slideRocksInRegion(char dish[200][200], int rows, int cols,
                    int rocksStart, int rocksEnd, int rocksNumber)
slideRocksNorthOnColumn(char dish[200][200], int rows, int cols, int col)
slideRocksNorth(char dish[200][200], int rows, int cols)
calcRocksLoad(char dish[200][200], int rows, int cols)
readDish(char dish[200][200], int &rows, int &cols)
 */
#include <iostream>
#include <fstream>

using namespace std;
ifstream fin("p1.in");
ofstream fout("p1.out");
void slideRocksInRegion(char dish[200][200], int col,
                        int rocksStart, int rocksEnd, int rocksNumber)
{
    if (rocksStart < 0 || rocksEnd < 0)
        return;
    for (int i = rocksStart; i <= rocksEnd; i++)
        if (rocksNumber)
        {
            dish[i][col] = 'O';
            rocksNumber--;
        }
        else
            dish[i][col] = '.';
}
void slideRocksOnColumn(char dish[200][200], int rows, int cols, int col)
{
    int regionStart = 0, regionRocks = 0;
    for (int i = 0; i < rows; i++)
        if (dish[i][col] == 'O')
            regionRocks++;
        else if (dish[i][col] == '#')
        {
            // new region
            slideRocksInRegion(dish, col, regionStart, i - 1, regionRocks);
            while (dish[i + 1][col] == '#')
                i++;
            regionStart = i + 1;
            regionRocks = 0;
        }
    if (regionStart < rows)
        slideRocksInRegion(dish, col, regionStart, rows - 1, regionRocks);
}
void slideRocksNorth(char dish[200][200], int rows, int cols)
{
    for (int j = 0; j < cols; j++)
        slideRocksOnColumn(dish, rows, cols, j);
}
int calcRocksLoad(char dish[200][200], int rows, int cols)
{
    int totalLoad = 0;
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            // cout << dish[i][j];
            if (dish[i][j] == 'O')
                totalLoad += rows - i;
        }
        // cout << endl;
    }
    return totalLoad;
}
void readDish(char dish[200][200], int &rows, int &cols)
{
    string inputRow;
    rows = 0;
    cols = 0;
    while (getline(fin, inputRow))
    {
        if (inputRow.length() == 0)
            break;
        cols = 0;
        for (char c : inputRow)
        {
            dish[rows][cols] = c;
            cols++;
        }
        rows++;
    }
}
int main()
{
    char dish[200][200];
    int rows, cols;
    int totalLoad = 0;
    readDish(dish, rows, cols);
    slideRocksNorth(dish, rows, cols);
    totalLoad = calcRocksLoad(dish, rows, cols);
    fout << totalLoad;
}