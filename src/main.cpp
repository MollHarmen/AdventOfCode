#include <iostream>
#include <string>
#include <vector>

#include "Utils/FileIO.hpp"
#include "Utils/StringHandling.hpp"

bool IsSpecialCharacter(const char& character)
{
    return (character != '.' && !isdigit(character));
}

int GetDigitStartIndexMinusOne(const int digitStartIndex)
{
    if(digitStartIndex > 0)
    {
        return digitStartIndex - 1;
    }

    return digitStartIndex;
}

int GetDigitStopIndexPlusOne(const int digitStopIndex, const int max)
{
    if(digitStopIndex < (max-1))
    {
        return digitStopIndex + 1;
    }

    return digitStopIndex;
}

bool DoesStringContainSpecialCharacter(const std::string& input)
{
    for(const char& character : input)
    {
        if(IsSpecialCharacter(character))
        {
            return true;
        }
    }

    return false;
}

bool IsAdjacentToSpcialCharacter(const std::vector<std::string>& grid, const size_t lineIndex, const int digitStartIndex, const int digitStopIndex)
{
    // Check line above for special character
    if(lineIndex > 0u)
    {
        const std::string& lineAbove = grid[lineIndex - 1];
        const int start = GetDigitStartIndexMinusOne(digitStartIndex);
        const int stop = GetDigitStopIndexPlusOne(digitStopIndex, lineAbove.length());
        const std::string substring = lineAbove.substr(start, stop - start + 1);

        const bool isValidNumber = DoesStringContainSpecialCharacter(substring);
        if(isValidNumber)
        {
            return true;
        }
    }

    // Check line below for special character
    if(lineIndex < grid.size()-1)
    {
        const std::string& lineBelow = grid[lineIndex + 1];
        const int start = GetDigitStartIndexMinusOne(digitStartIndex);
        const int stop = GetDigitStopIndexPlusOne(digitStopIndex, lineBelow.length());
        const std::string substring = lineBelow.substr(start, stop - start + 1);

        const bool isValidNumber = DoesStringContainSpecialCharacter(substring);
        if(isValidNumber)
        {
            return true;
        }
    }

    // Check left and right column for adjacent special characters
    const int leftColumnIndex = GetDigitStartIndexMinusOne(digitStartIndex);
    const int rightColumnIndex = GetDigitStopIndexPlusOne(digitStopIndex, grid[lineIndex].size());
    bool leftColumnHasSpecialCharacter = IsSpecialCharacter(grid[lineIndex][leftColumnIndex]);
    bool rightColumnHasSpecialCharacter = IsSpecialCharacter(grid[lineIndex][rightColumnIndex]);
    if(lineIndex > 0)
    {
        leftColumnHasSpecialCharacter |= IsSpecialCharacter(grid[lineIndex-1][leftColumnIndex]);
        rightColumnHasSpecialCharacter |= IsSpecialCharacter(grid[lineIndex-1][rightColumnIndex]);
    }

    if(lineIndex < grid.size()-1)
    {
        leftColumnHasSpecialCharacter |= IsSpecialCharacter(grid[lineIndex+1][leftColumnIndex]);
        rightColumnHasSpecialCharacter |= IsSpecialCharacter(grid[lineIndex+1][rightColumnIndex]);
    }

    return leftColumnHasSpecialCharacter | rightColumnHasSpecialCharacter;
}

void FirstPart()
{
    std::cout << "= First part =\n";
    Utils::FileIo fileIo("C:/projects/AdventOfCode/src/input.txt");
    const std::vector<std::string> filePerLine = fileIo.GetFileContent();

    int sumOfParts = 0;
    for(size_t lineIndex = 0u; lineIndex != filePerLine.size(); ++lineIndex)
    {
        const auto& line = filePerLine[lineIndex];

        int digitStartIndex = 0;
        int digitStopIndex = 0;
        bool foundNumber = false;
        for(size_t index = 0; index != line.size(); ++index)
        {
            const char& character = line[index];
            if(isdigit(character))
            {
                if (!foundNumber)
                {
                    digitStartIndex = index;
                    foundNumber = true;
                }
            }
            else
            {
                if(foundNumber)
                {
                    digitStopIndex = index-1;
                    foundNumber = false;

                    if(IsAdjacentToSpcialCharacter(filePerLine, lineIndex, digitStartIndex, digitStopIndex))
                    {
                        sumOfParts += std::stoi(line.substr(digitStartIndex, digitStopIndex - digitStartIndex + 1));
                        std::cout << line.substr(digitStartIndex, digitStopIndex - digitStartIndex + 1) << "\n";
                    }
                }
            }
        }
        if(foundNumber)
        {
            digitStopIndex = line.size() - 1;
            foundNumber = false;

            if(IsAdjacentToSpcialCharacter(filePerLine, lineIndex, digitStartIndex, digitStopIndex))
            {
                sumOfParts += std::stoi(line.substr(digitStartIndex, digitStopIndex - digitStartIndex + 1));
                std::cout << line.substr(digitStartIndex, digitStopIndex - digitStartIndex + 1) << "\n";
            }
        }
    }
    std::cout << "Sum of parts: " << sumOfParts << "\n";
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