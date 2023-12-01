#include <algorithm>
#include <iostream>
#include <list>
#include <vector>

#include "Utils/FileIO.hpp"

std::vector<int> GetUniqueNumbersFromLines(const std::vector<std::string>& lines)
{
    std::vector<int> numbers;
    for(const auto& line : lines)
    {
        const auto iterator = std::find(numbers.begin(), numbers.end(), std::stoi(line));
        if(iterator == numbers.end())
        {
            numbers.push_back(std::stoi(line));
        }
    }

    return numbers;
}

void FirstPart()
{
    std::cout << "= First part =\n";
    Utils::FileIo fileIo("../input.txt");
    const std::vector<std::string> filePerLine = fileIo.GetFileContent();

    const std::vector<int> numbers = GetUniqueNumbersFromLines(filePerLine);
    
    for(const auto& number : numbers)
    {
        const auto iterator = std::find(numbers.begin(), numbers.end(), 2020 - number);
        if(iterator != numbers.end())
        {
            std::cout << "Found it! Values are: " << number << " and " << *iterator << "\n";
            std::cout << "Answer: " << number * *iterator << "\n";
        }
    }

}

void SecondPart()
{
    std::cout << "= Second part =\n";
    Utils::FileIo fileIo("../input.txt");
    const auto filePerLine = fileIo.GetFileContent();

    const std::vector<int> numbers = GetUniqueNumbersFromLines(filePerLine);
    
    for(const auto& number : numbers)
    {
        for(const auto& secondNumber : numbers)
        {
            const auto iterator = std::find(numbers.begin(), numbers.end(), 2020 - number - secondNumber);
            if(iterator != numbers.end())
            {
                std::cout << "Found it! Values are: " << number << ", " << secondNumber << " and " << *iterator << "\n";
                std::cout << "Answer: " << number * secondNumber * *iterator << "\n";
            }
        }
    }

}

int main()
{
    std::cout << "Advent of code main\n";

    // FirstPart();
    SecondPart();

    return 0;
}