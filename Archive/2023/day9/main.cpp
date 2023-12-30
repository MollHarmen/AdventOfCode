#include <vector>

#include "Utils/FileIO.hpp"
#include "Utils/StringHandling.hpp"

bool AllZeros(const std::vector<int>& input)
{
    bool allZero = true;
    for(const auto& number : input)    
    {
        if(number != 0)
        {
            return false;
        }
    }

    return true;
}

int Extrapolate(const std::vector<int>& input)
{
    std::vector<int> diffs; 

    for(auto iter = input.begin() + 1; iter != input.end(); ++iter)
    {
        diffs.push_back(*iter - *(iter-1));
    }

    if(AllZeros(diffs))
    {
        return *(diffs.end() - 1);
    }
    else
    {
        const int result = Extrapolate(diffs);

        return *(diffs.end() - 1) + result;
    }
}

std::vector<int> ConvertToInts(const std::vector<std::string>& input)
{
    std::vector<int> numbers;
    for(const auto& item : input)
    {
        numbers.push_back(std::stoi(item));
    }

    return numbers;
}

void FirstPart()
{
    std::cout << "= First part =\n";
    Utils::FileIo fileIo("input.txt");
    const std::vector<std::string> filePerLine = fileIo.GetFileContent();

    int sum = 0;
    for(const auto& line : filePerLine)
    {
        std::vector<std::string> split;
        StringHandling::SplitStringOnCharacter(line, split, ' ');
        std::vector<int> numbers = ConvertToInts(split);

        const int result = Extrapolate(numbers);
        const int extrapolated = *(numbers.end() - 1) + result;
        
        sum += extrapolated;
    }

    std::cout << "result: " << sum << "\n";
}

int ExtrapolateReverse(const std::vector<int>& input)
{
    std::vector<int> diffs; 

    for(auto iter = input.begin() + 1; iter != input.end(); ++iter)
    {
        diffs.push_back(*iter - *(iter-1));
    }

    if(AllZeros(diffs))
    {
        return *(diffs.begin());
    }
    else
    {
        const int result = ExtrapolateReverse(diffs);

        return *diffs.begin() - result;
    }
}

void SecondPart()
{
    std::cout << "= Second part =\n";
    Utils::FileIo fileIo("input.txt");
    const std::vector<std::string> filePerLine = fileIo.GetFileContent();

    int sum = 0;
    for(const auto& line : filePerLine)
    {
        std::vector<std::string> split;
        StringHandling::SplitStringOnCharacter(line, split, ' ');
        std::vector<int> numbers = ConvertToInts(split);

        const int result = ExtrapolateReverse(numbers);
        std::cout << result << "\n";
        const int extrapolated = *(numbers.end() - 1) + result;
        
        sum += extrapolated;
    }

    std::cout << "result: " << sum << "\n";
}

int main()
{
    std::cout << "Advent of code main\n";

    // FirstPart();
    SecondPart();

    return 0;
}