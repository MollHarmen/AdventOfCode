#include "Utils/FileIO.hpp"
#include "Utils/StringHandling.hpp"

#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>
#include <tuple>
#include <vector>

typedef std::vector<std::tuple<int,int>> PageOrderRules;

std::tuple<PageOrderRules, std::vector<std::vector<int>>> GetInputsFromInputFile() {
    auto input_file = Utils::FileIo("input.txt");
    const auto file_content = input_file.GetFileContent();

    size_t line_index = 0u;
    PageOrderRules page_order_rules;

    while(file_content[line_index] != "" && line_index != file_content.size()) {
        std::vector<std::string> order;
        StringHandling::SplitStringOnCharacter(file_content[line_index++], order, '|');
        page_order_rules.push_back(std::make_tuple(std::stoi(order[0]), std::stoi(order[1])));
    }

    ++line_index;
    std::vector<std::vector<int>> updates;
    while(line_index != file_content.size()) {
        std::vector<int> update;
        std::vector<std::string> update_strings;
        StringHandling::SplitStringOnCharacter(file_content[line_index++], update_strings, ',');

        for(const auto& number_string : update_strings) {
            update.push_back(std::stoi(number_string));
        }
        
        updates.push_back(update);
    }

    return std::make_tuple(page_order_rules, updates);
}

PageOrderRules GetOrderingRulesForNumber(const PageOrderRules& page_order_rules, const int number) {
    PageOrderRules rules_for_number;
    for(const auto& rule : page_order_rules) {
        if(std::get<0>(rule) == number) {
            rules_for_number.push_back(rule);
        }
    }
    
    return rules_for_number;
}

bool IsUpdateInRightOrder(const std::vector<int>& update, const PageOrderRules& page_order_rules) {
    for(auto number_iterator = update.begin(); number_iterator != update.end(); ++number_iterator) {
        const auto ordering_rules = GetOrderingRulesForNumber(page_order_rules, *number_iterator);
        for(const auto& rule : ordering_rules) {
            const auto found_iterator = std::find(update.begin(), update.end(), std::get<1>(rule));
            if(found_iterator != update.end()) {
                if(std::distance(update.begin(), number_iterator) > std::distance(update.begin(), found_iterator)) {
                    return false;
                }
            }
        }
    }

    return true;
}

void FirstPart() {
    auto [page_order_rules, updates] = GetInputsFromInputFile();
    int result = 0;
    for(const auto& update : updates) {
        if(IsUpdateInRightOrder(update, page_order_rules))
        {
            const auto middle_number = update[(update.size() -1) / 2];
            // std::cout << middle_number << "\n";
            result += middle_number;
        }
    }

    std::cout << result << "\n";
}

std::vector<int> CorrectUpdate(const std::vector<int>& update, const PageOrderRules& page_order_rules) {
    std::vector<int> result = update;
    for(auto number_iterator = update.begin(); number_iterator != update.end(); ++number_iterator) {
        const auto ordering_rules = GetOrderingRulesForNumber(page_order_rules, *number_iterator);
        for(const auto& rule : ordering_rules) {
            const auto found_iterator = std::find(update.begin(), update.end(), std::get<1>(rule));
            if(found_iterator != update.end()) {
                if(std::distance(update.begin(), number_iterator) > std::distance(update.begin(), found_iterator)) {
                    auto left = std::find(result.begin(), result.end(), *number_iterator);
                    auto right = std::find(result.begin(), result.end(), *found_iterator);
                    std::swap(*left, *right);
                }
            }
        }
    }

    if(IsUpdateInRightOrder(result, page_order_rules)) {
        return result;
    }

    return CorrectUpdate(result, page_order_rules);
}

void SecondPart() {
    auto [page_order_rules, updates] = GetInputsFromInputFile();
    int result = 0;
    for(const auto& update : updates) {
        if(!IsUpdateInRightOrder(update, page_order_rules))
        {
            const auto corrected_update = CorrectUpdate(update, page_order_rules);
            const auto middle_number = corrected_update[(corrected_update.size() -1) / 2];
            // std::cout << middle_number << "\n";
            result += middle_number;
        }
    }
    std::cout << result << "\n";
}

int main()
{
    std::cout << "Advent of code main\n";

    // FirstPart();
    SecondPart();

    return 0;
} 