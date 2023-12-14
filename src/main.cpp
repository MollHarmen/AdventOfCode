#include <iostream>

#include "Utils/FileIO.hpp"
#include "Utils/StringHandling.hpp"

typedef std::vector<char> Column;
typedef std::vector<Column> Platform;

void Print(const Platform& platform)
{
    for(size_t rowIndex = 0u; rowIndex != platform[0].size(); ++rowIndex)
    {
        for(size_t columnIndex = 0u; columnIndex != platform.size(); ++columnIndex)
        {
            std::cout << platform[columnIndex][rowIndex];
        }
        std::cout << "\n";
    }
}

void Swap(std::vector<char>::iterator& right, std::vector<char>::iterator& left)
{
    char leftContent = *left;
    *left = *right;
    *right = leftContent;
}

void Swap(std::vector<char>::reverse_iterator& right, std::vector<char>::reverse_iterator& left)
{
    char leftContent = *left;
    *left = *right;
    *right = leftContent;
}

void TiltPlatformToNorth(Platform& platform)
{
    for(auto& column : platform)
    {
        for(size_t columnIndex = 0u; columnIndex != column.size(); ++columnIndex)
        {
            for(auto iterator = column.begin() + 1; iterator != column.end(); ++iterator)
            {
                if(*iterator == 'O' && *(iterator-1) == '.')
                {
                    Swap(iterator, iterator-1);
                }
            }
        }
    }
}

void TiltPlatformToWest(Platform& platform)
{
    for(size_t rowIndex = 0u; rowIndex != platform[0].size(); ++rowIndex)
    {
        for(size_t row = 0u; row != platform.size(); ++row)
        {
            for(size_t columnIndex = 1u; columnIndex != platform.size(); ++columnIndex)
            {
                char& right = platform[columnIndex][rowIndex];
                char& left = platform[columnIndex-1][rowIndex];
                if(right == 'O' && left == '.')
                {
                    char temp = left;
                    left = right;
                    right = temp;
                }
            }
        }
    }
}

void TiltPlatformToSouth(Platform& platform)
{
    for(auto& column : platform)
    {
        for(size_t columnIndex = 0u; columnIndex != column.size(); ++columnIndex)
        {
            for(auto iterator = column.rbegin() + 1; iterator != column.rend(); ++iterator)
            {
                if(*iterator == 'O' && *(iterator-1) == '.')
                {
                    Swap(iterator, iterator-1);
                }
            }
        }
    }
}

void TiltPlatformToEast(Platform& platform)
{
    for(size_t rowIndex = platform[0].size()-1; rowIndex != -1; --rowIndex)
    {
        for(size_t row = 0u; row != platform.size(); ++row)
        {
            for(size_t columnIndex = 1u; columnIndex != platform.size(); ++columnIndex)
            {
                char& right = platform[columnIndex][rowIndex];
                char& left = platform[columnIndex-1][rowIndex];
                if(left == 'O' && right == '.')
                {
                    char temp = left;
                    left = right;
                    right = temp;
                }
            }
        }
    }
}

int GetWeightForColumn(const Column& column)
{
    const int columnSize = column.size();
    int weight = 0; 

    for(size_t rowIndex = 0u; rowIndex != column.size(); ++rowIndex)
    {
        if(column[rowIndex] == 'O')
        {
            weight += columnSize - rowIndex;
        }
    }

    return weight;
}

int GetWeightOffPlatform(const Platform& platform)
{
    int weight = 0;
    for(const auto& column : platform)
    {
        weight += GetWeightForColumn(column);
    }

    return weight;
}

void PerformTiltingCycle(Platform& platform)
{
    TiltPlatformToNorth(platform);
    TiltPlatformToWest(platform);
    TiltPlatformToSouth(platform);
    TiltPlatformToEast(platform);
}

void FirstPart()
{
    std::cout << "= First part =\n";
    Utils::FileIo fileIo("C:/projects/AdventOfCode/src/input.txt");
    const std::vector<std::string> filePerLine = fileIo.GetFileContent();

    Platform platform;
    for(const auto& line : filePerLine)
    {
        if(platform.size() != line.length())
        {
            for(size_t columnIndex = 0u; columnIndex != line.length(); ++columnIndex)
            {
                platform.push_back(Column());
            }
        }

        for(size_t columnIndex = 0; columnIndex != filePerLine.size(); ++columnIndex)
        {
            platform[columnIndex].push_back(line[columnIndex]);
        }
    }

    for(unsigned long long reps = 0u; reps != 1000000000; ++reps)
    {
        PerformTiltingCycle(platform);
    }

    std::cout << "Weight: " << GetWeightOffPlatform(platform) << "\n";
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