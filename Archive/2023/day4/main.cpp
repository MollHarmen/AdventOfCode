#include <iostream>
#include <string>
#include <vector>

#include "Utils/FileIO.hpp"
#include "Utils/StringHandling.hpp"

struct Card
{
    std::vector<std::string> mWinningNumbers;
    std::vector<std::string> mNumbers;
    int copies = 0;

    bool operator==(const Card& other)
    {
        return other.mWinningNumbers == mWinningNumbers && other.mNumbers == mNumbers;
    }

    void Print()
    {
        std::cout << "winning: ";
        for(const auto& number : mWinningNumbers)
        {
            std::cout << number << " - ";
        }
        std::cout << "numbers: ";
        for(const auto& number : mNumbers)
        {
            std::cout << number << " - ";
        }

        std::cout << "copies: " << copies << "\n";
    }
};

std::vector<Card> GetCardsFromLines(const std::vector<std::string>& lines)
{
    std::vector<Card> cards;
    for(const auto& line : lines)
    {
        const auto winningNumbersPosition = line.find(": ") + 2;
        const auto deviderPosition = line.find(" | ");
        const auto winningNumbers = line.substr(winningNumbersPosition, (deviderPosition - winningNumbersPosition));
        const auto numbersPosition = deviderPosition + 3;
        const auto numbers = line.substr(numbersPosition, line.length() - numbersPosition);

        Card card;
        StringHandling::SplitStringOnCharacter(winningNumbers, card.mWinningNumbers, ' ');
        StringHandling::SplitStringOnCharacter(numbers, card.mNumbers, ' ');
        cards.push_back(card);
    }

    return cards;
}

int CalcualteScore(const std::vector<Card>& cards)
{
    int score = 0;

    for(const auto& card : cards)
    {
        int cardScore = 0;
        for(const auto& number : card.mNumbers)
        {
            if(number != " " && number != "")
            {
                const auto numberIterator = std::find(card.mWinningNumbers.begin(), card.mWinningNumbers.end(), number);
                if(numberIterator != card.mWinningNumbers.end())
                {
                    if(cardScore == 0)
                    {
                        ++cardScore;
                    }
                    else
                    {
                        cardScore = cardScore * 2;
                    }
                }
            }
        }
        score += cardScore;
    }

    return score;
}

int CalculateNumberOfCards(std::vector<Card>& cards)
{
    for(const auto& card : cards)
    {
        int matchingNumbers = 0;
        for(const auto& number : card.mNumbers)
        {
            if(number != " " && number != "")
            {
                const auto numberIterator = std::find(card.mWinningNumbers.begin(), card.mWinningNumbers.end(), number);
                if(numberIterator != card.mWinningNumbers.end())
                {
                    ++matchingNumbers;
                }
            }
        }

        const auto cardPosition = std::find(cards.begin(), cards.end(), card) + 1;
        for(int cardCount = 1+card.copies; cardCount != 0; --cardCount)
        {
            auto cardIterator = cardPosition;
            for(size_t cardIndex = 0; cardIndex != matchingNumbers; ++cardIndex, ++cardIterator)
            {
                ++((*cardIterator).copies);
            }
        }
    }

    int numebrOfCards = cards.size();
    for(const auto& card : cards)
    {
        numebrOfCards += card.copies;
    }

    return numebrOfCards;
}

void FirstPart()
{
    std::cout << "= First part =\n";
    Utils::FileIo fileIo("../input.txt");
    const std::vector<std::string> filePerLine = fileIo.GetFileContent();

    std::vector<Card> cards = GetCardsFromLines(filePerLine);
    const auto totalScore = CalcualteScore(cards);

    std::cout << "Total score: " << totalScore << "\n";
}

void SecondPart()
{
    std::cout << "= Second part =\n";
    Utils::FileIo fileIo("../input.txt");
    const std::vector<std::string> filePerLine = fileIo.GetFileContent();

    std::vector<Card> cards = GetCardsFromLines(filePerLine);
    const auto numberOfCards = CalculateNumberOfCards(cards);

    std::cout << "Number of cards: " << numberOfCards << "\n";
}

int main()
{
    std::cout << "Advent of code main\n";

    FirstPart();
    SecondPart();

    return 0;
}