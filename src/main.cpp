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

    int GetRegisterValueDuringCycleNumber(const int cycleNumber)
    {
        int instructionPointer = 0;
        int additionToPerform = 0;
        for(int cycles = 1; cycles != cycleNumber; ++cycles)
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
        }

        return registerX;
    }

    std::vector<int> SampleSignalStrengthAtCycleIntervals(const std::vector<int>& sampleIntervals)
    {
        std::vector<int> signalStrengthSamples;
        for(const auto& sampleInterval : sampleIntervals)
        {
            registerX = 1;
            const auto sample = GetRegisterValueDuringCycleNumber(sampleInterval);
            signalStrengthSamples.push_back(sample * sampleInterval);
        }

        return signalStrengthSamples;
    }

    const InstructionSet& mInstructionSet;
    int registerX = 1;
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
    std::vector<int> samplesAtCycle {20, 60, 100, 140, 180, 220};
    Cpu cpu(instructions);
    const auto samples = cpu.SampleSignalStrengthAtCycleIntervals(samplesAtCycle);
    
    std::cout << "\tSignal strength sum: " << std::accumulate(samples.begin(), samples.end(), 0) << "\n";
}

void SecondPart()
{
    std::cout << "= Second part =\n";
    Utils::FileIo fileIo("../src/day9-input.txt");
    const auto filePerLine = fileIo.GetFileContent();

}

int main()
{
    std::cout << "Advent of code main\n";

    FirstPart();
    SecondPart(); 

    return 0;
}