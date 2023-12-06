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

struct Seed
{
    Seed(const NumberBase& start, const NumberBase& range)
    : mStart(start)
    , mRange(range)
    {}

    NumberBase mStart;
    NumberBase mRange;
};

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
};

typedef std::vector<Seed> Seeds;
typedef std::vector<Mapping> Map;
typedef std::array<Map*, 7> Mappings;

const std::string seedsKey = "seeds: ";
const std::string seedToSoilKey = "seed-to-soil map:";
const std::string soilToFertilizerKey = "soil-to-fertilizer map:";
const std::string fertilzerToWaterKey = "fertilizer-to-water map:";
const std::string waterToLightKey = "water-to-light map:";
const std::string lightToTemperatureKey = "light-to-temperature map:";
const std::string temperatureToHumidityKey = "temperature-to-humidity map:";
const std::string humidityToLocationKey = "humidity-to-location map:";

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
                seeds.push_back({std::stoull(*iterator), std::stoull(*(iterator+1))});
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
    
    NumberBase lowestLocation = std::numeric_limits<NumberBase>::max();
    NumberBase lowestSeedNumber = 0;
    for(const auto& seed : seeds)
    {
        for(NumberBase currentSeedNumber = seed.mStart; currentSeedNumber < seed.mStart + (seed.mRange -1); currentSeedNumber+=1000)
        {
            const auto result = GetLocation(maps, currentSeedNumber);
            if(result < lowestLocation)
            {
                lowestLocation = result;
                lowestSeedNumber = currentSeedNumber;
                std::cout << ".";
            }
        }
    }

    std::cout << std::endl;

    std::cout << "Lowest 1000ths location number: " << lowestLocation << " from seed: " << lowestSeedNumber << "\n";

    for(NumberBase currentSeedNumber = lowestSeedNumber - 5000; currentSeedNumber < lowestSeedNumber + 5000; ++currentSeedNumber)
    {
        const auto result = GetLocation(maps, currentSeedNumber);
        if(result < lowestLocation)
        {
            lowestLocation = result;
            lowestSeedNumber = currentSeedNumber;
        }

        if(currentSeedNumber % 100 == 0)
        {
            std::cout << currentSeedNumber << "\n";
        }
    }
    std::cout << std::endl;

    std::cout << "Lowest location number: " << lowestLocation << " from seed: " << lowestSeedNumber << "\n";
}

int main()
{
    std::cout << "Advent of code main\n";

    // FirstPart();
    SecondPart();

    return 0;
}