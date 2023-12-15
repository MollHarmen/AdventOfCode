#include <algorithm>
#include <iostream>
#include <map>
#include <numeric>
#include <vector>

#include "Utils/FileIO.hpp"
#include "Utils/StringHandling.hpp"

typedef std::tuple<std::string, std::string> Node;
typedef std::map<std::string, Node> Network;

Node GetNodeFromString(const std::string& input)
{
    std::string copy = input;
    copy.erase(0, 1);
    copy.erase(copy.size() - 1);

    std::vector<std::string> split;
    StringHandling::SplitOnString(copy, split, ", ");

    return std::make_tuple(split[0], split[1]);
}

void FirstPart()
{
    std::cout << "= First part =\n";
    Utils::FileIo fileIo("C:/projects/AdventOfCode/src/input.txt");
    const std::vector<std::string> filePerLine = fileIo.GetFileContent();

    std::string instructions;
    Network network;

    for(const auto& line : filePerLine)
    {
        if(instructions.length() == 0)
        {
            instructions = line;
        }
        else
        {
            if(!line.empty())
            {
                std::vector<std::string> split;
                StringHandling::SplitOnString(line, split, " = ");
                network[split[0]] = GetNodeFromString(split[1]);
            }
        }
    }

    std::string position = "AAA";
    size_t instructionIndex = 0;
    int numberOfSteps = 0;

    while(position != "ZZZ")
    {
        const Node& node = network[position];
        if(instructions[instructionIndex] == 'L')
        {
            position = std::get<0>(node);
        }
        else
        {
            position = std::get<1>(node);
        }
        instructionIndex = ++instructionIndex % instructions.length();
        ++numberOfSteps;
    }

    std::cout << "Number of steps taken: " << numberOfSteps << "\n";
}

std::vector<std::string> GetStartPositions(const Network& network)
{
    std::vector<std::string> positions;
    for(const auto& mapItem : network)
    {
        const std::string nodeKey = mapItem.first;
        if(nodeKey[2] == 'A')
        {
            positions.push_back(nodeKey);
        }
    }
    return positions;
}

bool EndReached(std::vector<std::string>& positions)
{
    for(const auto& position : positions)
    {
        if(position[2] != 'Z')
        {
            return false;
        }
    }

    return true;
}

int GcdOfArray(std::vector<int>& arr, int idx)
{ 
    if (idx == arr.size() - 1) { 
        return arr[idx]; 
    } 
    int a = arr[idx]; 
    int b = GcdOfArray(arr, idx + 1); 
    return std::gcd(a, b); // __gcd(a,b) is inbuilt library function 
} 

void SecondPart()
{
    std::cout << "= Second part =\n";
    Utils::FileIo fileIo("C:/projects/AdventOfCode/src/input.txt");
    const std::vector<std::string> filePerLine = fileIo.GetFileContent();

    std::string instructions;
    Network network;

    for(const auto& line : filePerLine)
    {
        if(instructions.length() == 0)
        {
            instructions = line;
        }
        else
        {
            if(!line.empty())
            {
                std::vector<std::string> split;
                StringHandling::SplitOnString(line, split, " = ");
                network[split[0]] = GetNodeFromString(split[1]);
            }
        }
    }

    std::vector<std::string> positions = GetStartPositions(network);
    std::vector<int> numberOfStepsTaken;
    for(auto& position : positions)
    {
        size_t instructionIndex = 0;
        int numberOfSteps = 0;
        while(position[2] != 'Z')
        {
            const Node& node = network[position];
            if(instructions[instructionIndex] == 'L')
            {
                position = std::get<0>(node);
            }
            else
            {
                position = std::get<1>(node);
            }
            instructionIndex = ++instructionIndex % instructions.length();
            ++numberOfSteps;
        }
        numberOfStepsTaken.push_back(numberOfSteps);
    }

    std::sort(numberOfStepsTaken.begin(), numberOfStepsTaken.end());
    unsigned long long multiplier = GcdOfArray(numberOfStepsTaken, 0);


    // while(!allMatch)
    // {
    //     unsigned long long numberOfSteps = numberOfStepsTaken[0] * multiplier;
    //     bool noLeftOver = true;
    //     for(const int steps : numberOfStepsTaken)
    //     {
    //         if(noLeftOver)
    //         {
    //             noLeftOver &= numberOfSteps % steps == 0;
    //         }
    //     }
        
    //     if(noLeftOver)
    //     {
    //         allMatch = true;
    //     }
    //     ++multiplier;
    // }

    std::cout << "Number of steps taken: " << numberOfStepsTaken[0] * multiplier << "\n";
}

int main()
{
    std::cout << "Advent of code main\n";

    // FirstPart();
    SecondPart();

    return 0;
}