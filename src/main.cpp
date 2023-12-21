#include <algorithm>
#include <iostream>
#include <limits>
#include <string>
#include <vector>
#include "Utils/FileIO.hpp"
#include "Utils/StringHandling.hpp"

struct Position
{
    size_t x;
    size_t y;
};

typedef std::vector<std::string> Garden;
typedef std::vector<Position> Positions;
typedef std::vector<std::vector<bool>> Visited;


Position FindStartingPosition(const Garden& garden)
{
    for(size_t rowIndex = 0u; rowIndex != garden.size(); ++rowIndex)
    {
        const std::string& row = garden[rowIndex];
        const auto position = row.find("S");
        if(position != row.npos)
        {
            return { position, rowIndex };
        }
    }

    return {};
}

Visited GetVisitedMap(const Garden& garden)
{
    Visited visited;
    for(const auto& line : garden)
    {
        std::vector<bool> emptyLine(line.size(), false);
        visited.push_back(emptyLine);
    }
    return visited;
}

size_t g_numberOfRequiredSteps = 64;

int GetPossibleEndPossitionsFromPosition(const Position& position, const int iteration, const Garden& garden, Visited& visited)
{
    const size_t x = position.x;
    const size_t y = position.y;
    int result = 0;
    visited[y][x] = true;
    if(iteration != g_numberOfRequiredSteps)
    {
        if(y > 0 && garden[y-1][x] != '#') // north
        {
            Position nextPosition {x, y-1};
            if(visited[nextPosition.y][nextPosition.x] == false)
            {
                result += GetPossibleEndPossitionsFromPosition(nextPosition, iteration + 1, garden, visited);
            }
        }
        if(y < (garden.size()-1) && garden[y+1][x] != '#') // south
        {
            Position nextPosition { x, y+1 };
            if(visited[nextPosition.y][nextPosition.x] == false)
            {
                result += GetPossibleEndPossitionsFromPosition(nextPosition, iteration + 1, garden, visited);
            }
        }
        if(x > 0 && garden[y][x-1] != '#') // east
        {
            Position nextPosition { x-1, y };
            if(visited[nextPosition.y][nextPosition.x] == false)
            {
                result += GetPossibleEndPossitionsFromPosition(nextPosition, iteration + 1, garden, visited);
            }
        }
        if((y < garden[y].length()-1) && garden[y][x+1] != '#')
        {
            Position nextPosition { x+1, y };
            if(visited[nextPosition.y][nextPosition.x] == false)
            {
                result += GetPossibleEndPossitionsFromPosition(nextPosition, iteration + 1, garden, visited);
            }
        }
    }
    else
    {
        return 1;
    }

    return result;
}

void FirstPart()
{
    std::cout << "= First part =\n";
    Utils::FileIo fileIo("C:/projects/AdventOfCode/src/input.txt");
    const std::vector<std::string> filePerLine = fileIo.GetFileContent();
    Visited foundEndPositions = GetVisitedMap(filePerLine);

    const Position startingPosistion = FindStartingPosition(filePerLine);
    const int result = GetPossibleEndPossitionsFromPosition(startingPosistion, 0u, filePerLine, foundEndPositions);

    std::cout << "Garden plots reachable: " << result << "\n";
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