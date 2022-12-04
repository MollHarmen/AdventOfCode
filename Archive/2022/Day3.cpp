#include <iostream>
#include <numeric>
#include <string>
#include <vector>

#include "Utils/FileIO.hpp"

typedef std::vector<std::string> Rugsack;

Rugsack GetRugsackFromString(const std::string& input)
{
    const std::string firstCompartiment = input.substr(0, input.length()/2);
    const std::string secondCompartiment = input.substr(input.length()/2, input.length());

    return Rugsack {firstCompartiment, secondCompartiment};
}

std::vector<Rugsack> GetRugsacksFromFile()
{
    Utils::FileIo fileIo("../src/day3-input.txt");
    const auto filePerLines = fileIo.GetFileContent();

    std::vector<Rugsack> rugsacks;
    for(const auto& line : filePerLines)
    {
        rugsacks.push_back(GetRugsackFromString(line));
    }

    return rugsacks;
}

char GetSharedItemInRugsack(const Rugsack& rugsack)
{
    for(const auto& item : rugsack[0])
    {
        const auto itemIter = rugsack[1].find(item);
        if(itemIter != rugsack[1].npos)
        {
            return item;
        }
    }
    return {};
}

int GetValueOfItem(const char& item)
{
    if(item >= 'A' && item <= 'Z')
    {
        return static_cast<int>(item - 38);
    }
    else return static_cast<int>(item - 96);
}

std::vector<char> GetSharedItemsOfAllRugsacks(const std::vector<Rugsack>& rugsacks)
{
    std::vector<char> sharedItems;
    for(const auto& rugsack : rugsacks)
    {
        sharedItems.push_back(GetSharedItemInRugsack(rugsack));
    }

    return sharedItems;
}

std::vector<int> GetValuesOfItems(const std::vector<char> items)
{
    std::vector<int> values;
    for(const auto& item: items)
    {
        values.push_back(GetValueOfItem(item));
    }

    return values;
}

void FirstPart()
{
    std::cout << "= First part =\n";
    const std::vector<Rugsack> rugsacks = GetRugsacksFromFile();
    const auto sharedItemsOfRugsacks = GetSharedItemsOfAllRugsacks(rugsacks);
    const auto valuesOfSharedItems = GetValuesOfItems(sharedItemsOfRugsacks);

    const auto totalScore = std::accumulate(valuesOfSharedItems.begin(), valuesOfSharedItems.end(), 0);

    std::cout << "\tTotal value of all shared items: " << std::to_string(totalScore) << "\n";
}

char FindGroupItemInRugsacks(const std::string& firstRugsack, const std::string& secondRugsack, const std::string& thirdRugsack)
{
    for(const auto& item : firstRugsack)
    {
        if(secondRugsack.find(item) != secondRugsack.npos && thirdRugsack.find(item) != thirdRugsack.npos)
        {
            return item;
        }
    }

    return '?';
}

std::vector<char> IdentifyGroupItemsFromRugsacks(const std::vector<std::string>& rugsacks)
{
    constexpr size_t groupSize = 3;
    std::vector<char> groupItems;
    for(size_t index = 0; index < rugsacks.size() - 2; index+=groupSize)
    {
        const auto& firstRugsack = rugsacks[index];
        const auto& secondRugsack = rugsacks[index+1];
        const auto& thirdRugsack = rugsacks[index+2];
        groupItems.push_back(FindGroupItemInRugsacks(firstRugsack, secondRugsack, thirdRugsack));
    }

    return groupItems;
}

void SecondPart()
{
    std::cout << "= Second part =\n";
    Utils::FileIo fileIo("../src/day3-input.txt");
    const auto filePerLines = fileIo.GetFileContent();

    const std::vector<char> groupItems = IdentifyGroupItemsFromRugsacks(filePerLines);
    const auto valuesOfGroupItems = GetValuesOfItems(groupItems);

    const auto totalScore = std::accumulate(valuesOfGroupItems.begin(), valuesOfGroupItems.end(), 0);
    std::cout << "\tTotal score of goup items: " << std::to_string(totalScore) << "\n";

}

int main()
{
    std::cout << "Advent of code main\n";

    FirstPart();
    SecondPart(); 

    return 0;
}