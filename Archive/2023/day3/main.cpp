#include <algorithm>
#include <array>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "Utils/FileIO.hpp"
#include "Utils/StringHandling.hpp"

bool IsCharacterAGear(const char& character)
{
    return character == '*';
}

void FirstPart()
{
    std::cout << "= First part =\n";
    Utils::FileIo fileIo("C:/projects/AdventOfCode/src/input.txt");
    const std::vector<std::string> filePerLine = fileIo.GetFileContent();
}

int GetGearNumber(const std::string& line, const int characterIndex)
{
    int start = characterIndex;
    int stop = characterIndex;

    while(isdigit(line[start]) && start > 0)
    {
        --start;   
    }

    if (!isdigit(line[start]))
    {
        ++start;
    }

    while(isdigit(line[stop]) && stop < line.size()-1)
    {
        ++stop;
    }

    if (!isdigit(line[stop]))
    {
        --stop;
    }

    return std::stoi(line.substr(start, stop+1));
}

std::vector<int> GetGearNumbering(const std::vector<std::string> grid, const int lineIndex, const int characterIndex)
{
    std::vector<int> gearNumbers;
    bool foundUp = false;
    bool foundDown = false;

    // look up for digit
    if(lineIndex > 0)
    {
        // Up
        if(isdigit(grid[lineIndex-1][characterIndex]))
        {
            gearNumbers.push_back(GetGearNumber(grid[lineIndex-1], characterIndex));
            foundUp = true;
        }
    }

    if(lineIndex > 0 && !foundUp)
    {
        // Left up diagonal
        if(characterIndex > 0)
        {
            if(isdigit(grid[lineIndex-1][characterIndex-1]))
            {
                gearNumbers.push_back(GetGearNumber(grid[lineIndex-1], characterIndex-1));
            }
        }

        // Right up diagonal
        if(characterIndex < (grid[lineIndex-1].size()-1))
        {
            if(isdigit(grid[lineIndex-1][characterIndex+1]))
            {
                gearNumbers.push_back(GetGearNumber(grid[lineIndex-1], characterIndex+1));
            }
        }
    }

    // look down for digit
    if(lineIndex < (grid.size()-1))
    {
        // Down
        if(isdigit(grid[lineIndex+1][characterIndex]))
        {
            gearNumbers.push_back(GetGearNumber(grid[lineIndex+1], characterIndex));
            foundDown = true;
        }
    }
    
    if(lineIndex < (grid.size()-1) && !foundDown)
    {
        // Left down diagonal
        if(characterIndex > 0)
        {
            if(isdigit(grid[lineIndex+1][characterIndex-1]))
            {
                gearNumbers.push_back(GetGearNumber(grid[lineIndex+1], characterIndex-1));
            }
        }

        // Right down diagonal
        if(characterIndex < (grid[lineIndex+1].size()-1))
        {
            if(isdigit(grid[lineIndex+1][characterIndex+1]))
            {
                gearNumbers.push_back(GetGearNumber(grid[lineIndex+1], characterIndex+1));
            }
        }
    }

    // Left
    if(characterIndex > 0)
    {
        if(isdigit(grid[lineIndex][characterIndex-1]))
        {
            gearNumbers.push_back(GetGearNumber(grid[lineIndex], characterIndex-1));
        }
    }

    // Right 
    if(characterIndex < (grid[lineIndex].size()-1))
    {
        if(isdigit(grid[lineIndex][characterIndex+1]))
        {
            gearNumbers.push_back(GetGearNumber(grid[lineIndex], characterIndex));
        }
    }

    return gearNumbers;
}

void SecondPart()
{
    std::cout << "= Second part =\n";
    Utils::FileIo fileIo("C:/projects/AdventOfCode/src/input.txt");
    const std::vector<std::string> filePerLine = fileIo.GetFileContent();

    int sumOfParts = 0;
    for(size_t lineIndex = 0u; lineIndex != filePerLine.size(); ++lineIndex)
    {
        const auto& line = filePerLine[lineIndex];

        for(size_t characterIndex = 0u; characterIndex != line.length(); ++characterIndex)
        {
            if(IsCharacterAGear(line[characterIndex]))
            {
                const std::vector<int> gearRatio = GetGearNumbering(filePerLine, lineIndex, characterIndex);
                if (gearRatio.size() > 1)
                {
                    std::cout << gearRatio[0] << " * " << gearRatio[1] << "\n";
                    sumOfParts += (gearRatio[0] * gearRatio[1]);
                }
            }
        }
    }
    std::cout << "Sum of gear ratios: " << sumOfParts << "\n";
}

int main()
{
    std::cout << "Advent of code main\n";

    // FirstPart();
    SecondPart();

    return 0;
}