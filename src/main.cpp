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

bool DoesGroupFit(const std::string& report, const int groupSize, const size_t position)
{
    const size_t groupEndPosition = position + groupSize;
    if (groupEndPosition > report.length())
    {
        return false;
    }

    for (size_t index = position; index != groupEndPosition; ++index)
    {
        if(report[index] == '.')
        {
            return false;
        }
    }

    if (groupEndPosition < report.length())
    {
        if (report[groupEndPosition] == '#') // Group would be too large
        {
            return false;
        }
    }

    return true;
}

std::string ReplaceInString(const std::string& input, const size_t position, const int groupSize)
{
    std::string output = input;
    const size_t groupEndPosition = position + groupSize;
    for(size_t index = position; index != groupEndPosition; ++index)
    {
        output[index] = '#';
    }

    if(groupEndPosition < input.length())
    {
        output[groupEndPosition] = '.';
    }

    return output;
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

    return true;
}

Groups RemoveFirstElement(const Groups& groups)
{
    Groups groupsCopy = groups;
    groupsCopy.erase(groupsCopy.begin());

    return groupsCopy;
}

int GetNumberOfPossibilities(std::string conditionReport, Groups groups, const size_t position = 0u)
{
    int result = 0;
    for (size_t index = position; index < conditionReport.length(); ++index)
    {
        const char& spring = conditionReport[index];
        if(spring == '?')
        {
            if(!groups.empty() && DoesGroupFit(conditionReport, groups[0], index))
            {
                const std::string replacedReport = ReplaceInString(conditionReport, index, groups[0]);
                Groups groupsCopy = RemoveFirstElement(groups);

                if (groupsCopy.empty())
                {
                    // std::cout << "Found: " << replacedReport << "\n";
                    result += 1;
                }

                result += GetNumberOfPossibilities(replacedReport, groupsCopy, index + groups[0]);
            }

            conditionReport[index] = '.';
            // result += GetNumberOfPossibilities(conditionReport, groups, index);
        }
        else if(spring == '#')
        {
            if (!groups.empty() && IsGroup(conditionReport, index, groups[0]))
            {
                if(index + groups[0] < conditionReport.length() && conditionReport[index + groups[0]] == '?')
                {
                    conditionReport[index + groups[0]] = '.';
                }

                index += groups[0];
                groups = RemoveFirstElement(groups);

                if (groups.empty())
                {
                    // std::cout << "Found: " << conditionReport << "\n";
                    result += 1;
                }
            }
            else if(!groups.empty() && DoesGroupFit(conditionReport, groups[0], index))
            {
                const std::string replacedReport = ReplaceInString(conditionReport, index, groups[0]);
                Groups groupsCopy = RemoveFirstElement(groups);

                if (groupsCopy.empty())
                {
                    // std::cout << "Found: " << replacedReport << "\n";
                    result += 1;
                }

                result += GetNumberOfPossibilities(replacedReport, groupsCopy, index + groups[0]);
            }
            else
            {
                // Not posible, return 0
                return result;
            }
        }
    }

    return result;
}

void FirstPart()
{
    std::cout << "= First part =\n";
    Utils::FileIo fileIo("C:/projects/AdventOfCode/src/input.txt");
    const std::vector<std::string> filePerLine = fileIo.GetFileContent();

    int answer = 0;

    for(const auto& line : filePerLine)
    {
        const auto deviderPosition = line.find(" ");
        const std::string conditionReport = line.substr(0, deviderPosition);
        const std::string groupsString = line.substr(deviderPosition, line.length() - deviderPosition);
        const Groups groups = GetGroupsFromString(groupsString);

        const int numberOfPossibilities = GetNumberOfPossibilities(conditionReport, groups);
        // std::cout << "Result: " << numberOfPossibilities << "\n";
        answer += numberOfPossibilities;
    }

    std::cout << "Result: " << answer << "\n";
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