#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "Utils/FileIO.hpp"

typedef std::vector<std::vector<char>> Grid;

void DrawHorizontalLine(const int y, const int start, const int end, Grid& grid)
{
    for(int index = start; index != end+1; ++index)
    {
        grid[y][index] = '#';
    }
}

void DrawVerticalLine(const int x, const int start, const int end, Grid& grid)
{
    for(int index = start; index != end+1; ++index)
    {
        grid[index][x] = '#';
    }
}

Grid GetDefaultGrid()
{
    std::vector<char> xAxis;
    xAxis.resize(1000, '.');

    Grid grid(162, xAxis);

    return grid;
}

template<typename Stream>
void Print(const Grid& grid, Stream& stream)
{
    for(const auto& xAxis : grid)
    {
        std::string xAxisString(xAxis.begin(), xAxis.end());
        stream << xAxisString << "\n";
    }
}

Grid GetGridFromFile()
{
    Utils::FileIo fileIo("../src/day14-input.txt");
    const auto filePerLine = fileIo.GetFileContent();

    Grid grid = GetDefaultGrid();

    const std::string arrow = " -> ";

    for(std::string line : filePerLine)
    {
        while(line.find(arrow) != line.npos)
        {
            const int fromX = std::stoi(line.substr(0, line.find(",")));
            const int fromY = std::stoi(line.substr(line.find(",")+1, line.find(arrow)));
            line = line.substr(line.find(arrow)+arrow.length(), line.length());
            const int toX = std::stoi(line.substr(0, line.find(",")));
            const int toY = std::stoi(line.substr(line.find(",")+1, line.find(arrow)));

            if(fromX == toX)
            {
                DrawVerticalLine(fromX, std::min(fromY, toY), std::max(fromY, toY), grid);
            }
            else if(fromY == toY)
            {
                DrawHorizontalLine(fromY, std::min(fromX, toX), std::max(fromX, toX), grid);
            }
        }
    }

    return grid;
}

bool CanFallToTheLeft(const int y, const int x, Grid& grid)
{
    if(x == 0 || y == grid.size())
    {
        return false;
    }

    return grid[y+1][x-1] == '.';
}

bool CanFallToTheRight(const int y, const int x, Grid& grid)
{
    if(x == grid[y].size() || y == grid.size())
    {
        return false;
    }

    return grid[y+1][x+1] == '.';
}

bool DropSandUntilAbyss(Grid& grid)
{
    size_t sandIndex = 500;
    for(int yIndex = 0; yIndex != grid.size()-1; ++yIndex)
    {
        if(grid[yIndex+1][sandIndex] != '.')
        {
            if(CanFallToTheLeft(yIndex, sandIndex, grid))
            {
                --sandIndex;
            }
            else if(CanFallToTheRight(yIndex, sandIndex, grid))
            {
                ++sandIndex;
            }
            else
            {
                grid[yIndex][sandIndex] = 'o';
                return false;
            }
        }
    }
    return true;
}

bool DropSandUntilSource(Grid& grid)
{
    size_t sandIndex = 500;
    for(int yIndex = 0; yIndex != grid.size()-1; ++yIndex)
    {
        if(grid[yIndex+1][sandIndex] != '.')
        {
            if(CanFallToTheLeft(yIndex, sandIndex, grid))
            {
                --sandIndex;
            }
            else if(CanFallToTheRight(yIndex, sandIndex, grid))
            {
                ++sandIndex;
            }
            else
            {
                grid[yIndex][sandIndex] = 'o';
                return (yIndex == 0 && sandIndex == 500);
            }
        }
    }
    return false;
}

void FirstPart()
{
    std::cout << "= First part =\n";
    auto grid = GetGridFromFile();
    
    bool isSandFallingInAbyss = false;
    int numberOfSands = 0;
    while(!isSandFallingInAbyss)
    {
        isSandFallingInAbyss = DropSandUntilAbyss(grid);
        if(!isSandFallingInAbyss)
        {
            ++numberOfSands;
        }
    }
    std::ofstream file("output.txt");
    Print(grid, file);

    std::cout << "\tNumber of sand bags that fell into abyss: " << numberOfSands << "\n";
}

int GetHighestYCoordinateFromGrid(Grid& grid)
{
    int foundHighestCoordinate = 0;
    for(int rowIndex = 0; rowIndex != grid.size(); ++rowIndex)
    {
        const auto& row = grid[rowIndex];
        if(std::find(row.begin(), row.end(), '#') != row.end())
        {
            foundHighestCoordinate = std::max(rowIndex, foundHighestCoordinate);
        }
    }

    return foundHighestCoordinate;
}

void AddFloorToGrid(Grid& grid)
{
    const int highestYCoordinate = GetHighestYCoordinateFromGrid(grid);
    const int floorIndex = highestYCoordinate + 2;
    std::vector<char> floor(grid[0].size(), '#');
    grid[floorIndex] = floor;
}

void SecondPart()
{
    std::cout << "= Second part =\n";
    auto grid = GetGridFromFile();
    AddFloorToGrid(grid);
    
    bool isSandBlockingSource = false;
    int numberOfSands = 0;
    while(!isSandBlockingSource)
    {
        isSandBlockingSource = DropSandUntilSource(grid);
        ++numberOfSands;
    }

    std::ofstream file("output.txt");
    Print(grid, file);

    std::cout << "\tNumber of sand bags until source is blocked: " << numberOfSands << "\n";
}

int main()
{
    std::cout << "Advent of code main\n";

    FirstPart();
    SecondPart(); 

    return 0;
}