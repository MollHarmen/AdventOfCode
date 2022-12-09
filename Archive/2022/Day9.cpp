#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include "Utils/FileIO.hpp"

struct Head
{
    void Move(const std::string direction)
    {
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
};

struct Tail
{
    Tail()
    {
        AddCurrentPossitionToVisited();
    }

    void Move(const int xHead, const int yHead)
    {
        if(DiagonalMoveRequired(xHead, yHead))
        {
            MoveDiagonally(xHead, yHead);
        }
        else
        {
            SingleAxisMove(xHead, yHead);
        }
        AddCurrentPossitionToVisited();
    }

    bool DiagonalMoveRequired(const int xHead, const int yHead)
    {
        return (x != xHead || y != yHead) && ((std::abs(x - xHead) >= 1 && std::abs(y - yHead) == 2) || (std::abs(x - xHead) == 2 && std::abs(y - yHead) >= 1));
    }

    void MoveDiagonally(const int xHead, const int yHead)
    {
        if((xHead - x) > 0)
        {
            ++x;
        }
        else
        {
            --x;
        }

        if((yHead - y) > 0)
        {
            ++y;
        }
        else
        {
            --y;
        }
    }

    void SingleAxisMove(const int xHead, const int yHead)
    {
        if(x == xHead)
        {
            if((yHead - y) > 1)
            {
                ++y;
            }
            else if((yHead - y) < -1)
            {
                --y;
            }
        }
        else if(y == yHead)
        {
            if((xHead - x) > 1)
            {
                ++x;
            }
            else if((xHead - x) < -1)
            {
                --x;
            }
        }
    }

    void AddCurrentPossitionToVisited()
    {
        positionVisited[std::to_string(x) + std::to_string(y)] = 1;
    }

    int GetNumberOfPositionsVisited()
    {
        return positionVisited.size();
    }

    int x = 0;
    int y = 0;
    std::map<std::string, int> positionVisited;
};

void Print(const Head& head, const Tail& tail)
{
    std::vector<std::vector<std::string>> grid;
    for(int x = 0; x != 5; ++x)
    {
        grid.push_back({".",".",".",".",".","."});
    }
    std::cout << "\t HEAD: " << head.x << "-" << head.y << "\n";
    std::cout << "\t TAIL: " << tail.x << "-" << tail.y << "\n";

    grid[head.y][head.x] = "H";
    grid[tail.y][tail.x] = "T";

    for(auto iter = grid.end()-1; iter != grid.begin()-1; --iter)
    {
        for(const auto& row : *iter)
        {
            std::cout << row;
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

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
            tail.Move(head.x, head.y);
        }
    }
    std::cout << "\tNumer of position visited: " << tail.GetNumberOfPositionsVisited() << "\n";
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
            tails[0].Move(head.x, head.y);

            for(int index=1; index != tails.size(); ++index)
            {
                const auto& leadingTail = tails[index-1];
                tails[index].Move(leadingTail.x, leadingTail.y);
            }
        }
    }
    std::cout << "\tNumer of position visited: " << tails[8].GetNumberOfPositionsVisited() << "\n";
}

int main()
{
    std::cout << "Advent of code main\n";

    FirstPart();
    SecondPart(); 

    return 0;
}