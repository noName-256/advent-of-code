#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <map>
#include <vector>
#include <queue>

using namespace std;
ifstream fin("p1.in");
ofstream fout("p1.out");
struct Point
{
    int row, col;
    Point(int r = 0, int c = 0)
    {
        row = r;
        col = c;
    }
};
struct PointCmp
{
    bool operator()(const Point &c1, const Point &c2) const
    {
        if (c1.row != c2.row)
            return c1.row < c2.row;
        if (c1.col != c2.col)
            return c1.col < c2.col;
        return false;
    }
};
bool validPoint(const int &rows, const int &cols, int row, int col)
{
    if (row < 0 || col < 0 || row >= rows || col >= cols)
        return false;
    return true;
}
bool validPoint(const int &rows, const int &cols, Point p)
{
    if (p.row < 0 || p.col < 0 || p.row >= rows || p.col >= cols)
        return false;
    return true;
}
int countAvailablePoints(char hikingTrail[200][200], const int &rows, const int &cols,
                         int row, int col)
{
    int count = 0;
    if (validPoint(rows, cols, row - 1, col) && hikingTrail[row - 1][col] != '#')
        count++;
    if (validPoint(rows, cols, row - 1, col) && hikingTrail[row + 1][col] != '#')
        count++;
    if (validPoint(rows, cols, row - 1, col) && hikingTrail[row][col - 1] != '#')
        count++;
    if (validPoint(rows, cols, row - 1, col) && hikingTrail[row][col + 1] != '#')
        count++;
    return count;
}
void findNextPointsInHikingTrail(char hikingTrail[200][200], const int &rows, const int &cols,
                                 const map<Point, vector<pair<Point, int>>, PointCmp> &graphMap,
                                 const Point &p, vector<pair<Point, int>> &nextPoints)
{
    // we use bfs to find the next points that are present in graphMap
    struct State
    {
        Point point;
        int stepsFromOrigin;
        State(const Point &_p, const int steps)
        {
            point = _p;
            stepsFromOrigin = steps;
        }
    };
    queue<State> states;
    set<Point, PointCmp> passedPoints;
    states.push(State(p, 0));
    while (!states.empty())
    {
        State s = states.front();
        states.pop();
        if (!validPoint(rows, cols, s.point))
            continue;
        if (hikingTrail[s.point.row][s.point.col] == '#')
            continue;
        bool passedThisPointBefore = passedPoints.find(s.point) != passedPoints.end();
        if (passedThisPointBefore)
            continue;
        passedPoints.insert(s.point);
        bool notInitialPoint = s.point.row != p.row || s.point.col != p.col;
        bool pointInGraphMap = graphMap.find(s.point) != graphMap.end();
        if (notInitialPoint && pointInGraphMap)
        {
            // found important point
            // add it to nextPoints
            nextPoints.push_back(make_pair(s.point, s.stepsFromOrigin));
        }
        // add next states, but do not if we found a graph point
        else
            switch (hikingTrail[s.point.row][s.point.col])
            {
            case '.':
                // add all next states
                states.push(State(Point(s.point.row - 1, s.point.col), s.stepsFromOrigin + 1));
                states.push(State(Point(s.point.row + 1, s.point.col), s.stepsFromOrigin + 1));
                states.push(State(Point(s.point.row, s.point.col - 1), s.stepsFromOrigin + 1));
                states.push(State(Point(s.point.row, s.point.col + 1), s.stepsFromOrigin + 1));
                break;
            case '^':
                states.push(State(Point(s.point.row - 1, s.point.col), s.stepsFromOrigin + 1));
                break;
            case 'v':
                states.push(State(Point(s.point.row + 1, s.point.col), s.stepsFromOrigin + 1));
                break;
            case '<':
                states.push(State(Point(s.point.row, s.point.col - 1), s.stepsFromOrigin + 1));
                break;
            case '>':
                states.push(State(Point(s.point.row, s.point.col + 1), s.stepsFromOrigin + 1));
                break;
            default:
                cout << "ERROR: Invalid hiking trail character: " << hikingTrail[s.point.row][s.point.col] << endl;
            }
    }
}
void getHikingTrailMap(char hikingTrail[200][200], const int &rows, const int &cols,
                       map<Point, vector<pair<Point, int>>, PointCmp> &graphMap)
{
    // Since a lot of the trails are just 1 way roads that have no intersections
    // we will remember in the graph map just the: starting point, ending point and the intersection points
    // The graph map remembers for each point a list of pairs of a point and a number
    // representing the number of steps to get from the point to the point in the vector

    // Example: point(2,0) -> {[point(3,0), 1], [point(7,5), 20]}
    // means that you can get from point(2,0) to point(3,0) in 1 step
    // and from point(2,0) to point(7,5) in 20 steps
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
        {
            if ((hikingTrail[i][j] != '#' && countAvailablePoints(hikingTrail, rows, cols, i, j) >= 3) ||
                (i == 0 && j == 1) ||
                (i == rows - 1 && j == cols - 2))
            {
                // found intersection point
                vector<pair<Point, int>> mockNextPoints;
                Point p(i, j);
                auto graphNode = make_pair(p, mockNextPoints);
                graphMap.insert(graphNode);
            }
        }
    for (auto &graphNode : graphMap)
        findNextPointsInHikingTrail(hikingTrail, rows, cols, graphMap, graphNode.first, graphNode.second);
    // for (const auto &graphNode : graphMap)
    // {
    // cout << "Point(" << graphNode.first.row << "," << graphNode.first.col << "):\n";
    // for (const auto nextPoint : graphNode.second)
    // cout << "Point(" << nextPoint.first.row << "," << nextPoint.first.col << "), at " << nextPoint.second << " steps away\n";
    // cout << endl;
    // }
}
int maximumDistance(map<Point, vector<pair<Point, int>>, PointCmp> &graphMap, set<Point, PointCmp> &passedPoints,
                    const Point &fromP, const Point &toP)
{
    if (fromP.row == toP.row && fromP.col == toP.col)
        return 0;
    if (passedPoints.find(fromP) != passedPoints.end())
        return -1;
    auto &nextPoints = graphMap.find(fromP)->second;
    int maxDistance = -1;
    passedPoints.insert(fromP);
    for (const auto &nextPoint : nextPoints)
    {
        const Point &nextP = nextPoint.first;
        int distanceToNextPoint = nextPoint.second;
        int distanceFromNextPoint = maximumDistance(graphMap, passedPoints, nextP, toP);
        if (distanceFromNextPoint != -1)
            maxDistance = max(maxDistance, distanceToNextPoint + distanceFromNextPoint);
    }
    passedPoints.erase(fromP);
    return maxDistance;
}
void readHikingTrail(char hikingTrail[200][200], int &rows, int &cols)
{
    string inputRow;
    rows = 0;
    while (getline(fin, inputRow))
    {
        cols = 0;
        for (const char &c : inputRow)
        {
            hikingTrail[rows][cols] = c;
            cols++;
        }
        rows++;
    }
}
int main()
{
    char hikingTrail[200][200];
    int rows, cols;
    readHikingTrail(hikingTrail, rows, cols);
    map<Point, vector<pair<Point, int>>, PointCmp> graphMap;
    getHikingTrailMap(hikingTrail, rows, cols, graphMap);
    set<Point, PointCmp> passedPoints;
    fout << maximumDistance(graphMap, passedPoints, Point(0, 1), Point(rows - 1, cols - 2));
    return 0;
}