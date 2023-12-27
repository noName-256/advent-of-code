#include <iostream>
#include <fstream>
#include <vector>
#define START_RANGE 200000000000000
#define END_RANGE 400000000000000
using namespace std;
ifstream fin("p1.in");
ofstream fout("p1.out");

struct Hailstone
{
    long long x, y, z;    // original position
    long long vx, vy, vz; // velocity
    Hailstone()
    {
        char aux;
        if (!(fin >> x >> aux >> y >> aux >> z >> aux >> vx >> aux >> vy >> aux >> vz))
        {
            // give invalid coordinates if can't read
            x = -1;
            y = -1;
            z = -1;
            vx = 0;
            vy = 0;
            vz = 0;
        }
    }
};
struct Point
{
    long double x, y;
    bool isNull;
    Point(bool _isNull)
    {
        if (_isNull)
            isNull = _isNull;
        else
        {
            cout << "ERROR: Point constructor should not be called with isNull argument true";
        }
    }
    Point(const long double _x = 0, const long double _y = 0)
    {
        x = _x;
        y = _y;
    }
};
struct Segment
{
    long double equationA, equationB;
    long double originalX;
    long double originalY;
    bool xGrows; // true if x velocity > 0, false if x velocity < 0 (there is no 0 velocity in the input data)
    bool yGrows; // true if y velocity > 0, false if y velocity < 0 (there is no 0 velocity in the input data)
    bool pointOnSegment(const long double pointX, const long double pointY) const
    {
        // We will assume that points given satisfy the equation
        // If we want to check that we can uncomment the lines below
        // but we will need to use an epsilon as floating point calculations are sketchy

        // if (pointY != xToY(pointX))
        // return false;

        if (xGrows && originalX > pointX)
            return false;
        if (!xGrows && originalX < pointX)
            return false;
        if (yGrows && originalY > pointY)
            return false;
        if (!yGrows && originalY < pointY)
            return false;
        return true;
    }
    bool pointOnSegment(const Point p) const
    {
        return pointOnSegment(p.x, p.y);
    }
    long double xToY(const long double x) const
    {
        return x * equationA + equationB;
    }
    Segment(const Hailstone h)
    {
        long long point1X = h.x;
        long long point1Y = h.y;
        long long point2X = h.x + h.vx;
        long long point2Y = h.y + h.vy;
        equationA = ((long double)point1Y - point2Y) / (point1X - point2X);
        equationB = point1Y - point1X * equationA;
        originalX = point1X;
        originalY = point1Y;
        xGrows = h.vx > 0;
        yGrows = h.vy > 0;
    }
};
Point segmentIntersection(const Segment s1, const Segment s2,
                          const long long startRange, const long long endRange)
{
    // cout << s2.xToY(s2.originalX) << " " << s2.originalY << " " << (s2.xToY(s2.originalX) == s2.originalY) << endl;
    long double intersectionX = (s2.equationB - s1.equationB) / (s1.equationA - s2.equationA);
    long double intersectionY = s1.xToY(intersectionX);
    Point p(intersectionX, intersectionY);
    if (s1.equationA - s2.equationA == 0) // if same slope(parallel segments)
        if (s2.equationB - s1.equationB == 0)
        { // if same lines
            cout << "Same segments...\n";
            return Point(false); // return non-null point
        }
        else
            return Point(true); // return null point
    if (s1.pointOnSegment(p) && s2.pointOnSegment(p) &&
        (intersectionX >= startRange && intersectionX <= endRange) &&
        (intersectionY >= startRange && intersectionY <= endRange))
        return p;
    else
    {
        Point nullPoint(true);
        return nullPoint;
    }
}
int main()
{
    vector<Hailstone> hailstones;
    Hailstone h;
    while (h.x != -1)
    {
        hailstones.push_back(h);
        h = Hailstone();
    }
    vector<Segment> hailstoneSegments;
    long long segmentIntersectionCount = 0;
    for (const Hailstone &h : hailstones)
        hailstoneSegments.push_back(Segment(h));
    for (auto it = hailstoneSegments.begin(); it != hailstoneSegments.end(); it++)
    {
        Segment s1 = *it;
        for (auto it2 = it + 1; it2 != hailstoneSegments.end(); it2++)
        {
            Segment s2 = *it2;
            if (!segmentIntersection(s1, s2, START_RANGE, END_RANGE).isNull)
                segmentIntersectionCount++;
        }
    }
    fout << segmentIntersectionCount;
    return 0;
}