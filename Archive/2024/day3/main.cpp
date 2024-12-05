#include "Utils/FileIO.hpp"
#include "Utils/StringHandling.hpp"

#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

void FirstPart() {
    auto input_file = Utils::FileIo("input.txt");
    auto file_content = input_file.GetFileContent()[0];

    const std::string multiplier_string = "mul(";

    int count = 0;

    size_t found_index = file_content.find(multiplier_string);
    while(found_index != file_content.npos) {
        // std::cout << "string: " << file_content << "\n";
        file_content = file_content.substr(found_index + multiplier_string.size(), file_content.size());
        // std::cout << "Updated string: " << file_content << "\n";
        size_t closing_bracket = file_content.find(")");

        if(closing_bracket < 9) {
            const auto number_string = file_content.substr(0, closing_bracket);
            if(number_string.find(",") != number_string.npos)
            {
                std::vector<std::string> numbers;
                StringHandling::SplitOnString(number_string, numbers, ",");

                try {
                    count += (std::stoi(numbers[0]) * std::stoi(numbers[1]));
                } catch (const std::exception) {}
            }
            file_content = file_content.substr(closing_bracket+1, file_content.size());
        }

        found_index = file_content.find(multiplier_string);
    }

    std::cout << count << "\n";
}

void SecondPart() {
    auto input_file = Utils::FileIo("input.txt");
    auto file_content = input_file.GetFileContent()[0];

    const std::string multiplier_string = "mul(";
    const std::string dont_string = "don't()";
    const std::string do_string = "do()";

    int count = 0;

    size_t found_index = file_content.find(multiplier_string);
    size_t dont_index = file_content.find(dont_string);
    while(found_index != file_content.npos) {
        if(dont_index != file_content.npos && dont_index < found_index)
        {
            size_t do_index = file_content.find(do_string);
            if(do_index != file_content.npos) {
                file_content = file_content.substr(do_index + do_string.size(), file_content.size());
                found_index = file_content.find(multiplier_string);
                dont_index = file_content.find(dont_string);
            }
        }
        else {
            file_content = file_content.substr(found_index + multiplier_string.size(), file_content.size());
            size_t closing_bracket = file_content.find(")");

            if(closing_bracket < 9) {
                const auto number_string = file_content.substr(0, closing_bracket);
                if(number_string.find(",") != number_string.npos)
                {
                    std::vector<std::string> numbers;
                    StringHandling::SplitOnString(number_string, numbers, ",");

                    try {
                        count += (std::stoi(numbers[0]) * std::stoi(numbers[1]));
                    } catch (const std::exception) {}
                }
                file_content = file_content.substr(closing_bracket+1, file_content.size());
            }

            dont_index = file_content.find(dont_string);
            found_index = file_content.find(multiplier_string);
        }
    }

    std::cout << count << "\n";
}

int main()
{
    std::cout << "Advent of code main\n";

    // FirstPart();
    SecondPart();

    return 0;
} 