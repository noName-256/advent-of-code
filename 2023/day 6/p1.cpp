#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <vector>

using namespace std;
ifstream fin("p1.in");
ofstream fout("p1.out");
struct Race
{
    int time, distance;
    Race(int t = 0, int d = 0)
    {
        time = t;
        distance = d;
    }
};
int minWindUpToWin(Race race)
{
    for (int i = 0; i <= race.time; i++)
    {
        int windUp = i, timeMoving = race.time - i;
        if (windUp * timeMoving > race.distance)
            return windUp;
    }
    cout << "ERROR: this should not be reached";
    return -1;
}
int waysToWin(Race race)
{
    int minWindUp = minWindUpToWin(race), maxWindUp = race.time - minWindUp;
    return maxWindUp - minWindUp + 1;
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
        int time = stoi(i->str());
        int distance = stoi(j->str());
        Race newRace(time, distance);
        races.push_back(newRace);
        j++;
    }
    return races;
}
int main()
{
    vector<Race> races = getRaces();
    int marginOfError = 1;
    for (Race &race : races)
        marginOfError *= waysToWin(race);
    fout << marginOfError;
}