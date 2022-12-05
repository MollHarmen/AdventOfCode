#include <iostream>
#include <stack>
#include <string>
#include <vector>

#include "Utils/FileIO.hpp"

typedef std::stack<char, std::vector<char>> Stack;
typedef std::vector<Stack> Cargo;

void MoveFromStackToStack(const int from, const int to, Cargo& cargo)
{
    cargo[to].push(cargo[from].top());
    cargo[from].pop();
}

void MoveMultipleFromStackToStack(const int numberOfMoves, const int from, const int to, Cargo& cargo)
{
    Stack temp;

    for(int move = 0; move != numberOfMoves; ++move)
    {
        temp.push(cargo[from].top());
        cargo[from].pop();
    }

    for(size_t index = temp.size(); index != 0; --index)
    {
        cargo[to].push(temp.top());
        temp.pop();
    }
}

Cargo GetStartingPosition()
{
    Cargo startPosition { Stack(), Stack({'B','Z','T'}), Stack({'V','H','T','D','N'}), Stack({'B','F','M','D'}), Stack({'T', 'J', 'G','W','V','Q','L'}), 
    Stack({'W','D','G','P','V','F','Q','M'}), Stack({'V','Z','Q','G','H','F','S'}), Stack({'Z', 'S', 'N', 'R', 'L', 'T', 'C', 'W'}), 
    Stack({'Z', 'H', 'W', 'D', 'J', 'N', 'R', 'M'}),Stack({'M', 'Q', 'L', 'F', 'D', 'S'})};

    // Cargo startPosition { Stack(), Stack({'Z', 'N'}), Stack({'M', 'C', 'D'}), Stack({'P'}) };
    
    return startPosition;
}

void PrintTopOfCargo(const Cargo& cargo)
{
    for(const auto& stack : cargo)
    {
        if(!stack.empty())
        {
            std::cout << stack.top();
        }
    }
    std::cout << "\n";
}

void ExecuteMoveCommand(const std::string& moveCommand, Cargo& cargo)
{
    const std::string moveKey = "move ";
    const std::string fromKey = "from ";
    const std::string toKey = "to ";

    const auto movePosition = moveCommand.find(moveKey) + moveKey.length();
    const auto fromPosition = moveCommand.find(fromKey) + fromKey.length();
    const auto toPosition = moveCommand.find(toKey) + toKey.length();

    const int numberOfMoves = std::stoi(moveCommand.substr(movePosition, fromPosition-1));
    const int from = std::stoi(moveCommand.substr(fromPosition, toPosition-1));
    const int to = std::stoi(moveCommand.substr(toPosition, moveCommand.length()));

    MoveMultipleFromStackToStack(numberOfMoves, from, to, cargo);
}

void FirstPart()
{
    auto cargo = GetStartingPosition();

    Utils::FileIo fileIo("../src/day5-input.txt");
    const auto filePerLine = fileIo.GetFileContent();  
    for(const auto& line : filePerLine)
    {
        ExecuteMoveCommand(line, cargo);
    }  

    PrintTopOfCargo(cargo);
}

void SecondPart()
{
}

int main()
{
    std::cout << "Advent of code main\n";

    FirstPart();
    SecondPart(); 

    return 0;
}