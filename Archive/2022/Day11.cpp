#include <algorithm>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

#include "Utils/FileIO.hpp"

typedef unsigned long long WorryLevel;
typedef std::vector<WorryLevel> StartingItems;
typedef std::function<unsigned long long(unsigned long long)> Operation;
typedef std::function<unsigned long long(unsigned long long)> Test;
struct Monkey;
typedef std::vector<Monkey> Monkeys;

struct Monkey
{
    Monkey(StartingItems startingItems, Operation operation, Test testDivision, Monkeys& monkeys)
    : mStartingItems(startingItems)
    , mOperation(operation)
    , mTestDivision(testDivision)
    , mMonkeys(monkeys)
    {}

    void Inspect()
    {
        for(auto itemIterator = mStartingItems.begin(); itemIterator != mStartingItems.end(); ++itemIterator)
        {
            *itemIterator = mOperation(*itemIterator);
            // *itemIterator = *itemIterator / 3;
            *itemIterator %= 9699690;
            
            const unsigned long long throwToMonkeyNumber = mTestDivision(*itemIterator);
            ThrowItemToMonkey(*itemIterator, throwToMonkeyNumber);

            ++inspectedItemsCount;
        }
        mStartingItems.clear();
    }

    void ThrowItemToMonkey(const WorryLevel worryLevel, const unsigned long long monkeyNumber)
    {
        mMonkeys[monkeyNumber].mStartingItems.push_back(worryLevel);
    }

    StartingItems mStartingItems;
    Operation mOperation;
    Test mTestDivision;
    Monkeys& mMonkeys;
    unsigned long long inspectedItemsCount = 0;
};

StartingItems GetStartItemsFromLine(const std::string& line)
{
    auto copy = line;
    const auto firstNumberIndex = copy.find(": ") + 2;
    copy = copy.substr(firstNumberIndex, copy.length());

    StartingItems startingItems;
    std::cout << "StartItems: ";
    while(copy.find(", ") != copy.npos)
    {
        const auto commaIndex = copy.find(", ");
        const std::string startingItem = copy.substr(0, commaIndex);
        std::cout << startingItem;
        startingItems.push_back(std::stoi(startingItem));
        copy.erase(0, commaIndex+2);
        std::cout << " ";
    }
    std::cout << copy << "\n";
    startingItems.push_back(std::stoi(copy));

    return startingItems;
}

Operation GetOperationFromLine(const std::string& line)
{
    const auto oldIndex = line.find("old ");
    const std::string operatorString = line.substr(oldIndex+4, 1);
    const std::string numberString = line.substr(oldIndex+6, line.length());
    
    std::cout << "Operation: " << operatorString << " - " << numberString << " \n";
    return [operatorString, numberString](unsigned long long old)->unsigned long long
    { 
        unsigned long long number = 0;
        if(numberString == "old")
        {
            number = old;
        }
        else
        {
            number = std::stoi(numberString);
        }

        if(operatorString == "*")
        {
            return old * number;
        } 
        else if(operatorString == "+")
        {
            return old + number;
        }
        return 0;
    };
}

Test GetTestFromLines(std::vector<std::string> lines)
{
    const std::string divisionNumberString = lines[0].substr(lines[0].find("by ")+3, lines[0].length());
    const unsigned long long divisionNumber = std::stoi(divisionNumberString);
    const unsigned long long ifTrue = std::stoi(lines[1].substr(lines[1].find("monkey ")+7, lines[1].length()));
    const unsigned long long ifFalse = std::stoi(lines[2].substr(lines[2].find("monkey ")+7, lines[2].length()));

    std::cout << "Test: " << divisionNumber << " - " << ifTrue << " | " << ifFalse << "\n";

    return [divisionNumber, ifTrue, ifFalse](unsigned long long number)
    {
        if((number % divisionNumber) == 0)
        {
            return ifTrue;
        }
        else
        {
            return ifFalse;
        }
    };
}

void GetMonkeysFromFile(Monkeys& monkeys)
{
    Utils::FileIo fileIO("../src/day11-input.txt");
    const auto filePerLine = fileIO.GetFileContent();
    
    for(unsigned long long lineIndex = 0; lineIndex != filePerLine.size(); ++lineIndex)
    {
        const auto& line = filePerLine[lineIndex];
        if(line.find("Monkey ") != line.npos)
        {
            std::cout << "Monkey: \n";
            StartingItems startItems = GetStartItemsFromLine(filePerLine[++lineIndex]);
            Operation operation = GetOperationFromLine(filePerLine[++lineIndex]);
            Test test = GetTestFromLines(std::vector<std::string>(filePerLine.begin()+(++lineIndex), filePerLine.end()));
        
            monkeys.push_back(Monkey(startItems, operation, test, monkeys));
            std::cout << "\n";
        }
    }
}

void PrintMonkeys(const Monkeys& monkeys)
{
    for(unsigned long long index = 0; index != monkeys.size(); ++index)
    {
        std::cout << "\tMonkey " << index << ": ";
        for(const auto& item : monkeys[index].mStartingItems)
        {
            std::cout << item << ", ";
        }
        std::cout << "\n";
    }
}

std::vector<unsigned long long> GetInspectCountsFromMonkeys(const Monkeys& monkeys)
{
    std::vector<unsigned long long> counts;
    for(const auto& monkey : monkeys)
    {
        counts.push_back(monkey.inspectedItemsCount);
    }
    return counts;
}

std::vector<unsigned long long> GetSortedInspectCountsFromMonkeys(const Monkeys& monkeys)
{
    std::vector<unsigned long long> counts = GetInspectCountsFromMonkeys(monkeys);
    std::sort(counts.rbegin(), counts.rend());
    return counts;
}

void FirstPart()
{
    std::cout << "= First part =\n";
    Monkeys monkeys;
    GetMonkeysFromFile(monkeys);

    for(unsigned long long numberOfRounds = 0; numberOfRounds != 10000; ++numberOfRounds)
    {
        for(auto& monkey : monkeys)
        {
            monkey.Inspect();
        }
        if(numberOfRounds+1 == 1 || numberOfRounds+1 == 20 || numberOfRounds+1 == 1000 || numberOfRounds+1 == 2000 || numberOfRounds+1 == 3000 || numberOfRounds+1 == 4000 || numberOfRounds+1 == 10000)
        {
            std::cout << numberOfRounds+1 << "\n";
            const auto counts = GetInspectCountsFromMonkeys(monkeys);
            for(const auto& count : counts)
            {
                std::cout << "\t" << count << "\n";
            }
        }
    }

    const auto counts = GetSortedInspectCountsFromMonkeys(monkeys);
    std::cout <<"\t" << counts[0] << " * " << counts[1] << " = " << counts[0]*counts[1] << "\n";
}

void SecondPart()
{
    std::cout << "= Second part =\n";
}

int main()
{
    std::cout << "Advent of code main\n";

    FirstPart();
    SecondPart(); 

    return 0;
}