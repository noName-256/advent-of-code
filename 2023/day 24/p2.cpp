#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <iomanip>
#define VELOCITY_BOUND 350
using namespace std;
ifstream fin("p2.in");
ofstream fout("p2.out");

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
ostream &operator<<(ostream &os, const Hailstone &h)
{
    os << h.x << "," << h.y << "," << h.z << "~" << h.vx << "," << h.vy << "," << h.vz << endl;
    return os;
}
struct Point
{
    long double x, y, z;
    bool isNull;
    Point(const bool _isNull)
    {
        if (_isNull)
            isNull = true;
        else
        {
            cout << "ERROR: Point constructor should not be called with isNull argument true";
        }
    }
    Point(long double _x = 0, long double _y = 0, long double _z = 0)
    {
        x = _x;
        y = _y;
        z = _z;
        isNull = false;
    }
};
ostream &operator<<(ostream &os, const Point &p)
{
    os << p.x << "," << p.y << "," << p.z << endl;
    return os;
}
Point hailstoneIntersection(const Hailstone &h1, const Hailstone &h2)
{
    // x==h1.x+h1.vx*t
    // x==h2.x+h2.vx*s
    // x==x
    // h1.x+h1.vx*t==h2.x+h2.vx*s

    // t==(h2.x+h2.vx*s-h1.x)/h1.vx
    // t==(h2.y+h2.vy*s-h1.y)/h1.vy
    // t==(h2.z+h2.vz*s-h1.z)/h1.vz

    // (h2.x+h2.vx*s-h1.x)/h1.vx == (h2.y+h2.vy*s-h1.y)/h1.vy
    // (h2.x+h2.vx*s-h1.x)*h1.vy == (h2.y+h2.vy*s-h1.y)*h1.vx
    // h2.x*h1.vy + h2.vx*h1.vy*s - h1.x*h1.vy == h2.y*h1.vx + h2.vy*h1.vx*s - h1.y*h1.vx
    // s(h2.vx*h1.vy-h2.vy*h1.vx)==h2.y*h1.vx - h2.x*h1.vy - h1.y*h1.vx +h1.x*h1.vy
    // s==(h2.y*h1.vx - h2.x*h1.vy - h1.y*h1.vx +h1.x*h1.vy)/(h2.vx*h1.vy-h2.vy*h1.vx)
    bool sameX = false, sameY = false, sameZ = false;
    // if ((h1.vx - h2.vx == 0))
    //     if (h1.x != h2.x)
    //         return Point(true);
    //     else
    //         cout << "ERROR: you have to treat this specific case(two lines with the same acceleration and the same coordinate)\n";
    // if ((h1.vy - h2.vy == 0))
    //     if (h1.y != h2.y)
    //         return Point(true);
    //     else
    //         cout << "ERROR: you have to treat this specific case(two lines with the same acceleration and the same coordinate)\n";
    // if ((h1.vz - h2.vz == 0))
    //     if (h1.z != h2.z)
    //         return Point(true);
    //     else
    //         cout << "ERROR: you have to treat this specific case(two lines with the same acceleration and the same coordinate)\n";
    long double s = ((long double)h2.y * h1.vx - h2.x * h1.vy - h1.y * h1.vx + h1.x * h1.vy) / (h2.vx * h1.vy - h2.vy * h1.vx);
    long double t = (h2.x + h2.vx * s - h1.x) / h1.vx;
    // if (t != (h2.z + h2.vz * s - h1.z) / h1.vz) // if z's don't intersect
    //     return Point(true);                     // return null point
    Point returnPoint(h1.x + h1.vx * t, h1.y + h1.vy * t, h1.z + h1.vz * t);
    if (isnan(returnPoint.x) || isnan(returnPoint.y) || isnan(returnPoint.z) ||
        isinf(returnPoint.x) || isinf(returnPoint.y) || isinf(returnPoint.z))
        returnPoint.isNull = true;
    return returnPoint;
}
bool sameNumber(const long double &n1, const long double &n2)
{
    if (abs(n1 - n2) > 0.0001)
        return false;
    return true;
}
bool pointOnHailstone(const Point &p, const Hailstone &h)
{
    // p.x=h.x+h.vx*t
    // h.vx*t==p.x-h.x
    // t==(p.x-h.x)/h.vx
    long double tx = (p.x - h.x) / h.vx;
    long double ty = (p.y - h.y) / h.vy;
    long double tz = (p.z - h.z) / h.vz;
    if (!sameNumber(tx, ty))
        return false;
    if (!sameNumber(ty, tz))
        return false;
    return true;
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
    for (int x = -VELOCITY_BOUND; x <= VELOCITY_BOUND; x++)
        for (int y = -VELOCITY_BOUND; y <= VELOCITY_BOUND; y++)
            for (int z = -VELOCITY_BOUND; z <= VELOCITY_BOUND; z++)
            {
                // simulate a hailstone
                if (x == -3 && y == 1 && z == 2)
                    cout << "idk";
                hailstones.begin()->vx -= x;
                hailstones.begin()->vy -= y;
                hailstones.begin()->vz -= z;
                (hailstones.begin() + 1)->vx -= x;
                (hailstones.begin() + 1)->vy -= y;
                (hailstones.begin() + 1)->vz -= z;

                Point intersectionPoint = hailstoneIntersection(*hailstones.begin(), *(hailstones.begin() + 1));
                if (!intersectionPoint.isNull)
                {
                    bool foundIntersectionPoint = true;
                    int i;
                    for (i = 2; i < min((int)hailstones.size(), 10); i++)
                    {
                        hailstones.at(i).vx -= x;
                        hailstones.at(i).vy -= y;
                        hailstones.at(i).vz -= z;
                        if (!pointOnHailstone(intersectionPoint, hailstones.at(i)))
                        {
                            foundIntersectionPoint = false;
                            break;
                        }
                    }
                    if (!foundIntersectionPoint)
                        for (i; i >= 0; i--)
                        {
                            hailstones.at(i).vx += x;
                            hailstones.at(i).vy += y;
                            hailstones.at(i).vz += z;
                        }
                    if (foundIntersectionPoint)
                    {
                        cout << x << " " << y << " " << z;
                        fout << fixed << setprecision(0) << intersectionPoint.x + intersectionPoint.y + intersectionPoint.z;
                        return 0;
                    }
                }
                else
                {
                    hailstones.begin()->vx += x;
                    hailstones.begin()->vy += y;
                    hailstones.begin()->vz += z;
                    (hailstones.begin() + 1)->vx += x;
                    (hailstones.begin() + 1)->vy += y;
                    (hailstones.begin() + 1)->vz += z;
                }
            }
    return 0;
}