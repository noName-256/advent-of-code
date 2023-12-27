#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <vector>
#include <math.h>

using namespace std;
ifstream fin("p2.in");
ofstream fout("p2.out");
struct Race
{
    long long time, distance;
    Race(long long t = 0, long long d = 0)
    {
        time = t;
        distance = d;
    }
};
long long windUpToDistance(Race race, long long windUp)
{
    long long timeMoving = race.time - windUp;
    return windUp * timeMoving;
}
long long raceMinBinarySerach(Race race, long long start, long long end)
{
    long long middle = (start + end) / 2;
    if (start > end)
        return -1;
    if (windUpToDistance(race, middle) > race.distance)
    {
        // middle is good, therefore we check the lower half
        long long lowerHalf = raceMinBinarySerach(race, start, middle - 1);
        // if there actually is something good in the lower half return that
        if (lowerHalf != -1)
            return lowerHalf;
        return middle;
    }
    else
    {
        // middle is not good, return top half
        long long topHalf = raceMinBinarySerach(race, middle + 1, end);
        return topHalf;
    }
}
long long minWindUpToWin(Race race)
{
    return raceMinBinarySerach(race, 0, race.time / 2);
}
long long waysToWin(Race race)
{
    long long minWindUp = minWindUpToWin(race), maxWindUp = race.time - minWindUp;
    return maxWindUp - minWindUp + 1;
}
long long countDigits(long long n)
{
    long long nr = 0;
    while (n)
    {
        nr++;
        n /= 10;
    }
    return nr;
}
vector<Race> getRaces()
{
    string times, distances;
    getline(fin, times);
    getline(fin, distances);

    vector<Race> races;
    regex digitRegex("[0-9]+");
    auto times_matches_begin = sregex_iterator(times.begin(), times.end(), digitRegex);
    auto times_matches_end = sregex_iterator();
    auto distances_matches_begin = sregex_iterator(distances.begin(), distances.end(), digitRegex);
    auto distances_matches_end = sregex_iterator();

    auto j = distances_matches_begin;
    for (auto i = times_matches_begin; i != times_matches_end; i++)
    {
        long long time = stoi(i->str());
        long long distance = stoi(j->str());
        Race newRace(time, distance);
        races.push_back(newRace);
        j++;
    }
    return races;
}
Race getActualRace()
{
    vector<Race> races = getRaces();
    Race actualRace(0, 0);
    for (Race &race : races)
    {
        actualRace.time = actualRace.time * pow(10, countDigits(race.time)) + race.time;
        actualRace.distance = actualRace.distance * pow(10, countDigits(race.distance)) + race.distance;
    }
    return actualRace;
}
int main()
{
    Race race = getActualRace();
    long long marginOfError = waysToWin(race);
    fout << marginOfError;
}