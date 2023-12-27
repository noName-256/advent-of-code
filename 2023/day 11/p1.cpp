#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#define UNIVERSE_EXPANSION 1

using namespace std;
ifstream fin("p1.in");
ofstream fout("p1.out");
struct Point
{
    int row, col;
    Point(const int r, const int c)
    {
        row = r;
        col = c;
    }
};
bool anyGalaxiesOnRow(const vector<Point> &galaxies, const int row)
{
    for (const Point &galaxy : galaxies)
        if (galaxy.row == row)
            return true;
    return false;
}
bool anyGalaxiesOnCol(const vector<Point> &galaxies, const int col)
{
    for (const Point &galaxy : galaxies)
        if (galaxy.col == col)
            return true;
    return false;
}
void expandEmptyRows(int universe[500][500], int rows, int cols, const vector<Point> &galaxies)
{
    for (int i = 0; i < rows; i++)
    {
        if (!anyGalaxiesOnRow(galaxies, i))
        {
            // found row with no galaxies
            // this row will expand because of "universe laws"
            for (int j = 0; j < cols; j++)
                universe[i][j] += UNIVERSE_EXPANSION;
        }
    }
}
void expandEmptyColumns(int universe[500][500], int rows, int cols, const vector<Point> &galaxies)
{
    for (int j = 0; j < cols; j++)
    {
        if (!anyGalaxiesOnCol(galaxies, j))
        {
            // found column with no galaxies
            // this column will expand because of "universe laws"
            for (int i = 0; i < rows; i++)
                universe[i][j] += UNIVERSE_EXPANSION;
        }
    }
}
void readUniverse(int universe[500][500], int &rows, int &cols,
                  vector<Point> &galaxies)
{
    string inputRow;
    rows = 0;
    cols = 0;
    while (getline(fin, inputRow))
    {
        cols = 0;
        for (const char &c : inputRow)
        {
            universe[rows][cols] = 1;
            if (c == '#')
                galaxies.push_back(Point(rows, cols)); // found galaxy, store it
            cols++;
        }
        rows++;
    }
    expandEmptyRows(universe, rows, cols, galaxies);
    expandEmptyColumns(universe, rows, cols, galaxies);
}
int distanceBetweenGalaxies(const Point galaxy1, const Point galaxy2, int universe[500][500])
{
    Point g1 = galaxy1, g2 = galaxy2;
    int distance = 0;
    while (g1.row < g2.row)
    {
        distance += universe[g1.row][g1.col];
        g1.row++;
    }
    while (g1.row > g2.row)
    {
        distance += universe[g1.row][g1.col];
        g1.row--;
    }
    while (g1.col < g2.col)
    {
        distance += universe[g1.row][g1.col];
        g1.col++;
    }
    while (g1.col > g2.col)
    {
        distance += universe[g1.row][g1.col];
        g1.col--;
    }
    return distance;
}
int main()
{
    int universe[500][500], rows, cols;
    vector<Point> galaxies;
    readUniverse(universe, rows, cols, galaxies);
    int distancesSum = 0;
    for (auto galaxy1Iterator = galaxies.begin();
         galaxy1Iterator != galaxies.end();
         galaxy1Iterator++)
    {
        auto galaxy2Iterator = galaxy1Iterator;
        for (; galaxy2Iterator != galaxies.end(); galaxy2Iterator++)
        {
            Point galaxy1 = *galaxy1Iterator;
            Point galaxy2 = *galaxy2Iterator;
            distancesSum += distanceBetweenGalaxies(galaxy1, galaxy2, universe);
        }
    }
    fout << distancesSum;
}