#include <algorithm>
#include <iostream>
#include <map>
#include <numeric>
#include <string>
#include <utility>
#include <vector>

#include "Utils/FileIO.hpp"

typedef std::vector<std::pair<std::string, int>> InstructionSet;

struct Cpu
{
    Cpu(const InstructionSet& instructionSet)
    : mInstructionSet(instructionSet)
    {}

    void Tick()
    {
        if(additionToPerform == 0)
        {   
            const auto& instruction = std::get<0>(mInstructionSet[instructionPointer]);
            if(instruction != "noop")
            {
                additionToPerform = std::get<1>(mInstructionSet[instructionPointer]);
            }
            ++instructionPointer;
        }
        else
        {
            registerX += additionToPerform;
            additionToPerform = 0;
        }
        ++cycles;
    }

    const InstructionSet& mInstructionSet;
    int registerX = 1;
    int cycles = 1; 
    int instructionPointer = 0;
    int additionToPerform = 0;
};

struct Crt
{
    Crt(Cpu& cpu)
    : mCpu(cpu)
    , screen(std::string(240, '.'))
    {
    }

    void Tick()
    {
        std::cout << "\tDrawPos.: " << drawPosition << "\treg. X: " << mCpu.registerX << "\tDrawPos. % 40: " << drawPosition % 40 << "\n";
        if((drawPosition % 40) == mCpu.registerX-1 || (drawPosition % 40) == mCpu.registerX || (drawPosition % 40) == mCpu.registerX+1)
        {
            screen[drawPosition++] = '#';
        }
        else
        {
            screen[drawPosition++] = '.';
        }
    }
    
    void Print()
    {
        for(int index = 0; index != screen.length(); ++index)
        {
            if(index % 40 == 0)
            {
                std::cout << "\n";
            }
            std::cout << screen[index];
        }
    }

    std::string screen;
    int drawPosition = 0;
    Cpu& mCpu;
};

InstructionSet GetInstructionSetFromFile()
{
    Utils::FileIo fileIo("../src/day10-input.txt");
    const auto filePerLine = fileIo.GetFileContent();

    InstructionSet instructions;
    for(const auto& line : filePerLine)
    {
        if(line.find("noop") != line.npos)
        {
            instructions.push_back(std::make_pair("noop", 0));
        }
        else
        {
            const std::string instruction = line.substr(0, line.find(" "));
            const int value = std::stoi(line.substr(line.find(" "), line.length()));
            instructions.push_back(std::make_pair(instruction, value));
        }
    }

    return instructions;
}

void FirstPart()
{
    std::cout << "= First part =\n";
    InstructionSet instructions = GetInstructionSetFromFile();
    Cpu cpu(instructions);

    std::vector<int> samplesAtCycle {20, 60, 100, 140, 180, 220};
    std::vector<int> samples;
    for(const auto& cyclesToSampleAt : samplesAtCycle)
    {
        for(int clockTicks = cpu.cycles; clockTicks != cyclesToSampleAt; ++clockTicks)
        {
            cpu.Tick();
        }
        samples.push_back(cpu.registerX * cyclesToSampleAt);
    }    
}

void SecondPart()
{
    std::cout << "= Second part =\n";
    InstructionSet instructions = GetInstructionSetFromFile();
    Cpu cpu(instructions);
    Crt crt(cpu);

    for(int numberOfCycles = 0; numberOfCycles != 240; ++numberOfCycles)
    {
        crt.Tick();
        cpu.Tick();
        crt.Print();
        std::cout << "===========\n";
    }
}

int main()
{
    std::cout << "Advent of code main\n";

    FirstPart();
    SecondPart(); 

    return 0;
}