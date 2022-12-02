#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "Utils/FileIO.hpp"

typedef std::vector<std::string> Round;
typedef std::vector<Round> StrategyGuide;

const std::map<std::string, std::vector<std::string>> results { {"A", {"Z", "X", "Y"}}, {"B", {"X", "Y", "Z"}}, {"C", {"Y", "Z", "X"}}};

StrategyGuide GetEncryptedStrategyGuideFromFile()
{
    Utils::FileIo fileIo("../src/day2-input.txt");
    const auto filePerLine = fileIo.GetFileContent();
    constexpr size_t spaceOffset = 1;

    StrategyGuide strategyGuide;
    for(const auto& line : filePerLine)
    {
        const auto position = line.find(' ');
    
        const auto opponentAction = line.substr(0, position);
        const auto userAction = line.substr(position + spaceOffset, line.length());
        strategyGuide.push_back({opponentAction, userAction});
    }

    return strategyGuide;
}

bool ShouldUserWin(const std::string& user)
{
    return user == "Z";
}

bool ShouldBeDraw(const std::string& opponent, const std::string& user)
{
    return user == "Y";
}

std::size_t GetUserChoiseIndex(const std::string& result)
{
    if(result == "X")
    {
        return 0;
    } 
    else if(result == "Y")
    {
        return 1;
    }
    else if(result == "Z")
    {
        return 2;
    }
}

std::string GetUsersChoise(const std::string& opponent, const std::string& requiredRoundResult)
{
    const auto userChoiseIndex = GetUserChoiseIndex(requiredRoundResult);
    return results.at(opponent)[userChoiseIndex];
}

int GetUserChoisePoints(const std::string& opponent, const std::string& requiredRoundResult)
{
    const auto choise = GetUsersChoise(opponent, requiredRoundResult);

    if(choise == "X")
    {
        return 1;
    } 
    else if(choise == "Y")
    {
        return 2;
    }
    else if(choise == "Z")
    {
        return 3;
    }
}

int GetMatchResultPoints(const std::string& opponent, const std::string& user)
{
    if(ShouldUserWin(user))
    {
        return 6;
    }
    else if(ShouldBeDraw(opponent, user))
    {
        return 3;
    }

    return 0;
}

int GetRoundResultPoints(const std::string& opponent, const std::string& user)
{
    int matchResult = GetMatchResultPoints(opponent, user);
    matchResult += GetUserChoisePoints(opponent, user);

    return matchResult;
}

int GetRoundScore(const Round& round)
{
    const auto& opponent = round[0];
    const auto& user = round[1];
    return GetRoundResultPoints(opponent, user);
}

void GetResult()
{
    const auto strategyGuide = GetEncryptedStrategyGuideFromFile();

    int totalScore = 0;
    for(const auto& round : strategyGuide)
    {
        totalScore += GetRoundScore(round);
    }
    std::cout << "\tTotal Score: " << std::to_string(totalScore) << "\n";
}

int main()
{
    std::cout << "Advent of code main\n";

    GetResult();

    return 0;
}