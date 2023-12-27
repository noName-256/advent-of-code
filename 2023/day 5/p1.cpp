#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <regex>

using namespace std;
ifstream fin("p1.in");
ofstream fout("p1.out");

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
    Converter(string from = "", string to = "", vector<MapModule> converterMap = vector<MapModule>())
    {
        fromWhat = from;
        toWhat = to;
        map = converterMap;
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
vector<long long> getSeeds()
{
    string inputRow;
    vector<long long> seeds;
    while (getline(fin, inputRow))
    {
        if (inputRow.find("seeds: ") != string::npos)
        {
            // found seeds row
            regex digitRegex("[0-9]+");
            auto matches_begin = sregex_iterator(inputRow.begin(), inputRow.end(), digitRegex);
            auto matches_end = sregex_iterator();
            for (auto i = matches_begin; i != matches_end; i++)
            {
                smatch match = *i;
                seeds.push_back(stoll(match.str()));
            }
            return seeds;
        }
    }
    return seeds;
}
long long seedLocationMinimum(vector<long long> seeds, GeneralConverter gc)
{
    long long minimum = seeds.at(0);
    for (const long long &seed : seeds)
        minimum = min(minimum, gc.convertSeedToLocation(seed));
    return minimum;
}
int main()
{
    vector<long long> seeds = getSeeds();
    GeneralConverter gc = getGeneralConverter();
    fout << seedLocationMinimum(seeds, gc);
}