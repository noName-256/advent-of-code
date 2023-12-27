/*
slideRocksInRegion(char dish[ARRAY_SIZE][ARRAY_SIZE], int rows, int cols,
                    int rocksStart, int rocksEnd, int rocksNumber)
slideRocksNorthOnColumn(char dish[ARRAY_SIZE][ARRAY_SIZE], int rows, int cols, int col)
slideRocksNorth(char dish[ARRAY_SIZE][ARRAY_SIZE], int rows, int cols)
calcRocksLoad(char dish[ARRAY_SIZE][ARRAY_SIZE], int rows, int cols)
readDish(char dish[ARRAY_SIZE][ARRAY_SIZE], int &rows, int &cols)
 */
#include <iostream>
#include <fstream>
#include <map>
#include <algorithm>

#define ARRAY_SIZE 200

using namespace std;
ifstream fin("p2.in");
ofstream fout("p2.out");
void slideRocksInRegion(char dish[ARRAY_SIZE][ARRAY_SIZE], int col,
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
void slideRocksOnColumn(char dish[ARRAY_SIZE][ARRAY_SIZE], int rows, int cols, int col)
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
void slideRocksNorth(char dish[ARRAY_SIZE][ARRAY_SIZE], int rows, int cols)
{
    for (int j = 0; j < cols; j++)
        slideRocksOnColumn(dish, rows, cols, j);
}
int calcRocksLoad(char dish[ARRAY_SIZE][ARRAY_SIZE], int rows, int cols)
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
void copyTable(char table1[ARRAY_SIZE][ARRAY_SIZE], char table2[ARRAY_SIZE][ARRAY_SIZE])
{
    for (int i = 0; i < ARRAY_SIZE; i++)
        for (int j = 0; j < ARRAY_SIZE; j++)
            table1[i][j] = table2[i][j];
}
struct Dish
{
    char table[ARRAY_SIZE][ARRAY_SIZE];
    int tableRows, tableCols;
    void display(ostream &out = cout)
    {
        for (int i = 0; i < tableRows; i++)
        {
            for (int j = 0; j < tableCols; j++)
                out << table[i][j];
            out << endl;
        }
        out << endl;
    }
    void rotate()
    {
        char tableCopy[ARRAY_SIZE][ARRAY_SIZE];
        copyTable(tableCopy, table);
        swap(tableRows, tableCols);
        for (int i = 0; i < tableRows; i++)
            for (int j = 0; j < tableCols; j++)
                table[i][j] = tableCopy[j][i];
        for (int i = 0; i < tableRows; i++)
            for (int j = 0; j < tableCols / 2; j++)
                swap(table[i][j], table[i][tableCols - j - 1]);
    }
    void slideRocksUp()
    {
        slideRocksNorth(table, tableRows, tableCols);
    }
    void cycle()
    {
        slideRocksUp();
        rotate();
        slideRocksUp();
        rotate();
        slideRocksUp();
        rotate();
        slideRocksUp();
        rotate();
    }
    int rocksLoad()
    {
        return calcRocksLoad(table, tableRows, tableCols);
    }
    Dish(char _table[200][200], int _tableRows, int _tableCols)
    {
        tableRows = _tableRows;
        tableCols = _tableCols;
        copyTable(table, _table);
    }
};
struct DishCmp
{
    bool operator()(const Dish &d1, const Dish &d2) const
    {
        for (int i = 0; i < d1.tableRows; i++)
            for (int j = 0; j < d1.tableCols; j++)
                if (d1.table[i][j] != d2.table[i][j])
                    return d1.table[i][j] < d2.table[i][j];
        return false;
    }
};
struct
{
    map<Dish, int, DishCmp> cycles;
    int cThatRepeats;
    int howManyCyclesForCToRepeat;
    bool has(const Dish &d) const
    {
        if (cycles.find(d) != cycles.end())
            return true;
        return false;
    }
    void insert(const Dish d)
    {
        cycles.insert(pair<Dish, int>(d, cycles.size()));
    }
    void foundRepeatingCycle(Dish d)
    {
        cThatRepeats = cycles.find(d)->second;
        howManyCyclesForCToRepeat = cycles.size() - cThatRepeats;
        // cout << cThatRepeats << " " << howManyCyclesForCToRepeat << endl;
    }
    Dish getDishAfterNCycles(int n)
    {
        while (n > cycles.size())
            n -= howManyCyclesForCToRepeat;
        for (auto p : cycles)
            if (p.second == n)
                return p.first;
    }
} cycleCache;
void readDish(char dish[ARRAY_SIZE][ARRAY_SIZE], int &rows, int &cols)
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
    char table[ARRAY_SIZE][ARRAY_SIZE];
    int rows, cols;
    int totalLoad = 0;
    readDish(table, rows, cols);
    Dish dish(table, rows, cols);
    cycleCache.insert(dish);
    dish.cycle();
    do
    {
        cycleCache.insert(dish);
        dish.cycle();
    } while (!cycleCache.has(dish));
    cycleCache.foundRepeatingCycle(dish);
    Dish dishAfterCycles = cycleCache.getDishAfterNCycles(1000000000);
    // dishAfterCycles.display();
    totalLoad = dishAfterCycles.rocksLoad();
    fout << totalLoad;
}