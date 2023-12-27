#include <iostream>
#include <fstream>
#include <set>

using namespace std;
ifstream fin("p2.in");
ofstream fout("p2.out");
struct Brick
{
    static int numberOfBricks;
    int startX, startY, startZ;
    int endX, endY, endZ;
    int id;
    set<int> supportedByBricksIDs;
    Brick(int sx = 0, int sy = 0, int sz = 0,
          int ex = 0, int ey = 0, int ez = 0,
          int newID = 0)
    {
        startX = sx;
        startY = sy;
        startZ = sz;
        endX = ex;
        endY = ey;
        endZ = ez;
        if (newID)
            id = newID;
        else
        {
            numberOfBricks++;
            id = numberOfBricks;
        }
    }
};
int Brick ::numberOfBricks = 0;
struct
{
private:
    bool intervalsIntersect(int startInterval1, int endInterval1, int startInterval2, int endInterval2)
    {
        if (startInterval1 < startInterval2)
        {
            if (endInterval1 < startInterval2)
                return false;
            return true;
        }
        if (startInterval1 > startInterval2)
        {
            if (endInterval2 < startInterval1)
                return false;
            return true;
        }
        return true;
    }

public:
    bool bricksIntersectXY(const Brick &b1, const Brick &b2)
    {
        return intervalsIntersect(b1.startX, b1.endX, b2.startX, b2.endX) &&
               intervalsIntersect(b1.startY, b1.endY, b2.startY, b2.endY);
    }
} xyIntersectCalculator;
struct BrickCmp
{
    bool operator()(const Brick &b1, const Brick &b2) const
    {
        if (b1.startZ != b2.startZ)
            return b1.startZ < b2.startZ;
        if (b1.startY != b2.startY)
            return b1.startY < b2.startY;
        if (b1.startX != b2.startX)
            return b1.startX < b2.startX;
        return false;
    }
};
int makeBricksFall(set<Brick, BrickCmp> &bricks)
{
    // returns the amount of bricks that can individually be broken without making other bricks fall
    set<Brick, BrickCmp> fallenBricks;
    set<Brick, BrickCmp> singleSupportingBricks;
    for (const Brick &b : bricks)
    {
        int maxAvailableZ = 1, bricksSupportingB = 0;
        Brick lastSupportingBrick;
        for (const Brick &fb : fallenBricks)
            if (xyIntersectCalculator.bricksIntersectXY(b, fb)) // if bricks intersect on xy axis
            {
                if (fb.endZ + 1 > maxAvailableZ)
                {
                    bricksSupportingB = 1;
                    maxAvailableZ = fb.endZ + 1;
                    lastSupportingBrick = fb;
                }
                else if (fb.endZ + 1 == maxAvailableZ)
                {
                    bricksSupportingB++;
                    lastSupportingBrick = fb;
                }
            }
        if (bricksSupportingB == 1)
            singleSupportingBricks.insert(lastSupportingBrick);
        Brick nb(b.startX, b.startY, maxAvailableZ,
                 b.endX, b.endY, maxAvailableZ + b.endZ - b.startZ,
                 b.id); // new brick
        fallenBricks.insert(nb);
    }
    bricks = fallenBricks;
    return bricks.size() - singleSupportingBricks.size();
}
int bricksFallCount(const set<Brick, BrickCmp> &bricks)
{
    // returns the amount of bricks that change/fall
    set<Brick, BrickCmp> fallenBricks;
    int count = 0;
    for (const Brick &b : bricks)
    {
        int maxAvailableZ = 1, bricksSupportingB = 0;
        Brick lastSupportingBrick;
        for (const Brick &fb : fallenBricks)
            if (xyIntersectCalculator.bricksIntersectXY(b, fb)) // if bricks intersect on xy axis
            {
                if (fb.endZ + 1 > maxAvailableZ)
                {
                    bricksSupportingB = 1;
                    maxAvailableZ = fb.endZ + 1;
                    lastSupportingBrick = fb;
                }
                else if (fb.endZ + 1 == maxAvailableZ)
                {
                    bricksSupportingB++;
                    lastSupportingBrick = fb;
                }
            }
        if (b.startZ != maxAvailableZ) // if brick has moved
            count++;
        Brick nb(b.startX, b.startY, maxAvailableZ,
                 b.endX, b.endY, maxAvailableZ + b.endZ - b.startZ,
                 b.id); // new brick
        fallenBricks.insert(nb);
    }
    return count;
}
void outputSpacesForTable(int n, ostream &out = fout)
{
    const int MAX_DIGITS = 2;
    int digits = 0;
    if (n == 0)
        digits = 1;
    else
        while (n)
        {
            digits++;
            n /= 10;
        }
    for (int i = 0; i <= MAX_DIGITS - digits; i++)
        fout << " ";
}
void outputBricks(const set<Brick, BrickCmp> bricks)
{
    int maxX = 0; /* 10 */  // maxX is the maximum number for the x coordinates + 1
    int maxY = 0; /* 10 */  // maxY is the maximum number for the y coordinates + 1
    int maxZ = 0; /* 350 */ // maxZ is the maximum number for the z coordinates + 1
    for (const Brick &b : bricks)
    {
        maxX = max(maxX, b.endX);
        maxY = max(maxY, b.endY);
        maxZ = max(maxZ, b.endZ);
    }
    maxX++;
    maxY++;
    maxZ++;
    int brickLayout[maxX][maxY][maxZ] = {0};
    for (int x = 0; x < maxX; x++)
        for (int y = 0; y < maxY; y++)
            for (int z = 0; z < maxZ; z++)
                brickLayout[x][y][z] = 0;
    for (const Brick &b : bricks)
    {
        for (int x = b.startX; x <= b.endX; x++)
            for (int y = b.startY; y <= b.endY; y++)
                for (int z = b.startZ; z <= b.endZ; z++)
                    brickLayout[x][y][z] = b.id;
    }
    for (int z = 0; z < maxZ; z++)
    {
        fout << "z=" << z << "                                                                  " << endl;
        for (int x = 0; x < maxX; x++)
        {
            for (int y = 0; y < maxY; y++)
            {
                fout << brickLayout[x][y][z];
                outputSpacesForTable(brickLayout[x][y][z]);
            }
            fout << endl;
        }
        fout << "                                                                           " << endl;
    }
    fout << "                                                                                      " << endl;
}
bool readBrick(Brick &b)
{
    char aux;
    if (fin >> b.startX >> aux >> b.startY >> aux >> b.startZ >>
        aux >>
        b.endX >> aux >> b.endY >> aux >> b.endZ)
    {
        if (b.startX > b.endX)
            swap(b.startX, b.endX);
        if (b.startY > b.endY)
            swap(b.startY, b.endY);
        if (b.startZ > b.endZ)
            swap(b.startZ, b.endZ);
        return true;
    }
    return false;
}
int main()
{
    set<Brick, BrickCmp> bricks;
    Brick b(0, 0, 0, 0, 0, 0, 0);
    while (readBrick(b))
    {
        bricks.insert(b);
        b = Brick(0, 0, 0, 0, 0, 0, 0);
    }
    makeBricksFall(bricks);
    set<Brick, BrickCmp> bricksCopy = bricks;
    int sumBricksFallCount = 0;
    for (const Brick &b : bricks)
    {
        bricksCopy.erase(b);
        sumBricksFallCount += bricksFallCount(bricksCopy);
        bricksCopy.insert(b);
    }
    fout << sumBricksFallCount << endl;
    return 0;
}