#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#define UNIVERSE_EXPANSION 999999

using namespace std;
ifstream fin("p2.in");
ofstream fout("p2.out");
struct Point
{
    long long row, col;
    Point(const long long r, const long long c)
    {
        row = r;
        col = c;
    }
};
bool anyGalaxiesOnRow(const vector<Point> &galaxies, const long long row)
{
    for (const Point &galaxy : galaxies)
        if (galaxy.row == row)
            return true;
    return false;
}
bool anyGalaxiesOnCol(const vector<Point> &galaxies, const long long col)
{
    for (const Point &galaxy : galaxies)
        if (galaxy.col == col)
            return true;
    return false;
}
void expandEmptyRows(long long universe[500][500], long long rows, long long cols, const vector<Point> &galaxies)
{
    for (long long i = 0; i < rows; i++)
    {
        if (!anyGalaxiesOnRow(galaxies, i))
        {
            // found row with no galaxies
            // this row will expand because of "universe laws"
            for (long long j = 0; j < cols; j++)
                universe[i][j] += UNIVERSE_EXPANSION;
        }
    }
}
void expandEmptyColumns(long long universe[500][500], long long rows, long long cols, const vector<Point> &galaxies)
{
    for (long long j = 0; j < cols; j++)
    {
        if (!anyGalaxiesOnCol(galaxies, j))
        {
            // found column with no galaxies
            // this column will expand because of "universe laws"
            for (long long i = 0; i < rows; i++)
                universe[i][j] += UNIVERSE_EXPANSION;
        }
    }
}
void readUniverse(long long universe[500][500], long long &rows, long long &cols,
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
long long distanceBetweenGalaxies(const Point galaxy1, const Point galaxy2, long long universe[500][500])
{
    Point g1 = galaxy1, g2 = galaxy2;
    long long distance = 0;
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
    long long universe[500][500], rows, cols;
    vector<Point> galaxies;
    readUniverse(universe, rows, cols, galaxies);
    long long distancesSum = 0;
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