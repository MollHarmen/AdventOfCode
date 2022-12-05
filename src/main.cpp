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

void AddSectionsOfLine(const std::string& line, std::vector<Section>& sections)
{
    constexpr size_t commaOffset = 1;
    std::string firstSection = line.substr(0, line.find(','));
    std::string secondSection = line.substr(line.find(',')+commaOffset, line.size());

    sections.push_back(GetSectionFromString(firstSection));
    sections.push_back(GetSectionFromString(secondSection));
}

SectionPair GetSectionPairFromLine(const std::string& line, std::vector<Section>& sections)
{
    constexpr size_t commaOffset = 1;
    std::string firstSection = line.substr(0, line.find(','));
    std::string secondSection = line.substr(line.find(',')+commaOffset, line.size());

    return std::make_pair(firstSection, secondSection);
}

std::vector<Section> GetSectionsFromFile()
{
    Utils::FileIo fileIo("../src/day4-input.txt");
    const auto filePerLine = fileIo.GetFileContent();

    std::vector<Section> sections;
    for(const auto& line : filePerLine)
    {
        AddSectionsOfLine(line, sections);
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

void FirstPart()
{
    std::cout << "= First part =\n";
    const auto sections = GetSectionsFromFile();
    for(const auto& section : sections)
    {
        for(const auto& compareSection : sections)
        {
            if(compareSection != section)
            {
                const bool isInSectionRange = DoesSectionFitWithinOtherSection(section, compareSection);
                
                if(isInSectionRange)
                {   
                    std::cout << std::get<0>(section) << "-" << std::get<1>(section) << " AND ";
                    std::cout << std::get<0>(compareSection) << "-" << std::get<1>(compareSection);
                    std::cout << " = Yes\n";
                }
           }
        }
    }
}

void SecondPart()
{
    std::cout << "= Second part =\n";
}

int main()
{
    std::cout << "Advent of code main\n";

    FirstPart();
    SecondPart(); 

    return 0;
}