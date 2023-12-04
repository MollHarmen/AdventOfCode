#include <algorithm>
#include <iostream>
#include <list>
#include <vector>

#include "Utils/FileIO.hpp"

int GetNumberOfCharsInString(const char character, const std::string& string)
{
    int count = 0;
    for(const char& sourceChar : string)
    {
        if(sourceChar == character)
        {
            ++count;
        }
    }

    return count;
}
struct Policy
{
    Policy(const int min, const int max, const char letter)
    : minCount(min)
    , maxCount(max)
    , mLetter(letter)
    {}

    const int minCount = 0;
    const int maxCount = 0;
    const char mLetter = 'a';
};

struct DatabaseItem
{
    DatabaseItem(Policy policy, const std::string password)
    : mPolicy(policy)
    , mPassword(password)
    {}

    // bool IsValid() const
    // {
    //     const auto numberOfCharsInString = GetNumberOfCharsInString(mPolicy.mLetter, mPassword);

    //     return numberOfCharsInString >= mPolicy.minCount && numberOfCharsInString <= mPolicy.maxCount;
    // }

    bool IsValid() const
    {
        const bool isFirstIndexCharacterEqual = (mPassword[mPolicy.minCount] == mPolicy.mLetter);
        const bool isSecondIndexCharacterEqual = (mPassword[mPolicy.maxCount] == mPolicy.mLetter);

        return (isFirstIndexCharacterEqual && !isSecondIndexCharacterEqual) || (!isFirstIndexCharacterEqual && isSecondIndexCharacterEqual);
    }

    Policy mPolicy;
    const std::string mPassword;
};


std::vector<DatabaseItem> GetDatabaseItemsFromLines(const std::vector<std::string>& lines)
{
    std::vector<DatabaseItem> databaseItems;
    for(const auto& line : lines)
    {
        const auto dashPosition = line.find("-");
        const int min = std::stoi(line.substr(0, dashPosition));
        const auto firstSpacePosition = line.find(" ");
        const int max = std::stoi(line.substr(dashPosition+1, firstSpacePosition));
        const auto colonPosition = line.find(":");
        const char letter = line[colonPosition-1];
        const std::string password = line.substr(colonPosition+1);

        databaseItems.push_back({ { min, max, letter }, password });
    }

    return databaseItems;
}

void FirstPart()
{
    std::cout << "= First part =\n";
    Utils::FileIo fileIo("C:/projects/AdventOfCode/build/input.txt");
    const std::vector<std::string> filePerLine = fileIo.GetFileContent();

    std::vector<DatabaseItem> databaseItems = GetDatabaseItemsFromLines(filePerLine);
    int count = 0;
    for(const auto& dbItem : databaseItems)
    {
        if(dbItem.IsValid())
        {
            std::cout << dbItem.mPassword << " is valid\n";
            ++count;
        }
    }

    std::cout << "\n\t" << count << " passwords are valid\n";
}

void SecondPart()
{
    std::cout << "= Second part =\n";
    Utils::FileIo fileIo("../input.txt");
    const auto filePerLine = fileIo.GetFileContent();

    std::vector<DatabaseItem> databaseItems = GetDatabaseItemsFromLines(filePerLine);
    int count = 0;
    for(const auto& dbItem : databaseItems)
    {
        if(dbItem.IsValid())
        {
            std::cout << dbItem.mPassword << " is valid\n";
            ++count;
        }
    }

    std::cout << "\n\t" << count << " passwords are valid\n";
}

int main()
{
    std::cout << "Advent of code main\n";

    FirstPart();
    // SecondPart();

    return 0;
}