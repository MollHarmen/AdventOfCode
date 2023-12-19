#include <algorithm>
#include <iostream>
#include <map>
#include <stdexcept>
#include <vector>

#include "Utils/FileIO.hpp"
#include "Utils/StringHandling.hpp"

struct Trench
{
    Trench(const unsigned long long x, const unsigned long long y, const std::string color)
    : mX(x)
    , mY(y)
    , mColor(color)
    {}

    unsigned long long mX;
    unsigned long long mY;
    std::string mColor;
};

typedef std::vector<Trench> Trenches;

void PrintCoordinates(const Trenches& trenches)
{
    for(const auto& trench : trenches)
    {
        std::cout << trench.mX << ", " << trench.mY << "\n";
    }
}

int ShoeLaceAlgorithm(const int x1, const int x2, const int y1, const int y2)
{
    return (x1 * y2) - (x2 * y1);
}

unsigned long long TrapezoidFormula(const unsigned long long x1, const unsigned long long x2, const unsigned long long y1, const unsigned long long y2)
{
    return (y1 + y2) * (x1 -x2);
}

int GetTrenchInteriorSize(const Trenches& trenches)
{
    int result = 0;
    for(size_t trenchIndex = 0u; trenchIndex != trenches.size() - 1; ++trenchIndex)
    {
        const int x1 = trenches[trenchIndex].mX;
        const int x2 = trenches[trenchIndex+1].mX;
        const int y1 = trenches[trenchIndex].mY;
        const int y2 = trenches[trenchIndex+1].mY;

        result += ShoeLaceAlgorithm(x1, x2, y1, y2);
    }

    const auto& last = *trenches.rbegin();
    const auto& first = *trenches.begin();
    result += ShoeLaceAlgorithm(last.mX, first.mX, last.mY, first.mY);

    return result / 2;
}

int CalculateOutline(const int x1, const int x2, const int y1, const int y2)
{
    return std::abs(x2 - x1) + std::abs(y2 - y1);
}

unsigned long long GetTrenchInteriorSizeTrapezoidFormula(const Trenches& trenches)
{
    unsigned long long result = 0;
    unsigned long long outline = 0;
    for(size_t trenchIndex = 0u; trenchIndex != trenches.size() - 1; ++trenchIndex)
    {
        const unsigned long long x1 = trenches[trenchIndex].mX;
        const unsigned long long x2 = trenches[trenchIndex+1].mX;
        const unsigned long long y1 = trenches[trenchIndex].mY;
        const unsigned long long y2 = trenches[trenchIndex+1].mY;

        result += TrapezoidFormula(x1, x2, y1, y2);
        outline += CalculateOutline(x1, x2, y1, y2);
    }

    const auto& last = *trenches.rbegin();
    const auto& first = *trenches.begin();
    result += TrapezoidFormula(last.mX, first.mX, last.mY, first.mY);
    outline += CalculateOutline(last.mX, first.mX, last.mY, first.mY);

    return result / 2 + ((outline / 2) + 1);
}

void FirstPart()
{
    std::cout << "= First part =\n";
    Utils::FileIo fileIo("C:/projects/AdventOfCode/src/input.txt");
    const std::vector<std::string> filePerLine = fileIo.GetFileContent();

    Trenches trenches;

    unsigned long long x = 0;
    unsigned long long y = 0;
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
            trenches.push_back({x, y, color});
        }
        else if(direction == "L")
        {
            x -= distance;
            trenches.push_back({x, y, color});
        }
        else if(direction == "U")
        {
            y -= distance;
            trenches.push_back({x, y, color});
        }
        else if(direction == "D")
        {
            y += distance;
            trenches.push_back({x, y, color});
        }
        else
        {
            std::cout << "INVALLID DIRECTION!?\n";
        }
    }
    const int interiorSizeTrapezoid = GetTrenchInteriorSizeTrapezoidFormula(trenches);
    std::cout << "InteriorSize with Trapezoid formula: " << interiorSizeTrapezoid << "\n";
}

enum class Direction : int
{
    Right = 0,
    Down = 1,
    Left = 2,
    Up = 3
};

Direction GetDirectionFromColor(const std::string& color)
{
    return static_cast<Direction>(std::stoi(color.substr(color.size()-2, 1)));
}

unsigned GetDistanceFromColor(const std::string& color)
{
    const std::string substr = color.substr(2, 5);
    unsigned distance;

    std::stringstream ss;
    ss << std::hex << substr;
    ss >> distance;

    return distance;
}

void SecondPart()
{
    std::cout << "= Second part =\n";
    Utils::FileIo fileIo("C:/projects/AdventOfCode/src/input.txt");
    const std::vector<std::string> filePerLine = fileIo.GetFileContent();

    Trenches trenches;

    unsigned long long x = 0u;
    unsigned long long y = 0u;
    for(const auto& line : filePerLine)
    {
        std::vector<std::string> split;
        StringHandling::SplitStringOnCharacter(line, split, ' ');

        const std::string color = split[2];
        const Direction direction = GetDirectionFromColor(color);
        const unsigned long long distance = GetDistanceFromColor(color);
        if(direction == Direction::Right)
        {
            x += distance;
            trenches.push_back({x, y, color});
        }
        else if(direction == Direction::Left)
        {
            x -= distance;
            trenches.push_back({x, y, color});
        }
        else if(direction == Direction::Up)
        {
            y -= distance;
            trenches.push_back({x, y, color});
        }
        else if(direction == Direction::Down)
        {
            y += distance;
            trenches.push_back({x, y, color});
        }
        else
        {
            std::cout << "INVALLID DIRECTION!?\n";
        }
    }
    const unsigned long long interiorSizeTrapezoid = GetTrenchInteriorSizeTrapezoidFormula(trenches);
    std::cout << "InteriorSize with Trapezoid formula: " << interiorSizeTrapezoid << "\n";
}

int main()
{
    std::cout << "Advent of code main\n";

    // FirstPart();
    SecondPart();

    return 0;
}