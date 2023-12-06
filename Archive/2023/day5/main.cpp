#include <algorithm>
#include <array>
#include <iostream>
#include <limits>
#include <map>
#include <string>
#include <vector>

#include "Utils/FileIO.hpp"
#include "Utils/StringHandling.hpp"

typedef unsigned long long NumberBase;

struct Mapping
{
    const NumberBase mDestination;
    const NumberBase mSource;
    const NumberBase mRange;
    const NumberBase mMax;
    const long long mDiff;

    Mapping(NumberBase destination, NumberBase source, NumberBase range)
    : mDestination(destination)
    , mSource(source)
    , mRange(range)
    , mMax(source + (range-1))
    , mDiff(destination - source)
    {
    }

    bool IsInRange(const NumberBase& source) const
    {
        return source >= mSource && source <= mMax;
    }

    NumberBase GetDestination(const NumberBase& source) const
    {
        return source + mDiff;
    }

    NumberBase GetUpperBound() const
    {
        return mMax;
    }

    NumberBase GetLowerBound() const
    {
        return mSource;
    }
};


typedef std::vector<NumberBase> Seeds;
typedef std::vector<Mapping> Map;
typedef std::array<Map*, 7> Mappings;

struct MapCollection
{
    MapCollection(Map map)
    : mMappings(map)
    // , mLowerBound()
    // , mUpperBound()
    {
    }

    void Stuff()
    {
        NumberBase highestUpper = 0;
        NumberBase lowestLower = std::numeric_limits<NumberBase>::max();

        for(const Mapping& map : mMappings)
        {
            const auto upper = map.GetUpperBound();
            if(upper > highestUpper)
            {
                highestUpper = upper;
            }

            const auto lower = map.GetLowerBound();
            if(lower < lowestLower)
            {
                lowestLower = lower;
            }
        }
    }

    Map mMappings;
};

const std::string seedsKey = "seeds: ";
const std::string seedToSoilKey = "seed-to-soil map:";
const std::string soilToFertilizerKey = "soil-to-fertilizer map:";
const std::string fertilzerToWaterKey = "fertilizer-to-water map:";
const std::string waterToLightKey = "water-to-light map:";
const std::string lightToTemperatureKey = "light-to-temperature map:";
const std::string temperatureToHumidityKey = "temperature-to-humidity map:";
const std::string humidityToLocationKey = "humidity-to-location map:";

Seeds GetSeedsFromInput(const std::vector<std::string>& input)
{
    Seeds seeds;
    for(const std::string& line : input)
    {
        if(line.find(seedsKey) != line.npos)
        {
            const std::string seedNumbers = line.substr(seedsKey.size(), line.size() - seedsKey.size());
            std::cout << seedNumbers << "\n";
            std::vector<std::string> seedsStrings;
            StringHandling::SplitStringOnCharacter(seedNumbers, seedsStrings, ' ');
            for(const auto& seed : seedsStrings)
            {
                seeds.push_back(static_cast<Seeds::value_type>(std::stoull(seed)));
            }
        }
    }

    return seeds;
}

void AddSeedsInRange(Seeds& seeds, const NumberBase& start, const NumberBase& range)
{
    for (NumberBase seedNumber = start; seedNumber != (start + (range - 1)); ++seedNumber)
    {
        seeds.push_back(seedNumber);
    }
}

Seeds GetSeedRangesFromInput(const std::vector<std::string>& input)
{
    Seeds seeds;
    for(const std::string& line : input)
    {
        if(line.find(seedsKey) != line.npos)
        {
            const std::string seedNumbers = line.substr(seedsKey.size(), line.size() - seedsKey.size());
            std::vector<std::string> seedsStrings;
            StringHandling::SplitStringOnCharacter(seedNumbers, seedsStrings, ' ');

            for(auto iterator = seedsStrings.begin(); iterator != seedsStrings.end(); iterator+=2)
            {
                AddSeedsInRange(seeds, std::stoull(*iterator), std::stoull(*(iterator+1)));
                AddSeedsInRange(seeds, std::stoull(*iterator), std::stoull(*(iterator+1)));

            }
        }
    }

    return seeds;
}

void AddRangeToMap(Map& map, const std::string& input)
{
    std::vector<std::string> splittedInputLine;
    StringHandling::SplitStringOnCharacter(input, splittedInputLine, ' ');
    NumberBase destination = std::stoull(splittedInputLine[0]);
    NumberBase source = std::stoull(splittedInputLine[1]);
    const NumberBase range = std::stoull(splittedInputLine[2]);

    map.push_back({destination, source, range});
}

