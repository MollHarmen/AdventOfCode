#include "Utils/FileIO.hpp"
#include "Utils/StringHandling.hpp"

#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

template<typename Type>
bool IsSafe(const Type& input) {
    const auto is_declining = (input[0] - input[1]) < 0;
    for(size_t index = 0; index != input.size() -1; ++index) {
        const auto diff = input[index] - input[index+1];
        if(is_declining && diff > 0 || !is_declining && diff < 0) {
            return false;
        }

        if(std::abs(diff) > 3 || std::abs(diff) == 0) {
            return false;
        }
    }
    return true;
}

void Print(const std::vector<int>& input)
{
    for(const auto& thing : input) {
        std::cout << thing;
    }
    std::cout << "\n";
}

template<typename Type>
bool IsSafeWithDampener(const Type& input) {
    const auto is_declining = (input[0] - input[1]) < 0;
    for(size_t index = 0; index != input.size() -1; ++index) {
        const auto diff = input[index] - input[index+1];
        if((is_declining && diff > 0 || !is_declining && diff < 0) || (std::abs(diff) > 3 || std::abs(diff) == 0)) {
            auto erase_current = input;
            auto erase_next = input;
            erase_current.erase(erase_current.begin() + index);
            erase_next.erase(erase_next.begin() + index+1);            

            if(index == 0)
            {
                return IsSafe(erase_current) || IsSafe(erase_next);
            }

            auto erase_previous = input;
            erase_previous.erase(erase_previous.begin() + index-1);

            return IsSafe(erase_current) || IsSafe(erase_next) || IsSafe(erase_previous);
        }
    }
    return true;
}

void FirstPart() {
    auto input_file = Utils::FileIo("input.txt");
    const auto file_content = input_file.GetFileContent();

    std::vector<std::vector<int>> unusual_data;
    int safe_count = 0;

    for(const auto& line : file_content)
    {
        std::vector<std::string> string_numbers;
        const auto number_of_columns = StringHandling::SplitOnString(line, string_numbers, " ");

        std::vector<int> numbers;
        for(const auto& string_number : string_numbers)
        {
            numbers.push_back(std::stoi(string_number));
        }
        
        if(IsSafe(numbers))
        {
            ++safe_count;
        }
    }

    std::cout << "Safe Count: " << safe_count << "\n";
}

void SecondPart() {
    auto input_file = Utils::FileIo("input.txt");
    const auto file_content = input_file.GetFileContent();

    std::vector<std::vector<int>> unusual_data;
    int safe_count = 0;

    for(const auto& line : file_content)
    {
        std::vector<std::string> string_numbers;
        const auto number_of_columns = StringHandling::SplitOnString(line, string_numbers, " ");

        std::vector<int> numbers;
        for(const auto& string_number : string_numbers)
        {
            numbers.push_back(std::stoi(string_number));
        }
        
        if(IsSafeWithDampener(numbers))
        {
            ++safe_count;
        }
    }

    std::cout << "Safe Count: " << safe_count << "\n";
}

int main()
{
    std::cout << "Advent of code main\n";

    // FirstPart();
    SecondPart();

    return 0;
}