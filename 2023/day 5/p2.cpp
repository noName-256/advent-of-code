#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <regex>

#define MIN_RANGE 0
#define MAX_RANGE 1000000000000
using namespace std;
ifstream fin("p2.in");
ofstream fout("p2.out");

struct MapModule
{
    long long rangeFrom, rangeTo, conversionCoefficient;
    bool valueInRange(long long value)
    {
        return value >= rangeFrom && value <= rangeTo;
    }
    long long convertValue(long long value)
    {
        return value + conversionCoefficient;
    }
    MapModule(long long from, long long to, long long coefficient)
    {
        rangeFrom = from;
        rangeTo = to;
        conversionCoefficient = coefficient;
    }
};
bool compareMapModule(MapModule m1, MapModule m2)
{
    return m1.rangeFrom < m2.rangeFrom;
}
struct Range
{
    long long from, to;
    void display(ostream &out = cout)
    {
        out << "Range from " << from << " to " << to << endl;
    }
    Range(long long fromRange, long long toRange)
    {
        from = fromRange;
        to = toRange;
    }
};
void displayRanges(vector<Range> ranges, ostream &out = cout)
{
    out << "Ranges: \n";
    for (Range &range : ranges)
        range.display();
    out << endl;
}
struct Converter
{
    string fromWhat, toWhat;
    vector<MapModule> map;
    long long convertValue(long long value)
    {
        for (MapModule m : map)
            if (m.valueInRange(value))        // if value in some module
                return m.convertValue(value); // return that value converted in the module
        return value;                         // default to returning the same value
    }
    void sortMap()
    {
        sort(map.begin(), map.end(), compareMapModule);
    }
    void display(ostream &out = cout)
    {
        out << "Converter from " << fromWhat << " to " << toWhat << ": \n";
        for (const MapModule &module : map)
            out << "From " << module.rangeFrom
                << " to " << module.rangeTo << ": "
                << (((module.conversionCoefficient) >= 0)
                        ? "+"
                        : "")
                << module.conversionCoefficient << endl;
        out << endl;
    }
    void addMissingRanges()
    {
        long long i = MIN_RANGE, moduleIndex = 0;
        vector<MapModule> newModules;
        while (i <= MAX_RANGE)
        {
            if (moduleIndex >= map.size())
            {
                // out of modules
                // add one that goes from i to the end(MAX_RANGE)
                MapModule newModule(i, MAX_RANGE, 0);
                newModules.push_back(newModule);
                i = MAX_RANGE + 1;
            }
            else if (i < map.at(moduleIndex).rangeFrom)
            {
                // found a new range needed
                // create a new module that goes from i to the where the next module starts - 1
                MapModule newModule(i, map.at(moduleIndex).rangeFrom - 1, 0);
                newModules.push_back(newModule);
                i = map.at(moduleIndex).rangeTo + 1; // skip to where that module ends + 1
            }
            else if (i > map.at(moduleIndex).rangeTo)
            {
                // modules are behind
                // go to next module
                moduleIndex++;
            }
            else
            {
                // i is within a module
                i = map.at(moduleIndex).rangeTo + 1; // skip to where that module ends + 1
            }
        }
        // add new modules to all modules
        for (const MapModule &newModule : newModules)
            map.push_back(newModule);
        // sort the map
        sortMap();
    }
    vector<Range> convertRange(Range range)
    {
        vector<Range> resultRanges;
        long long rangeStart = range.from, rangeEnd = range.to, moduleIndex = 0;
        while (rangeStart <= rangeEnd)
        {
            if (map.at(moduleIndex).rangeTo < rangeStart)
            {
                // modules are behind
                // go to next module
                moduleIndex++;
            }
            else if (map.at(moduleIndex).rangeFrom > rangeStart)
            {
                // this means that somehow our modules are ahead, this should not happen
                cout << "ERROR: this cout should never be called";
            }
            else
            {
                // something in range is in this module

                // newRange starts from rangeStart(becuase the module can never be ahead as there are no gaps in the modules)
                // newRange ends from either rangeEnd or from where the module ends, depending on which ends first
                Range newRange(rangeStart, min(map.at(moduleIndex).rangeTo, rangeEnd));
                // apply coefficient to the range
                newRange.from = convertValue(newRange.from);
                newRange.to = convertValue(newRange.to);
                resultRanges.push_back(newRange);
                rangeStart = map.at(moduleIndex).rangeTo + 1;
            }
        }
        return resultRanges;
    }
    vector<Range> convertRanges(vector<Range> ranges)
    {
        vector<Range> resultRanges;
        for (Range &range : ranges)
        {
            vector<Range> convertedRanges = convertRange(range);
            // append convertedRanges to resultRanges
            resultRanges.insert(resultRanges.end(),
                                convertedRanges.begin(), convertedRanges.end());
        }
        return resultRanges;
    }
    Converter(string from = "", string to = "", vector<MapModule> converterMap = vector<MapModule>())
    {
        fromWhat = from;
        toWhat = to;
        map = converterMap;
        sortMap();
        addMissingRanges();
    }
};
struct GeneralConverter
{
    // converts from seed(first in the almanac) to location(last in the almanac)
    Converter seedToSoil;
    Converter soilToFertilizer;
    Converter fertilizerToWater;
    Converter waterToLight;
    Converter lightToTemperature;
    Converter temperatureToHumidity;
    Converter humidityToLocation;
    long long convertSeedToLocation(long long seed)
    {
        long long soil = seedToSoil.convertValue(seed);
        long long fertilizer = soilToFertilizer.convertValue(soil);
        long long water = fertilizerToWater.convertValue(fertilizer);
        long long light = waterToLight.convertValue(water);
        long long temperature = lightToTemperature.convertValue(light);
        long long humidity = temperatureToHumidity.convertValue(temperature);
        long long location = humidityToLocation.convertValue(humidity);
        return location;
    }
    vector<Range> convertSeedRangesToLocationRanges(vector<Range> seedRanges)
    {
        vector<Range> soilRanges = seedToSoil.convertRanges(seedRanges);
        vector<Range> fertilizerRanges = soilToFertilizer.convertRanges(soilRanges);
        vector<Range> waterRanges = fertilizerToWater.convertRanges(fertilizerRanges);
        vector<Range> lightRanges = waterToLight.convertRanges(waterRanges);
        vector<Range> temperatureRanges = lightToTemperature.convertRanges(lightRanges);
        vector<Range> humidityRanges = temperatureToHumidity.convertRanges(temperatureRanges);
        vector<Range> locationRanges = humidityToLocation.convertRanges(humidityRanges);
        return locationRanges;
    }
    void display(ostream &out = cout)
    {
        out << "All converters: \n";
        seedToSoil.display(out);
        soilToFertilizer.display(out);
        fertilizerToWater.display(out);
        waterToLight.display(out);
        lightToTemperature.display(out);
        temperatureToHumidity.display(out);
        humidityToLocation.display(out);
        out << endl;
    }
    GeneralConverter(Converter _seedToSoil, Converter _soilToFertilizer,
                     Converter _fertilizerToWater, Converter _waterToLight,
                     Converter _lightToTemperature, Converter _temperatureToHumidity,
                     Converter _humidityToLocation)
    {
        seedToSoil = _seedToSoil;
        soilToFertilizer = _soilToFertilizer;
        fertilizerToWater = _fertilizerToWater;
        waterToLight = _waterToLight;
        lightToTemperature = _lightToTemperature;
        temperatureToHumidity = _temperatureToHumidity;
        humidityToLocation = _humidityToLocation;
    }
};
vector<string> getMap(string mapType)
{
    /*
     mapType: "seed-to-soil" / "soil-to-fertilizer" / "fertilizer-to-water"
     / "water-to-light" / "light-to-temperature" / "temperature-to-humidity"
     / "humidity-to-location"
    */
    string inputRow;
    vector<string> map;
    while (getline(fin, inputRow))
    {
        if (inputRow.find(mapType) != string::npos)
        {
            // found map
            while (getline(fin, inputRow))
            {
                if (inputRow.length() == 0)
                    break;
                map.push_back(inputRow);
            }
            return map;
        }
    }
    return map;
}
Converter getConverter(vector<string> map, string fromWhat, string toWhat)
{
    vector<MapModule> fullMap;
    for (const string mapLine : map)
    {
        stringstream ss(mapLine);
        long long result, source, sourceRange;
        ss >> result >> source >> sourceRange;
        long long from = source, to = source + sourceRange - 1, coefficient = result - source;
        MapModule module(from, to, coefficient);
        fullMap.push_back(module);
    }
    Converter c(fromWhat, toWhat, fullMap);
    return c;
}
GeneralConverter getGeneralConverter()
{
    Converter seedToSoil = getConverter(getMap("seed-to-soil"), "seed", "soil");
    Converter soilToFertilizer = getConverter(getMap("soil-to-fertilizer"), "soil", "fertilizer");
    Converter fertilizerToWater = getConverter(getMap("fertilizer-to-water"), "fertilizer", "water");
    Converter waterToLight = getConverter(getMap("water-to-light"), "water", "light");
    Converter lightToTemperature = getConverter(getMap("light-to-temperature"), "light", "temperature");
    Converter temperatureToHumidity = getConverter(getMap("temperature-to-humidity"), "temperature", "humidity");
    Converter humiditiyToLocation = getConverter(getMap("humidity-to-location"), "humidity", "location");
    GeneralConverter gc(seedToSoil, soilToFertilizer, fertilizerToWater, waterToLight,
                        lightToTemperature, temperatureToHumidity, humiditiyToLocation);
    return gc;
}
vector<Range> getSeedRanges()
{
    string inputRow;
    vector<Range> seedRanges;
    while (getline(fin, inputRow))
    {
        long long rangeFrom = -1, rangeTo = -1, rangeLength = -1;
        if (inputRow.find("seeds: ") != string::npos)
        {
            // found seeds row
            regex digitRegex("[0-9]+");
            auto matches_begin = sregex_iterator(inputRow.begin(), inputRow.end(), digitRegex);
            auto matches_end = sregex_iterator();
            for (auto i = matches_begin; i != matches_end; i++)
            {
                smatch match = *i;
                long long number = stoll(match.str());
                if (rangeFrom == -1)
                    rangeFrom = number;
                else
                {
                    rangeLength = number;
                    rangeTo = rangeFrom + rangeLength - 1;
                    Range newRange(rangeFrom, rangeTo);
                    seedRanges.push_back(newRange);
                    rangeFrom = -1;
                    rangeTo = -1;
                    rangeLength = -1;
                }
            }
            return seedRanges;
        }
    }
    return seedRanges;
}
long long seedLocationMinimum(vector<long long> seeds, GeneralConverter gc)
{
    long long minimum = seeds.at(0);
    for (const long long &seed : seeds)
        minimum = min(minimum, gc.convertSeedToLocation(seed));
    return minimum;
}
long long minimumLocationInRanges(vector<Range> locationRanges)
{
    // we will use from as everything else in the range is bigger than it
    long long minimumLocation = locationRanges.at(0).from;
    for (Range &range : locationRanges)
        minimumLocation = min(minimumLocation, range.from);
    return minimumLocation;
}
int main()
{
    vector<Range> seedRanges = getSeedRanges();
    GeneralConverter gc = getGeneralConverter();
    vector<Range> locationRanges = gc.convertSeedRangesToLocationRanges(seedRanges);
    long long minimumLocation = minimumLocationInRanges(locationRanges);
    fout << minimumLocation;
}