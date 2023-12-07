#include <algorithm>
#include <iostream>
#include <sstream>
#include <vector>

#include "Utils/FileIO.hpp"
#include "Utils/StringHandling.hpp"

const std::string CharacterValueOrder = "23456789TJQKA";
struct
{
    bool operator()(const char& left, const char& right) const 
    {
        return CharacterValueOrder.find(left) > CharacterValueOrder.find(right); 
    }
} cardCompare;

const std::vector<char> patternChars {'X', 'Y', 'Z', 'A', 'B'};



struct Hand
{
    std::vector<char> mHand;
    const int mBid;
    const std::string mPattern;

    Hand(std::vector<char> hand, const int bid)
    : mHand(hand)
    , mBid(bid)
    , mPattern(GetHandPattern())
    {
        std::sort(mHand.begin(), mHand.end(), [&hand](const char& left, const char& right)
        {
            if(std::count(hand.begin(), hand.end(), left) == std::count(hand.begin(), hand.end(), right))
            {
                return CharacterValueOrder.find(left) > CharacterValueOrder.find(right);
            }

            return std::count(hand.begin(), hand.end(), left) > std::count(hand.begin(), hand.end(), right);
        });
    }
    
    void Print() const
    {
        const std::string hand(mHand.begin(), mHand.end());
        std::cout << hand << " " << mPattern << " " << mBid << "\n";
    }

    std::string GetHandPattern() const
    {
        char previousChar = *mHand.begin();
        size_t patternCharacterIndex = 0;
        std::stringstream pattern;

        for(auto cardIterator = mHand.begin(); cardIterator != mHand.end(); ++cardIterator)
        {
            if(*cardIterator == previousChar)
            {
                pattern << patternChars[patternCharacterIndex];
            }
            else
            {
                pattern << patternChars[++patternCharacterIndex];
            }
            previousChar = *cardIterator;
        }

        return pattern.str();
    }

    char GetHighestCard()
    {
        return mHand[0];
    }

    bool operator<(const Hand& other)
    {
        return mPattern < other.mPattern;
    }
};

typedef std::vector<Hand> Hands;

std::vector<char> SplitStringIntoChars(const std::string& input)
{
    std::vector<char> characters;
    for(const auto& character : input)
    {
        characters.push_back(character);
    }

    return characters;
}

Hand GetHandFromLine(const std::string& line)
{
    std::vector<std::string> split;
    StringHandling::SplitStringOnCharacter(line, split, ' ');

    return {SplitStringIntoChars(split[0]), std::stoi(split[1])};
}

Hands GetHandsFromInput(const std::vector<std::string>& input)
{
    Hands hands;
    for(const auto& line : input)
    {
        hands.push_back(GetHandFromLine(line));
    }

    return hands;
}


void FirstPart()
{
    std::cout << "= First part =\n";
    Utils::FileIo fileIo("C:/projects/AdventOfCode/src/input.txt");
    const std::vector<std::string> filePerLine = fileIo.GetFileContent();
    
    Hands hands = GetHandsFromInput(filePerLine);
    for(const auto& hand : hands)
    {
        hand.Print();
    }
    // std::sort(hands.begin(), hands.end(), cardCompare);
}
void SecondPart()
{
    std::cout << "= Second part =\n";
    Utils::FileIo fileIo("C:/projects/AdventOfCode/src/input.txt");
    const std::vector<std::string> filePerLine = fileIo.GetFileContent();

}

int main()
{
    std::cout << "Advent of code main\n";

    FirstPart();
    // SecondPart();

    return 0;
}