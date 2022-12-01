#include <iostream>

#include "FileIO.hpp"

constexpr int indexOffset = 1;

int CountNumberOfCharactersInString(const std::string& input, const char character)
{
    int count = 0;
    for(const char& inputChar : input)
    {
        if(inputChar == character)
        {
            ++count;
        }
    }

    return count;
}

int FindIndexWhereCountReachesMinusOne(const std::string& input)
{
    int count = 0;
    for(auto index = 0; index != input.size(); ++index)
    {
        if(input[index] == '(')
        {
            ++count;
        }
        else
        {
            --count;
        }
        if(count == -1)
            return index + indexOffset;
    }
}

std::string GetInputFileAsString()
{
    Utils::FileIo fileIo("../day1-input.txt");
    return fileIo.GetNextLine();
}

void FirstPart()
{
    std::cout << "First part\n";
    const std::string input = GetInputFileAsString();

    const int plusCount = CountNumberOfCharactersInString(input, '(');
    const int minusCount = CountNumberOfCharactersInString(input, ')');

    std::cout << "\tFloor = " << plusCount - minusCount << "\n";
}

void SecondPart()
{
    std::cout << "Second part\n";

    const std::string input = GetInputFileAsString();
    const auto index = FindIndexWhereCountReachesMinusOne(input);

    std::cout << "\tBasement reached at index " << std::to_string(index) << "\n";
}

int main()
{
    std::cout << "Advent of code main\n";

    FirstPart();
    SecondPart();   

    return 0;
}