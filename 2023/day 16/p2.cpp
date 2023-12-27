#include <iostream>
#include <fstream>
#include <string>
#include <set>

using namespace std;
ifstream fin("p2.in");
ofstream fout("p2.out");
void outputTiles(bool tiles[200][200], int rows, int cols, ostream &out = fout)
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
            out << (tiles[i][j] ? '#' : '.');
        out << endl;
    }
    out << endl;
}
struct Beam
{
    char direction; // N / W / S / E
    int row;
    int col;
    Beam(int r, int c, char d)
    {
        row = r;
        col = c;
        direction = d;
    }
};
struct BeamCmp
{
    bool operator()(const Beam &b1, const Beam &b2) const
    {
        if (b1.row != b2.row)
            return b1.row < b2.row;
        if (b1.col != b2.col)
            return b1.col < b2.col;
        if (b1.direction != b2.direction)
            return b1.direction < b2.direction;
        return false;
    }
};
struct Go
{
    int rows, cols;
    set<Beam, BeamCmp> north(const Beam &b)
    {
        set<Beam, BeamCmp> s;
        if (b.row - 1 >= 0)
            s.insert(Beam(b.row - 1, b.col, 'N'));
        return s;
    }
    set<Beam, BeamCmp> west(const Beam &b)
    {
        set<Beam, BeamCmp> s;
        if (b.col - 1 >= 0)
            s.insert(Beam(b.row, b.col - 1, 'W'));
        return s;
    }
    set<Beam, BeamCmp> south(const Beam &b)
    {
        set<Beam, BeamCmp> s;
        if (b.row + 1 < rows)
            s.insert(Beam(b.row + 1, b.col, 'S'));
        return s;
    }
    set<Beam, BeamCmp> east(const Beam &b)
    {
        set<Beam, BeamCmp> s;
        if (b.col + 1 < cols)
            s.insert(Beam(b.row, b.col + 1, 'E'));
        return s;
    }
    set<Beam, BeamCmp> northSouth(const Beam &b)
    {
        set<Beam, BeamCmp> s;
        set<Beam, BeamCmp> nor = north(b);
        set<Beam, BeamCmp> sou = south(b);
        s.insert(nor.begin(), nor.end());
        s.insert(sou.begin(), sou.end());
        return s;
    }
    set<Beam, BeamCmp> westEast(const Beam &b)
    {
        set<Beam, BeamCmp> s;
        set<Beam, BeamCmp> w = west(b);
        set<Beam, BeamCmp> e = east(b);
        s.insert(w.begin(), w.end());
        s.insert(e.begin(), e.end());
        return s;
    }
    Go(int r, int c)
    {
        rows = r;
        cols = c;
    }
};
struct Beams
{
    set<Beam, BeamCmp> beams;
    set<Beam, BeamCmp> passedBeams;
    char tiles[200][200];
    bool energizedTiles[200][200];
    int rows, cols;
    Go go = Go(0, 0);
    set<Beam, BeamCmp> nextForBeam(const Beam &b)
    {
        switch (b.direction)
        {
        case 'N':
            switch (tiles[b.row][b.col])
            {
            case '/':
                return go.east(b);
            case '\\':
                return go.west(b);
            case '-':
                return go.westEast(b);
            case '|':
            case '.':
                return go.north(b);
            default:
                cout << "ERROR: invalid tile: " << tiles[b.row][b.col] << endl;
            }
        case 'W':
            switch (tiles[b.row][b.col])
            {
            case '/':
                return go.south(b);
            case '\\':
                return go.north(b);
            case '-':
            case '.':
                return go.west(b);
            case '|':
                return go.northSouth(b);
            default:
                cout << "ERROR: invalid tile: " << tiles[b.row][b.col] << endl;
            }
        case 'S':
            switch (tiles[b.row][b.col])
            {
            case '/':
                return go.west(b);
            case '\\':
                return go.east(b);
            case '-':
                return go.westEast(b);
            case '|':
            case '.':
                return go.south(b);
            default:
                cout << "ERROR: invalid tile: " << tiles[b.row][b.col] << endl;
            }
        case 'E':
            switch (tiles[b.row][b.col])
            {
            case '/':
                return go.north(b);
            case '\\':
                return go.south(b);
            case '-':
            case '.':
                return go.east(b);
            case '|':
                return go.northSouth(b);
            default:
                cout << "ERROR: invalid tile: " << tiles[b.row][b.col] << endl;
            }
        default:
            cout << "ERROR: invalid direction: " << b.direction << endl;
        }
    }
    void removePassedBeams()
    {
        set<Beam, BeamCmp> beamsCopy = beams;
        for (const Beam &beam : beamsCopy)
            if (passedBeams.find(beam) != passedBeams.end())
            {
                beams.erase(beam);
            }
    }
    bool moveBeams()
    {
        removePassedBeams();
        if (beams.empty())
            return false;
        for (const Beam &b : beams)
            energizedTiles[b.row][b.col] = true;
        set<Beam, BeamCmp> newBeams;
        for (const Beam &b : beams)
        {
            set<Beam, BeamCmp> nextBeams = nextForBeam(b);
            newBeams.insert(nextBeams.begin(), nextBeams.end());
        }
        passedBeams.insert(beams.begin(), beams.end());
        beams = newBeams;
        return true;
    }
    void moveBeamsUntilSteady()
    {
        while (moveBeams())
        {
        }
    }
    int energizedTilesCount()
    {
        int nr = 0;
        for (int i = 0; i < rows; i++)
            for (int j = 0; j < cols; j++)
                if (energizedTiles[i][j])
                    nr++;
        return nr;
    }
    Beams(char t[200][200], int r, int c, Beam startingBeam)
    {
        rows = r;
        cols = c;
        for (int i = 0; i < rows; i++)
            for (int j = 0; j < cols; j++)
            {
                tiles[i][j] = t[i][j];
                energizedTiles[i][j] = false;
            }
        go = Go(rows, cols);
        beams.insert(startingBeam);
    }
};
void readTiles(char tiles[200][200], int &rows, int &cols)
{
    string inputRow;
    rows = 0;
    cols = 0;
    while (getline(fin, inputRow))
    {
        cols = 0;
        for (const char &c : inputRow)
        {
            tiles[rows][cols] = c;
            cols++;
        }
        rows++;
    }
}
int main()
{
    char tiles[200][200];
    int rows, cols;
    readTiles(tiles, rows, cols);
    int maxEnergy = 0;
    for (int i = 0; i < rows; i++)
    {
        Beams beamsLeft(tiles, rows, cols, Beam(i, 0, 'E'));
        beamsLeft.moveBeamsUntilSteady();
        maxEnergy = max(maxEnergy, beamsLeft.energizedTilesCount());

        Beams beamsRight(tiles, rows, cols, Beam(i, cols - 1, 'W'));
        beamsRight.moveBeamsUntilSteady();
        maxEnergy = max(maxEnergy, beamsRight.energizedTilesCount());
    }

    for (int j = 0; j < cols; j++)
    {
        Beams beamsUp(tiles, rows, cols, Beam(0, j, 'S'));
        beamsUp.moveBeamsUntilSteady();
        maxEnergy = max(maxEnergy, beamsUp.energizedTilesCount());

        Beams beamsDown(tiles, rows, cols, Beam(rows - 1, j, 'N'));
        beamsDown.moveBeamsUntilSteady();
        maxEnergy = max(maxEnergy, beamsDown.energizedTilesCount());
    }
    fout << maxEnergy;
}