#include <array>
#include <iostream>
#include <sstream>
#include <tuple>
#include <unordered_map>
#include <vector>

#include "Utils/FileIO.hpp"
#include "Utils/StringHandling.hpp"

std::vector<std::string> GetInitializationSequence(const std::vector<std::string> filePerLine)
{
    std::vector<std::string> initializationSequence;

    for(const auto& line : filePerLine)
    {
        std::vector<std::string> splitted;
        StringHandling::SplitStringOnCharacter(line, splitted, ',');
        initializationSequence.insert(initializationSequence.end(), splitted.begin(), splitted.end());
    }

    return initializationSequence;
}

int GetAsciiCode(const char& character)
{
    return static_cast<int>(character);
}

int CalculateHash(const std::string& step)
{
// Determine the ASCII code for the current character of the string.
// Increase the current value by the ASCII code you just determined.
// Set the current value to itself multiplied by 17.
// Set the current value to the remainder of dividing itself by 256.

    int value = 0;
    for(const char& character : step)
    {
        value += GetAsciiCode(character);
        value *= 17;
        value = value % 256;
    }
    return value;
}

void FirstPart()
{
    std::cout << "= First part =\n";
    Utils::FileIo fileIo("C:/projects/AdventOfCode/src/input.txt");
    const std::vector<std::string> filePerLine = fileIo.GetFileContent();

    const std::vector<std::string> initializationSequence = GetInitializationSequence(filePerLine);

    int result = 0;
    for(const auto& step : initializationSequence)
    {
        result += CalculateHash(step);
    }
    std::cout << "Result: " << result << "\n";
}

typedef std::vector<std::tuple<std::string, int>> Box;
typedef std::array<Box, 256> Boxes;

int GetBoxNumberFromLabel(const std::string& step)
{
    return CalculateHash(step);
}

Box& GetBoxFromLabel(Boxes& boxes, const std::string& label)
{
    const auto boxNumber = GetBoxNumberFromLabel(label);
    return boxes[boxNumber];
}

bool GetLensFromBox(Box& box, const std::string& label, std::vector<std::tuple<std::string, int>>::iterator& lens)
{
    const auto iter = std::find_if(box.begin(), box.end(), [&label](const std::tuple<std::string, int> lens){ return std::get<0>(lens) == label; });
    if(iter != box.end())
    {
        lens = iter;
        return true;
    }

    return false;
}

void PerformStepOnBox(Boxes& boxes, const std::string& step)
{
    std::string label;
    std::string operation;
    int focalLength = 0;

    [&](const std::string& step)
    {
        auto position = step.find("=");
        if(position != step.npos)
        {
            const int length = step.length();
            const int count = length - position;
            focalLength = std::stoi(step.substr(position+1, count-1));
        }
        else
        {
            position = step.find("-");
        }

        label = step.substr(0, position);
        operation = step.substr(position, 1);
    }(step);

    Box& box = GetBoxFromLabel(boxes, label);

    if(operation == "=")
    {
        std::vector<std::tuple<std::string, int>>::iterator lens;
        if(GetLensFromBox(box, label, lens))
        {
            std::get<1>(*lens) = focalLength;
        }
        else
        {
            box.push_back(std::make_tuple(label, focalLength));
        }
    }
    else
    {
        const auto iter = std::find_if(box.begin(), box.end(), [&label](const std::tuple<std::string, int> lens){ return std::get<0>(lens) == label; });
        if(iter != box.end())
        {
            box.erase(iter);
        }
    }
}

int CalculateResult(const Boxes& boxes)
{
    int result = 0;
    for(size_t boxIndex = 0u; boxIndex != boxes.size(); ++boxIndex)
    {
        const Box& box = boxes[boxIndex];
        size_t lensIndex = 1;
        for(auto lensIterator = box.begin(); lensIterator != box.end(); ++lensIterator)
        {
            result += (boxIndex + 1) * lensIndex * std::get<1>(*lensIterator);
            ++lensIndex;
        }
    }

    return result;
}

void SecondPart()
{
    std::cout << "= Second part =\n";
    Utils::FileIo fileIo("C:/projects/AdventOfCode/src/input.txt");
    const std::vector<std::string> filePerLine = fileIo.GetFileContent();

    const std::vector<std::string> initializationSequence = GetInitializationSequence(filePerLine);
    Boxes boxes;


    for(const auto& step : initializationSequence)
    {
        PerformStepOnBox(boxes, step);
    }
    const int result = CalculateResult(boxes);
    std::cout << "Result: " << result << "\n";
}

int main()
{
    std::cout << "Advent of code main\n";

    // FirstPart();
    SecondPart();

    return 0;
}