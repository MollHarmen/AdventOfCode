#include <iostream>

#include "Utils/FileIO.hpp"
#include "Utils/StringHandling.hpp"

typedef std::vector<int> Groups;

Groups GetGroupsFromString(const std::string& input)
{
    std::vector<std::string> split;
    StringHandling::SplitStringOnCharacter(input, split, ',');

    Groups groups;
    for(const auto& group : split)
    {
        groups.push_back(std::stoi(group));
    }

    return groups;
}

void GetPossibleSolutions(std::vector<std::string>& solutions, std::string line)
{
    const auto position = line.find("?");
    if(position != line.npos)
    {
        std::string one = line;
        one[position] = '#';
        std::string two = line;
        two[position] = '.';

        GetPossibleSolutions(solutions, one);
        GetPossibleSolutions(solutions, two);
    }
    else
    {
        solutions.push_back(line);
    }
}

bool IsGroup(const std::string& input, const size_t position, const int groupSize)
{
    const size_t groupEndPosition = position + groupSize;
    if (groupEndPosition > input.length())
    {
        return false;
    }

    for (size_t index = position; index != groupEndPosition; ++index)
    {
        if (input[index] != '#')
        {
            return false;
        }
    }

    if (groupEndPosition < input.length())
    {
        if (input[groupEndPosition] == '#')
        {
            return false;
        }
    }

    return true;
}

bool IsValid(const std::string& solution, Groups groups)
{
    size_t groupIndex = 0u;
    for(size_t index = 0u; index < solution.length(); ++index)
    {
        if(solution[index] == '#')
        {
            if (groupIndex >= groups.size())
            {
                return false;
            }

            bool result = IsGroup(solution, index, groups[groupIndex]);
            if(!result)
            {
                return false;
            }
            else 
            {
                index += groups[groupIndex];
                ++groupIndex;
            }
        }
    }

    return groupIndex == groups.size();
}

void FirstPart()
{
    std::cout << "= First part =\n";
    Utils::FileIo fileIo("C:/projects/AdventOfCode/src/input.txt");
    const std::vector<std::string> filePerLine = fileIo.GetFileContent();


    int result = 0;
    for(const auto& line : filePerLine)
    {
        const auto deviderPosition = line.find(" ");
        const std::string conditionReport = line.substr(0, deviderPosition);
        const std::string groupsString = line.substr(deviderPosition, line.length() - deviderPosition);
        const Groups groups = GetGroupsFromString(groupsString);

        std::vector<std::string> solutions;
        GetPossibleSolutions(solutions, conditionReport);

        for(const auto& solution : solutions)
        {
            if(IsValid(solution, groups))
            {
                ++result;
            }
        }
    }

    std::cout << "Result: " << result << "\n";
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