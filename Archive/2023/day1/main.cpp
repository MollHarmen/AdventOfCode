#include <algorithm>
#include <array>
#include <iostream>
#include <sstream>
#include <vector>
#include <tuple>

#include "Utils/FileIO.hpp"

const std::array<std::string, 9> digits { "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};
const std::array<char, 9> numbers { '1','2','3','4','5','6','7','8','9' };

std::tuple<std::string, size_t> FindFirstStringDigit(const std::string& line)
{
    size_t foundPosition = line.length();
    std::string foundNumber;
    for(const auto& digit : digits)
    {
        const size_t position = line.find(digit);
        if(position != line.npos)
        {
            if (position < foundPosition)
            {
                foundPosition = position;
                foundNumber = digit;
            }
        }
    }

    return { foundNumber, foundPosition };
}

std::tuple<std::string, size_t> FindLastStringDigit(const std::string& line)
{
    size_t foundPosition = 0u;
    std::string foundNumber;
    for(const auto& digit : digits)
    {
        const size_t position = line.rfind(digit);
        if(position != line.npos)
        {
            if (position > foundPosition)
            {
                foundPosition = position;
                foundNumber = digit;
            }
        }
    }

    return { foundNumber, foundPosition };
}

std::tuple<unsigned, size_t> FindFirstDigit(const std::string& line)
{
    const auto firstDigitIterator = std::find_if(line.begin(), line.end(), [](const char& character){ return isdigit(character); });

    if(firstDigitIterator != line.end())
    {
        const unsigned number = std::find(numbers.begin(), numbers.end(), *firstDigitIterator) - numbers.begin();
        return std::make_tuple(number, std::distance(line.begin(), firstDigitIterator));
    }

    return { 0, line.length() };
}

std::tuple<unsigned, size_t> FindLastDigit(const std::string& line)
{
    const std::string::const_reverse_iterator lastDigitIterator = std::find_if(line.rbegin(), line.rend(), [](const char& character){ return isdigit(character); });

    if(lastDigitIterator != line.rend())
    {
        const unsigned number = std::find(numbers.begin(), numbers.end(), *lastDigitIterator) - numbers.begin();
        return std::make_tuple(number, std::distance(line.begin(), lastDigitIterator.base()) - 1);
    }

    return { 0, 0 };
}

char GetFirstDigitFromLine(const std::string& line)
{
    const auto foundStringDigitTuple = FindFirstStringDigit(line);
    const auto foundDigitTuple = FindFirstDigit(line);

    if(std::get<1>(foundStringDigitTuple) < std::get<1>(foundDigitTuple))
    {
        const auto stringDigitIterator = std::find(digits.begin(), digits.end(), std::get<0>(foundStringDigitTuple));
        const auto index = (stringDigitIterator - digits.begin());
        return numbers[index];
    }
    else
    {
        return numbers[std::get<0>(foundDigitTuple)];
    }
}

char GetLastDigitFromLine(const std::string& line)
{
    const auto foundStringDigitTuple = FindLastStringDigit(line);
    const auto foundDigitTuple = FindLastDigit(line);

    if(std::get<1>(foundStringDigitTuple) > std::get<1>(foundDigitTuple))
    {
        const auto stringDigitIterator = std::find(digits.begin(), digits.end(), std::get<0>(foundStringDigitTuple));
        const auto index = (stringDigitIterator - digits.begin());
        return numbers[index];
    }
    else
    {
        return numbers[std::get<0>(foundDigitTuple)];
    }

    const std::string::const_reverse_iterator lastDigitIterator = std::find_if(line.rbegin(), line.rend(), [](const char& character){ return isdigit(character); });
    return *lastDigitIterator;
}

unsigned AccumulateNumbersFromLines(const std::vector<std::string>& lines)
{
    unsigned accumulate = 0u;
    for(const auto& line : lines)
    {
        std::stringstream ss;
        ss << GetFirstDigitFromLine(line) << GetLastDigitFromLine(line);

        const auto string = ss.str();
        accumulate += std::stoi(string);
    }

    return accumulate;
}

void FirstPart()
{
    std::cout << "= First part =\n";
    Utils::FileIo fileIo("../input.txt");
    const std::vector<std::string> filePerLine = fileIo.GetFileContent();

    std::cout << AccumulateNumbersFromLines(filePerLine) << "\n";
}

void SecondPart()
{
    std::cout << "= Second part =\n";
    Utils::FileIo fileIo("../input.txt");
    const auto filePerLine = fileIo.GetFileContent();

    std::cout << AccumulateNumbersFromLines(filePerLine) << "\n";
}

int main()
{
    std::cout << "Advent of code main\n";

    // FirstPart();
    SecondPart();

    return 0;
}