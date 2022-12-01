#include <algorithm>
#include <numeric>
#include <iostream>
#include <string>
#include <vector>

#include "Utils/FileIO.hpp"

struct Elf;

typedef std::vector<int> FoodItems;
typedef std::vector<Elf> ExpeditionGroup; 

struct Elf
{
    Elf(const FoodItems caloriesPerItem)
    : foodItems(caloriesPerItem)
    , totalCaloricCount(std::accumulate(foodItems.begin(), foodItems.end(), 0))
    {}

    const FoodItems foodItems;
    const int totalCaloricCount = 0;
};

ExpeditionGroup GetExpeditionGroupFromFile()
{
    Utils::FileIo fileIo("../day1-input.txt");
    const auto filePerLine = fileIo.GetFileContent();

    ExpeditionGroup expeditionGroup;
    FoodItems foodItemsFromElf;
    for(const auto& line : filePerLine)
    {   
        if(line.length() > 0)
        {
            foodItemsFromElf.push_back(std::stoi(line));
        }
        else
        {
            expeditionGroup.push_back({foodItemsFromElf});
            foodItemsFromElf.clear();
        }
    }

    if(foodItemsFromElf.size() > 0) // When last item is on last line of file
    {
        expeditionGroup.push_back({foodItemsFromElf});
    }

    return expeditionGroup;
}

void FirstPart()
{
    std::cout << "= First part =\n";
    const ExpeditionGroup expeditionGroup = GetExpeditionGroupFromFile();
    const auto elfIterator = *std::max_element(expeditionGroup.begin(), expeditionGroup.end(), [](const Elf& a, const Elf& b) {
        return a.totalCaloricCount < b.totalCaloricCount;
    });

    std::cout << "\tMax amount of calories: " << std::to_string(elfIterator.totalCaloricCount) << " \n";
}

FoodItems ConvertExpeditionGroupToFoodItems(const ExpeditionGroup& expeditionGroup)
{
    FoodItems foodItems;
    for(const auto& elf : expeditionGroup)
    {
        foodItems.push_back(elf.totalCaloricCount);
    }

    return foodItems;
}

FoodItems GetTopThreeCaloricValuesOfExpeditionGroup(const ExpeditionGroup& expeditionGroup)
{
    
    // For some reason std::max_element for ExpeditionGroupe type returns a strange type of iterator 
    //      form which I cannot extract the caloric counts (quickly). ConvertExpeditionGroupToFoodItems is a hack solution
    auto caloriesPerElf = ConvertExpeditionGroupToFoodItems(expeditionGroup); 

    constexpr int topCount = 3;
    FoodItems counts;
    for(int elfIndex = 0; elfIndex != topCount; ++elfIndex)
    {
        const auto iterator = std::max_element(caloriesPerElf.begin(), caloriesPerElf.end());
        counts.push_back(*iterator);
        caloriesPerElf.erase(iterator);
    }   

    return counts;
}

void SecondPart()
{
    std::cout << "= Second part =\n";
    const ExpeditionGroup expeditionGroup = GetExpeditionGroupFromFile();
    const auto topThree = GetTopThreeCaloricValuesOfExpeditionGroup(expeditionGroup);
    const auto totalTopThreeCalories = std::accumulate(topThree.begin(), topThree.end(), 0);
    std::cout << "\tTop Three Total Calories: " << std::to_string(totalTopThreeCalories) << "\n";
}

int main()
{
    std::cout << "Advent of code main\n";

    // FirstPart();
    SecondPart(); 

    return 0;
}