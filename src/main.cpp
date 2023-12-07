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
const std::string patternString(patternChars.rbegin(), patternChars.rend());

bool CompareCharacterValuesOfString(const std::string& left, const std::string& right, const std::string& characterValues)
{
    for(size_t compareIndex = 0; compareIndex != left.size(); ++compareIndex)
    {
        if (characterValues.find(left[compareIndex]) != characterValues.find(right[compareIndex]))
        {
            const auto leftValue = characterValues.find(left[compareIndex]);
            const auto rightValue = characterValues.find(right[compareIndex]);
            return leftValue < rightValue;
        }
    }
    return false;
}

struct Hand
{
    std::vector<char> mHand;
    std::vector<char> mSortedHand;
    int mBid;
    std::string mPattern;

    Hand(std::vector<char> hand, const int bid)
    : mHand(hand)
    , mSortedHand(hand)
    , mBid(bid)
    {
        std::sort(mSortedHand.begin(), mSortedHand.end(), [&hand](const char& left, const char& right)
        {
            if(std::count(hand.begin(), hand.end(), left) == std::count(hand.begin(), hand.end(), right))
            {
                return CharacterValueOrder.find(left) > CharacterValueOrder.find(right);
            }

            return std::count(hand.begin(), hand.end(), left) > std::count(hand.begin(), hand.end(), right);
        });

        mPattern = GetHandPattern();
    }
    
    void Print() const
    {
        const std::string hand(mHand.begin(), mHand.end());
        std::cout << hand << " " << mPattern << " " << mBid;
    }

    std::string GetHandPattern() const
    {
        char previousChar = *mSortedHand.begin();
        size_t patternCharacterIndex = 0;
        std::stringstream pattern;

        for(auto cardIterator = mSortedHand.begin(); cardIterator != mSortedHand.end(); ++cardIterator)
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
};

struct 
{
    bool operator()(const Hand& left, const Hand& right) const
    {
        if(left.mPattern == right.mPattern)
        {
            for(size_t compareIndex = 0; compareIndex != left.mHand.size(); ++compareIndex)
            {
                const auto leftValue = CharacterValueOrder.find(left.mHand[compareIndex]);
                const auto rightValue = CharacterValueOrder.find(right.mHand[compareIndex]);
                if(leftValue != rightValue)
                {
                    return (leftValue < rightValue);
                }
            }
            return false;
        }

        return CompareCharacterValuesOfString(left.mPattern, right.mPattern, patternString);
    }
} handCompare;

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

unsigned long long CalculateValue(const Hands& hands)
{
    int multiplier = 1;
    unsigned long long value = 0;
    for(const auto& hand : hands)
    {
        value += (multiplier * hand.mBid);
        
        hand.Print();
        std::cout << " - \t" << hand.mBid << " * " << multiplier << " - \t" << value << "\n";
        
        ++multiplier;
    }

    return value;
}

void FirstPart()
{
    std::cout << "= First part =\n";
    Utils::FileIo fileIo("C:/projects/AdventOfCode/src/input.txt");
    const std::vector<std::string> filePerLine = fileIo.GetFileContent();
    
    Hands hands = GetHandsFromInput(filePerLine);
    std::sort(hands.begin(), hands.end(), handCompare);

    std::cout << "Value of hands: \n" << CalculateValue(hands) << "\n";
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