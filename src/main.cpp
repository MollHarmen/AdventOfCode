#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include "Utils/FileIO.hpp"

struct Head
{
    void Move(const std::string& direction)
    {
        previousPosition = std::make_pair(x, y);

        if(direction == "U")
        {
            ++y;
        } 
        else if(direction == "D")
        {
            --y;
        }
        else if(direction == "L")
        {
            --x;
        }
        else if(direction == "R")
        {
            ++x;
        }
    }

    int x = 0;
    int y = 0;

    std::pair<int,int> previousPosition;
};

struct Tail
{
    Tail()
    {
        AddCurrentPossitionToVisited();
    }

    void AddCurrentPossitionToVisited()
    {
        positionVisited[std::to_string(mX) + std::to_string(mY)] = 1;
    }

    void Move(const int x, const int y)
    {
        mX = x;
        mY = y;
        AddCurrentPossitionToVisited();
    }

    bool NeedsToMove(const int x, const int y)
    {
        return std::abs(x - mX) > 1 || std::abs(y - mY) > 1;
    }

    int GetNumberOfPositionsVisited()
    {
        return positionVisited.size();
    }

    int mX = 0;
    int mY = 0;
    std::map<std::string, int> positionVisited;
};

void FirstPart()
{
    std::cout << "= First part =\n";
    Utils::FileIo fileIo("../src/day9-input.txt");
    const auto filePerLine = fileIo.GetFileContent();

    Head head;
    Tail tail;

    for(const auto& line : filePerLine)
    {
        const std::string direction = line.substr(0, 1);
        const int numberOfSteps = std::stoi(line.substr(2, line.length()));

        for(int stepsTaken = 0; stepsTaken != numberOfSteps; ++stepsTaken)
        {
            head.Move(direction);
            if(tail.NeedsToMove(head.x, head.y))
            {
                tail.Move(std::get<0>(head.previousPosition), std::get<1>(head.previousPosition));
            }
        }
    }

    std::cout << "\tNumber of unique position visited by tail: " << std::to_string(tail.GetNumberOfPositionsVisited()) << "\n";
}

void SecondPart()
{
    std::cout << "= Second part =\n";
    Utils::FileIo fileIo("../src/day9-input.txt");
    const auto filePerLine = fileIo.GetFileContent();

    Head head;
    std::array<Tail, 9> tails = {};

    for(const auto& line : filePerLine)
    {
        const std::string direction = line.substr(0, 1);
        const int numberOfSteps = std::stoi(line.substr(2, line.length()));

        for(int stepsTaken = 0; stepsTaken != numberOfSteps; ++stepsTaken)
        {
            head.Move(direction);
            for(int index = 0; index != tails.size(); ++index)
            {
                if(index = 0)
                {
                    if(tails[index].NeedsToMove(head.x, head.y))
                    {
                        tails[index].Move(head.x, head.y);
                    }
                }
                else
                {
                    if(tails[index].NeedsToMove(tails[index-1].mX, tails[index-1].mY))
                    {
                        tails[index].Move(tails[index-1].mX, tails[index-1].mY);
                    }   
                }
            }   
        }
    }
    std::cout << "\tNumber of unique position visited by tail: " << std::to_string(tails[8].GetNumberOfPositionsVisited()) << "\n";
}

int main()
{
    std::cout << "Advent of code main\n";

    FirstPart();
    SecondPart(); 

    return 0;
}