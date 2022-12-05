#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>
#include <utility>

#include "Utils/FileIO.hpp"

typedef std::pair<int, int> Section;
typedef std::pair<Section, Section> SectionPair;

Section GetSectionFromString(const std::string& input)
{
    constexpr size_t dashOffset = 1;
    const int min = std::stoi(input.substr(0, input.find('-')));
    const int max = std::stoi(input.substr(input.find('-')+dashOffset, input.size()));

    return std::make_pair(min, max);
}

SectionPair GetSectionPairFromLine(const std::string& line)
{
    constexpr size_t commaOffset = 1;
    std::string firstSection = line.substr(0, line.find(','));
    std::string secondSection = line.substr(line.find(',')+commaOffset, line.size());

    return std::make_pair(GetSectionFromString(firstSection), GetSectionFromString(secondSection));
}

std::vector<SectionPair> GetSectionPairsFromFile()
{
    Utils::FileIo fileIo("../src/day4-input.txt");
    const auto filePerLine = fileIo.GetFileContent();

    std::vector<SectionPair> sections;
    for(const auto& line : filePerLine)
    {
        sections.push_back(GetSectionPairFromLine(line));
    }

    return sections;
}

bool IsInRange(const int number, const int min, const int max)
{
    return number >= min && number <= max;
}

bool DoesSectionFitWithinOtherSection(const Section& section, const Section& other)
{
    const bool isLowerSectionInRange = IsInRange(std::get<0>(section), std::get<0>(other), std::get<1>(other));
    const bool isUpperSectionInRange = IsInRange(std::get<1>(section), std::get<0>(other), std::get<1>(other));

    return isLowerSectionInRange && isUpperSectionInRange;
}

bool IsSectionOverlappingWithinOtherSection(const Section& section, const Section& other)
{
    const bool isLowerSectionInRange = IsInRange(std::get<0>(section), std::get<0>(other), std::get<1>(other));
    const bool isUpperSectionInRange = IsInRange(std::get<1>(section), std::get<0>(other), std::get<1>(other));

    return isLowerSectionInRange || isUpperSectionInRange;
}

int GetFullyOverlapCountForSectionPairs(const std::vector<SectionPair>& sectionPairs)
{
    int count = 0;

    for(const auto& sectionPair : sectionPairs)
    {
        const auto& firstSection = std::get<0>(sectionPair);
        const auto& secondSection = std::get<1>(sectionPair);

        const bool isInSectionRange = DoesSectionFitWithinOtherSection(firstSection, secondSection) || DoesSectionFitWithinOtherSection(secondSection, firstSection);

        if(isInSectionRange)
        {   
            ++count;
        }     
    }

    return count;
}

int GetOverlapCountForSectionPairs(const std::vector<SectionPair>& sectionPairs)
{
    int count = 0;

    for(const auto& sectionPair : sectionPairs)
    {
        const auto& firstSection = std::get<0>(sectionPair);
        const auto& secondSection = std::get<1>(sectionPair);

        const bool isInSectionRange = IsSectionOverlappingWithinOtherSection(firstSection, secondSection) || IsSectionOverlappingWithinOtherSection(secondSection, firstSection);

        if(isInSectionRange)
        {   
            ++count;
        }     
    }

    return count;
}

void FirstPart()
{
    std::cout << "= First part =\n";
    const auto sectionPairs = GetSectionPairsFromFile();
    const int overlapCount = GetFullyOverlapCountForSectionPairs(sectionPairs);

    std::cout << "\tOverlap count: " << std::to_string(overlapCount) << "\n";
}

void SecondPart()
{
    std::cout << "= Second part =\n";
    const auto sectionPairs = GetSectionPairsFromFile();
    const int overlapCount = GetOverlapCountForSectionPairs(sectionPairs);

    std::cout << "\tOverlap count: " << std::to_string(overlapCount) << "\n";
}

int main()
{
    std::cout << "Advent of code main\n";

    FirstPart();
    SecondPart(); 

    return 0;
}