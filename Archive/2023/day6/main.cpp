#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "Utils/FileIO.hpp"
#include "Utils/StringHandling.hpp"

typedef unsigned long long NumberBase;
struct Race
{
    Race(const NumberBase time, const NumberBase distance)
    : mTime(time)
    , mDistanceRecord(distance)
    {}

    const NumberBase mTime;
    const NumberBase mDistanceRecord;
    NumberBase mNumberOfRecordBreakingRaceStrategies = 0;
};

std::vector<NumberBase> GetNumbersFromString(const std::string& input, const std::string& key)
{
    std::vector<NumberBase> values;

    const std::string times = input.substr(std::string(key).length());
    std::vector<std::string> splittedString;
    StringHandling::SplitStringOnCharacter(times, splittedString, ' ');
    for(const auto& character : splittedString)
    {
        if(character != "")
        {
            values.push_back(std::stoull(character));
        }
    }

    return values;
}

NumberBase GetNumberFromLine(const std::string& line, const std::string& key)
{
    const std::string times = line.substr(std::string(key).length());
    std::vector<std::string> splittedString;
    StringHandling::SplitStringOnCharacter(times, splittedString, ' ');

    std::stringstream stream;
    for(const auto& character : splittedString)
    {
        stream << character;
    }

    return std::stoull(stream.str());
}

std::vector<Race> GetRacesFromTimeAndDistances(const std::vector<NumberBase>& times, const std::vector<NumberBase>& distances)
{
    std::vector<Race> races;
    for(size_t index = 0u; index != times.size(); ++index)
    {
        races.push_back({times[index], distances[index]});
    }

    return races;
}

Race GetRaceFromTimeAndDistance(const NumberBase time, const NumberBase distance)
{
    return {time, distance};
}

void SetNumberOfRecordBeatingWins(Race& race)
{
    std::vector<NumberBase> raceBeatingPressTimes;
    for(size_t pressTime = 0u; pressTime != race.mTime; ++pressTime)
    {
        const NumberBase distanceTraveled = (race.mTime - pressTime) * pressTime;
        if(distanceTraveled > race.mDistanceRecord)
        {
            raceBeatingPressTimes.push_back(pressTime);
        }
    }

    race.mNumberOfRecordBreakingRaceStrategies = raceBeatingPressTimes.size();
}

void FirstPart()
{
    std::cout << "= First part =\n";
    Utils::FileIo fileIo("C:/projects/AdventOfCode/src/input.txt");
    const std::vector<std::string> filePerLine = fileIo.GetFileContent();

    std::vector<NumberBase> times = GetNumbersFromString(filePerLine[0], "Time:");
    std::vector<NumberBase> distances = GetNumbersFromString(filePerLine[1], "Distance:");

    std::vector<Race> races = GetRacesFromTimeAndDistances(times, distances);

    NumberBase result = 1;
    for(auto& race : races)
    {
        SetNumberOfRecordBeatingWins(race);
        result *= race.mNumberOfRecordBreakingRaceStrategies;
    }

    std::cout << "Result: " << result << "\n";
}

void SecondPart()
{
    std::cout << "= Second part =\n";
    Utils::FileIo fileIo("C:/projects/AdventOfCode/src/input.txt");
    const std::vector<std::string> filePerLine = fileIo.GetFileContent();

    NumberBase time = GetNumberFromLine(filePerLine[0], "Time:");
    NumberBase distance = GetNumberFromLine(filePerLine[1], "Distance:");

    Race race = GetRaceFromTimeAndDistance(time, distance);
    SetNumberOfRecordBeatingWins(race);

    std::cout << "Result: " << race.mNumberOfRecordBreakingRaceStrategies << "\n";
}

int main()
{
    std::cout << "Advent of code main\n";

    // FirstPart();
    SecondPart();

    return 0;
}