#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <math.h>
#include <sstream>

using namespace std;
ifstream fin("p2.in");
ofstream fout("p2.out");

struct Instruction
{
    int steps;
    char direction;
    string color;
    Instruction(int s, char d, string col)
    {
        steps = s;
        direction = d;
        color = col;
    }
};
struct Point
{
    int x, y;
    Point(int _x, int _y)
    {
        x = _x;
        y = _y;
    }
};
struct Polygon
{
    vector<Point> points;
    int perimeter()
    {
        long long p = 0;
        for (auto it = points.begin(); it + 1 != points.end(); it++)
        {
            Point p1 = *it, p2 = *(it + 1);
            p += abs((long long)p1.x - p2.x + p1.y - p2.y);
        }
        return p;
    }
    long long area()
    {
        long long totalArea = 0;
        for (auto it = points.begin(); it + 1 != points.end(); it++)
        {
            Point p1 = *it, p2 = *(it + 1);
            totalArea += (long long)p1.x * p2.y - (long long)p2.x * p1.y;
        }
        totalArea = abs(totalArea) / 2;
        return totalArea;
    }
    Polygon(const vector<Instruction> &instructions)
    {
        int currentX = 0, currentY = 0;
        points.push_back(Point(currentX, currentY));
        for (const Instruction &instruction : instructions)
        {
            switch (instruction.direction)
            {
            case 'U':
                currentY += instruction.steps;
                break;
            case 'D':
                currentY -= instruction.steps;
                break;
            case 'L':
                currentX -= instruction.steps;
                break;
            case 'R':
                currentX += instruction.steps;
                break;
            default:
                cout << "ERROR: invalid instruction direction: " << instruction.direction << endl;
            }
            points.push_back(Point(currentX, currentY));
        }
    }
};
vector<Instruction> readInstructions()
{
    int steps;
    char direction;
    string color;
    vector<Instruction> instructions;
    while (fin >> direction >> steps >> color)
    {
        color = color.substr(2, 6);
        stringstream trueInput("0x" + color.substr(0, 5) + " " + color.substr(5, 1));
        int directionCode;
        trueInput >> hex >> steps >> dec >> directionCode;
        switch (directionCode)
        {
        case 0:
            direction = 'R';
            break;
        case 1:
            direction = 'D';
            break;
        case 2:
            direction = 'L';
            break;
        case 3:
            direction = 'U';
            break;
        default:
            cout << "ERROR: invalid direction code: " << directionCode;
        }
        instructions.push_back(Instruction(steps, direction, color));
    }
    return instructions;
}
int main()
{
    vector<Instruction> instructions = readInstructions();
    Polygon polygon(instructions);
    fout << polygon.area() + polygon.perimeter() / 2 + 1;
}