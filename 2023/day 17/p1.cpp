#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <map>
#include <queue>

#define MAX_STEPS 3

using namespace std;
ifstream fin("p1.in");
ofstream fout("p1.out");
struct PathPoint
{
    int row, col;
    int distance;
    char allowedAxis; // allowed axis is either N / H / V (none, horizontal, vertical)
    PathPoint(int r, int c, char ax, int d)
    {
        row = r;
        col = c;
        allowedAxis = ax;
        distance = d;
    }
};
struct PathPointCmp
{
    bool operator()(PathPoint p1, PathPoint p2) const
    {
        if (p1.distance != p2.distance)
            return p1.distance < p2.distance;
        if (p1.row != p2.row)
            return p1.row < p2.row;
        if (p1.col != p2.col)
            return p1.col < p2.col;
        if (p1.allowedAxis != p2.allowedAxis)
            return p1.allowedAxis < p2.allowedAxis;
        return false;
    }
};
bool validPoint(const int &row, const int &col, const int &rows, const int &cols)
{
    if (row < 0 || col < 0 || row >= rows || col >= cols)
        return false;
    return true;
}
void updatePoint(set<PathPoint, PathPointCmp> &points, PathPoint newPoint, int oldDistance)
{
    int newDistance = newPoint.distance;
    newPoint.distance = oldDistance;
    if (points.find(newPoint) != points.end())
        points.erase(newPoint);
    newPoint.distance = newDistance;
    points.insert(newPoint);
}
void proccessNextPoint(set<PathPoint, PathPointCmp> &points,
                       int heatMap[200][200], const int &rows, const int &cols,
                       int horizontalPointCache[200][200], int verticalPointCache[200][200],
                       const PathPoint &point, char to, int steps)
{
    // to is either U / D / L / R
    PathPoint newPoint = point;
    switch (to)
    {
    case 'U':
        newPoint.row = newPoint.row - steps;
        newPoint.allowedAxis = 'H';

        break;
    case 'D':
        newPoint.row = newPoint.row + steps;
        newPoint.allowedAxis = 'H';

        break;
    case 'L':
        newPoint.col = newPoint.col - steps;
        newPoint.allowedAxis = 'V';

        break;
    case 'R':
        newPoint.col = newPoint.col + steps;
        newPoint.allowedAxis = 'V';
        break;
    default:
        cout << "ERROR: invalid to in proccessNextPoint: " << to << endl;
        return;
    }
    newPoint.distance = point.distance;
    bool lowerDistance;
    if (validPoint(newPoint.row, newPoint.col, rows, cols))
        switch (to)
        {
        case 'U':
            for (int step = 1; step <= steps; step++)
                newPoint.distance += heatMap[point.row - step][point.col];
            lowerDistance = horizontalPointCache[newPoint.row][newPoint.col] > newPoint.distance;
            if (lowerDistance)
            {
                updatePoint(points,
                            newPoint,
                            horizontalPointCache[newPoint.row][newPoint.col]);
                horizontalPointCache[newPoint.row][newPoint.col] = newPoint.distance;
            }
            break;
        case 'D':
            for (int step = 1; step <= steps; step++)
                newPoint.distance += heatMap[point.row + step][point.col];
            lowerDistance = horizontalPointCache[newPoint.row][newPoint.col] > newPoint.distance;
            if (lowerDistance)
            {
                updatePoint(points,
                            newPoint,
                            horizontalPointCache[newPoint.row][newPoint.col]);
                horizontalPointCache[newPoint.row][newPoint.col] = newPoint.distance;
            }
            break;
        case 'L':
            for (int step = 1; step <= steps; step++)
                newPoint.distance += heatMap[point.row][point.col - step];
            lowerDistance = verticalPointCache[newPoint.row][newPoint.col] > newPoint.distance;
            if (lowerDistance)
            {
                updatePoint(points,
                            newPoint,
                            verticalPointCache[newPoint.row][newPoint.col]);
                verticalPointCache[newPoint.row][newPoint.col] = newPoint.distance;
            }
            break;
        case 'R':
            for (int step = 1; step <= steps; step++)
                newPoint.distance += heatMap[point.row][point.col + step];
            lowerDistance = verticalPointCache[newPoint.row][newPoint.col] > newPoint.distance;
            if (lowerDistance)
            {
                updatePoint(points,
                            newPoint,
                            verticalPointCache[newPoint.row][newPoint.col]);
                verticalPointCache[newPoint.row][newPoint.col] = newPoint.distance;
            }
            break;
        }
}
int heatDistance(int heatMap[200][200], const int &rows, const int &cols)
{
    int horizontalPointCache[200][200], verticalPointCache[200][200];
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
        {
            horizontalPointCache[i][j] = INT32_MAX;
            verticalPointCache[i][j] = INT32_MAX;
        }
    set<PathPoint, PathPointCmp> points;
    horizontalPointCache[0][0] = heatMap[0][0];
    verticalPointCache[0][0] = heatMap[0][0];
    points.insert(PathPoint(0, 0, 'N', heatMap[0][0]));
    int loopCount = 0;
    while (!points.empty())
    {
        loopCount++;
        PathPoint point = *points.begin();
        points.erase(point);
        for (int steps = 1; steps <= MAX_STEPS; steps++)
            switch (point.allowedAxis)
            {
            case 'N':
                proccessNextPoint(points, heatMap, rows, cols, horizontalPointCache, verticalPointCache, point, 'U', steps);
                proccessNextPoint(points, heatMap, rows, cols, horizontalPointCache, verticalPointCache, point, 'D', steps);
                proccessNextPoint(points, heatMap, rows, cols, horizontalPointCache, verticalPointCache, point, 'L', steps);
                proccessNextPoint(points, heatMap, rows, cols, horizontalPointCache, verticalPointCache, point, 'R', steps);
                break;
            case 'H':
                proccessNextPoint(points, heatMap, rows, cols, horizontalPointCache, verticalPointCache, point, 'L', steps);
                proccessNextPoint(points, heatMap, rows, cols, horizontalPointCache, verticalPointCache, point, 'R', steps);
                break;
            case 'V':
                proccessNextPoint(points, heatMap, rows, cols, horizontalPointCache, verticalPointCache, point, 'U', steps);
                proccessNextPoint(points, heatMap, rows, cols, horizontalPointCache, verticalPointCache, point, 'D', steps);
            }
    }
    return min(horizontalPointCache[rows - 1][cols - 1],
               verticalPointCache[rows - 1][cols - 1]) -
           heatMap[0][0];
}
void readHeatMap(int heatMap[200][200], int &rows, int &cols)
{
    string inputRow;
    rows = 0;
    cols = 0;
    while (getline(fin, inputRow))
    {
        if (inputRow.length() == 0)
            return;
        cols = 0;
        for (const char &c : inputRow)
        {
            heatMap[rows][cols] = c - '0';
            cols++;
        }
        rows++;
    }
}
int main()
{
    int heatMap[200][200], rows, cols;
    readHeatMap(heatMap, rows, cols);
    fout << heatDistance(heatMap, rows, cols);
    return 0;
}