#include "Utils/FileIO.hpp"
#include "Utils/StringHandling.hpp"

#include <algorithm>
#include <iostream>
#include <numeric>
#include <map>
#include <string>
#include <vector>

template<typename InputType>
void SortVector(InputType& input) {
    std::sort(input.begin(), input.end());
}

template<typename InputType>
std::vector<int> CalculateDiffs(const InputType& left, const InputType& right) {
    std::vector<int> results;
    for(size_t index = 0u; index != left.size(); ++index) {
        results.push_back(std::abs(left[index] - right[index]));
    }

    return results;
}

template<typename InputType>
int CalculateSimilarityScore(const InputType& left, const InputType& right) {
    std::map<int, int> cash;

    int similarity_score = 0;

    for(const auto& number : left)
    {
        if(const auto iterator = cash.find(number); iterator != cash.end()) {
            similarity_score += cash[number];
        }
        else {
            int number_count = 0;
            std::for_each(right.begin(), right.end(), [&number, &number_count](const auto& right_number){ 
                if(number == right_number) { ++number_count; }
            });

            const auto number_score = number * number_count;
            similarity_score += number_score;
            cash[number] = number_score;
        }
    }

    return similarity_score;
}

void FirstPart() {

    auto input_file = Utils::FileIo("input.txt");
    const auto file_content = input_file.GetFileContent();
    
    std::vector<int> first_list; 
    std::vector<int> second_list; 


    for(const auto& line : file_content)
    {
        std::vector<std::string> numbers;
        const auto number_of_columns = StringHandling::SplitOnString(line, numbers, "   ");

        first_list.push_back(std::stoi(numbers[0]));
        second_list.push_back(std::stoi(numbers[1]));
    }

    SortVector(first_list);
    SortVector(second_list);

    const auto diffs = CalculateDiffs(first_list, second_list);
    std::cout << "Diff accumulate: " << std::accumulate(diffs.begin(), diffs.end(), 0) << "\n";
}

void SecondPart() {
    auto input_file = Utils::FileIo("input.txt");
    const auto file_content = input_file.GetFileContent();
    
    std::vector<int> first_list; 
    std::vector<int> second_list; 

    for(const auto& line : file_content)
    {
        std::vector<std::string> numbers;
        const auto number_of_columns = StringHandling::SplitOnString(line, numbers, "   ");

        first_list.push_back(std::stoi(numbers[0]));
        second_list.push_back(std::stoi(numbers[1]));
    }

    std::cout << CalculateSimilarityScore(first_list, second_list) << "\n";
}

int main()
{
    std::cout << "Advent of code main\n";

    // FirstPart();
    SecondPart();

    return 0;
}