#include <array>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "Utils/FileIO.hpp"
#include "Utils/StringHandling.hpp"

typedef unsigned long long NumberBase;

typedef std::vector<NumberBase> Seeds;
typedef std::map<NumberBase, NumberBase> Map;
typedef std::array<Map*, 7> Mappings;

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

void AddRangeToMap(Map& map, const std::string& input)
{
    std::vector<std::string> splittedInputLine;
    StringHandling::SplitStringOnCharacter(input, splittedInputLine, ' ');
    NumberBase destination = std::stoull(splittedInputLine[0]);
    NumberBase source = std::stoull(splittedInputLine[1]);
    const NumberBase range = std::stoull(splittedInputLine[2]);

    const NumberBase expectedMapSize = map.size() + range;
    while(map.size() != expectedMapSize)
    {
        map.insert(std::make_pair(destination, source));
        ++destination;
        ++source;
    }
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
    const auto mapIterator = std::find_if(map.begin(), map.end(), [&source](const auto& item){ return item.second == source;});
    if(mapIterator != map.end())
    {
        return mapIterator->first;
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
    
    for(const auto& seed : seeds)
    {
        std::cout << GetLocation(maps, seed) << "\n";
    }
}

void SecondPart()
{
    std::cout << "= Second part =\n";
    Utils::FileIo fileIo("C:/projects/AdventOfCode/src/input.txt");
    const std::vector<std::string> filePerLine = fileIo.GetFileContent();

}

int main()
{
    std::cout << "Advent of code main\n";

    FirstPart();
    // SecondPart();

    return 0;
}