Map GetMappingFromKey(const std::vector<std::string>& input, const std::string key)
{
    Map map;
    for(auto inputIterator = input.begin(); inputIterator != input.end(); ++inputIterator)
    {
        const auto& line = *inputIterator;
        if(line.find(key) != line.npos)
        {
            ++inputIterator;
            while (inputIterator != input.end() && !(*inputIterator).empty())
            {
                AddRangeToMap(map, *inputIterator);
                ++inputIterator;
            }
            return map;
        }
    }

    return map;
}

NumberBase GetMappedValueFromSource(const Map& map, const NumberBase& source)
{
    for(const auto& mapping : map)
    {
        if(mapping.IsInRange(source))
        {
            return mapping.GetDestination(source);
        }
    }

    return source; // source value is not in map, return source as destiny.
}

NumberBase GetLocation(const Mappings& mappings, const NumberBase& source)
{
    NumberBase result = source;
    for(const Map* map : mappings)
    {
        result = GetMappedValueFromSource(*map, result);
    }

    return result;
}

void FirstPart()
{
    std::cout << "= First part =\n";
    Utils::FileIo fileIo("C:/projects/AdventOfCode/src/input.txt");
    const std::vector<std::string> filePerLine = fileIo.GetFileContent();

    Seeds seeds = GetSeedsFromInput(filePerLine);
    Map seedToSoilMap = GetMappingFromKey(filePerLine, seedToSoilKey);
    Map soilToFertilizerMap = GetMappingFromKey(filePerLine, soilToFertilizerKey);
    Map fertilzerToWaterMap = GetMappingFromKey(filePerLine, fertilzerToWaterKey);
    Map waterToLightMap = GetMappingFromKey(filePerLine, waterToLightKey);
    Map lightToTemperatureMap = GetMappingFromKey(filePerLine, lightToTemperatureKey);
    Map temperatureToHumidityMap = GetMappingFromKey(filePerLine, temperatureToHumidityKey);
    Map humidityToLocationMap = GetMappingFromKey(filePerLine, humidityToLocationKey);

    const Mappings maps { &seedToSoilMap, &soilToFertilizerMap, &fertilzerToWaterMap, &waterToLightMap, &lightToTemperatureMap, &temperatureToHumidityMap, &humidityToLocationMap };
    
    std::vector<NumberBase> results;
    for(const auto& seed : seeds)
    {
        results.push_back(GetLocation(maps, seed));
    }

    std::cout << "Lowest location number: " << *std::min_element(results.begin(), results.end()) << "\n";
}

void SecondPart()
{
    std::cout << "= Second part =\n";
    Utils::FileIo fileIo("C:/projects/AdventOfCode/src/input.txt");
    const std::vector<std::string> filePerLine = fileIo.GetFileContent();

    Seeds seeds = GetSeedRangesFromInput(filePerLine);
    Map seedToSoilMap = GetMappingFromKey(filePerLine, seedToSoilKey);
    Map soilToFertilizerMap = GetMappingFromKey(filePerLine, soilToFertilizerKey);
    Map fertilzerToWaterMap = GetMappingFromKey(filePerLine, fertilzerToWaterKey);
    Map waterToLightMap = GetMappingFromKey(filePerLine, waterToLightKey);
    Map lightToTemperatureMap = GetMappingFromKey(filePerLine, lightToTemperatureKey);
    Map temperatureToHumidityMap = GetMappingFromKey(filePerLine, temperatureToHumidityKey);
    Map humidityToLocationMap = GetMappingFromKey(filePerLine, humidityToLocationKey);

    const Mappings maps { &seedToSoilMap, &soilToFertilizerMap, &fertilzerToWaterMap, &waterToLightMap, &lightToTemperatureMap, &temperatureToHumidityMap, &humidityToLocationMap };
    
    std::vector<NumberBase> results;
    for(const auto& seed : seeds)
    {
        results.push_back(GetLocation(maps, seed));
    }

    std::cout << "Lowest location number: " << *std::min_element(results.begin(), results.end()) << "\n";
}

int main()
{
    std::cout << "Advent of code main\n";

    // FirstPart();
    SecondPart();

    return 0;
}