#include <iostream>
#include <string>
#include <vector>

#include "Utils/FileIO.hpp"
#include "Utils/StringHandling.hpp"

int GetGameIdFromString(const std::string& game)
{
    const auto gameIdStartPosition = game.find("Game ") + 5;
    const auto gameIdStopPosistion = game.find(": ");

    return std::stoi(game.substr(gameIdStartPosition, gameIdStopPosistion - gameIdStartPosition));
}

bool IsSetPossible(const std::string set)
{
    std::vector<std::string> colors;
    StringHandling::SplitStringOnCharacter(set, colors, ',');

    for(const std::string& color : colors)
    {
        const auto bluePosition = color.find(" blue");
        if(bluePosition != color.npos)
        {
            const int amountOfBlue = std::stoi(color.substr(0, bluePosition));
            if(amountOfBlue > 14)
            { 
                return false;
            }
        }

        const auto redPosition = color.find(" red");
        if(redPosition != color.npos)
        {
            const int amountOfRed = std::stoi(color.substr(0, redPosition));
            if(amountOfRed > 12)
            {
                return false;
            }
        }

        const auto greenPosition = color.find(" green");
        if(greenPosition != color.npos)
        {
            const int amountOfGreen = std::stoi(color.substr(0, greenPosition));
            if(amountOfGreen > 13)
            {
                return false;
            }
        }
    }

    return true;

    // 12 red cubes, 13 green cubes, and 14 blue cubes
}

bool IsGamePossible(const std::string& game)
{
    const std::string sets = game.substr(game.find(": ") + 2);
    std::vector<std::string> gameSets;
    StringHandling::SplitStringOnCharacter(sets, gameSets, ';');

    for(const auto& set : gameSets)
    {
        if(!IsSetPossible(set))
        {
            return false;
        }
    }

    return true;
}

int GetMinimumAmountOfCubesRequired(const std::string& game)
{
    int minimumAmount = 0;

    const std::string sets = game.substr(game.find(": ") + 2);
    std::vector<std::string> gameSets;
    StringHandling::SplitStringOnCharacter(sets, gameSets, ';');

    int minRed = 0;
    int minGreen = 0;
    int minBlue = 0;

    for(const auto& set : gameSets)
    {        
        std::vector<std::string> colors;
        StringHandling::SplitStringOnCharacter(set, colors, ',');

        for(const std::string& color : colors)
        {
            const auto bluePosition = color.find(" blue");
            if(bluePosition != color.npos)
            {
                const int amountOfBlue = std::stoi(color.substr(0, bluePosition));
                if(amountOfBlue > minBlue)
                {
                    minBlue = amountOfBlue;
                }
            }

            const auto redPosition = color.find(" red");
            if(redPosition != color.npos)
            {
                const int amountOfRed = std::stoi(color.substr(0, redPosition));
                if(amountOfRed > minRed)
                {
                    minRed = amountOfRed;
                }
            }

            const auto greenPosition = color.find(" green");
            if(greenPosition != color.npos)
            {
                const int amountOfGreen = std::stoi(color.substr(0, greenPosition));
                if(amountOfGreen > minGreen)
                {
                    minGreen = amountOfGreen;
                }
            }
        }
    }

    minimumAmount = minRed * minGreen * minBlue;

    return minimumAmount;
}

void FirstPart()
{
    std::cout << "= First part =\n";
    Utils::FileIo fileIo("../input.txt");
    const std::vector<std::string> filePerLine = fileIo.GetFileContent();

    int sumOfPossibleGames = 0;
    for(const auto& line : filePerLine)
    {
        const int gameId = GetGameIdFromString(line);

        if(IsGamePossible(line))
        {
            sumOfPossibleGames += gameId;
        }
    }

    std::cout << "Sum of possible games: " << sumOfPossibleGames << "\n";
}

void SecondPart()
{
    std::cout << "= Second part =\n";
    Utils::FileIo fileIo("../input.txt");
    const std::vector<std::string> filePerLine = fileIo.GetFileContent();

    int sumOfMinimum = 0;
    for(const auto& line : filePerLine)
    {
        const int gameId = GetGameIdFromString(line);
        const int minimum = GetMinimumAmountOfCubesRequired(line);

        sumOfMinimum += minimum;
        std::cout << gameId << ": " << minimum << "\n";
    }

    std::cout << "Sum of minimum: " << sumOfMinimum << "\n";
}

int main()
{
    std::cout << "Advent of code main\n";

    // FirstPart();
    SecondPart();

    return 0;
}