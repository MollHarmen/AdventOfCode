#include <algorithm>
#include <iostream>
#include <map>
#include <stdexcept>
#include <vector>

#include "Utils/FileIO.hpp"
#include "Utils/StringHandling.hpp"

struct Trench
{
    Trench(const int x, const int y, const std::string color)
    : mX(x)
    , mY(y)
    , mColor(color)
    {}

    bool operator<(const Trench& other)
    {
        return mX < other.mX;
    }

    bool operator==(const Trench& other)
    {
        return mX == other.mX && mY == other.mY;
    }

    int mX;
    int mY;
    std::string mColor;

    Trench* mNabour;
    int mDistance = 0;
};

typedef std::map<int, std::vector<Trench>> Trenches;

int GetTrenchInteriorSize(Trenches& trenches)
{
    int accumulate = 0;
    for(auto& rowTuple : trenches)
    {
        auto& row = std::get<1>(rowTuple);
        std::sort(row.begin(), row.end());

        const int distanceToLeft = row[row.size()-1].mX - row[0].mX +1;
        accumulate += distanceToLeft;
    }

    return accumulate;
}

void AddTrenchesInYRanche(Trenches& trenches, const int x, const int yStart, const int numberOfTrenches, const std::string color)
{
    for(int index = yStart; index != yStart+numberOfTrenches+1; ++index)
    {
        const auto found = std::find_if(trenches[index].begin(), trenches[index].end(), [&x](const auto& trench){ return trench.mX == x;});
        if(found == trenches[index].end())
        {
            trenches[index].push_back({x, index, color});
        }
    }
}

void FirstPart()
{
    std::cout << "= First part =\n";
    Utils::FileIo fileIo("C:/projects/AdventOfCode/src/input.txt");
    const std::vector<std::string> filePerLine = fileIo.GetFileContent();

    Trenches trenches;

    int x = 0;
    int y = 0;
    for(const auto& line : filePerLine)
    {
        std::vector<std::string> split;
        StringHandling::SplitStringOnCharacter(line, split, ' ');

        const std::string& direction = split[0];
        const int distance = std::stoi(split[1]);
        const std::string color = split[2];

        if(direction == "R")
        {
            x += distance;
            trenches[y].push_back({x, y, color});
        }
        else if(direction == "L")
        {
            x -= distance;
            trenches[y].push_back({x, y, color});
        }
        else if(direction == "U")
        {
            AddTrenchesInYRanche(trenches, x, y - distance, distance, color);
            y -= distance;
        }
        else if(direction == "D")
        {
            AddTrenchesInYRanche(trenches, x, y, distance, color);
            y += distance;
        }
        else
        {
            std::cout << "INVALLID DIRECTION!?\n";
        }
    }

    const int interiorSize = GetTrenchInteriorSize(trenches);
    std::cout << "InteriorSize: " << interiorSize << "\n";
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