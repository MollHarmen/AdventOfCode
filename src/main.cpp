#include <algorithm>
#include <iostream>
#include <map>
#include <stdexcept>
#include <vector>
#include <queue>

#include "Utils/FileIO.hpp"
#include "Utils/StringHandling.hpp"

struct Signal
{
    bool value;
    std::string source;
    std::string target;
};

typedef std::queue<Signal> Signals;
Signals g_signals;

struct Module
{
    Module(const std::string& name, std::vector<std::string> targets)
    : mName(name)
    , mTargets(targets)
    {}

    virtual ~Module(){}

    virtual void Process(const bool signal){};

    bool Output() const
    {
        return mState;
    }

    void PutOutputToSignals(const bool signal)
    {
        for(const auto& target : mTargets)
        {
            g_signals.push({signal, mName, target});
        }
    }

    virtual void AddConnection(const Module* module)
    {}

    bool HasNameInTargets(const std::string& name) const
    {
        const auto found = std::find_if(mTargets.begin(), mTargets.end(), [&name](const std::string& targetName){ return name == targetName; });
        return found != mTargets.end();
    }

    std::string mName;
    bool mState = false;
    std::vector<std::string> mTargets;
};

struct Button : public Module
{
    Button()
    : Module("button", {"broadcaster"})
    {}
    
    void Process(const bool signal) override
    {
        PutOutputToSignals(signal);
    };
};

struct Broadcaster : public Module
{
    Broadcaster(const std::string& name, std::vector<std::string> targets)
    : Module(name, targets)
    {}

    void Process(const bool signal)
    {
        PutOutputToSignals(signal);
    }
};

struct FlipFlop : public Module
{
    FlipFlop(const std::string& name, std::vector<std::string> targets)
    : Module(name, targets)
    {}

    void Process(const bool signal) override
    {
        if(!signal)
        {
            mState = !mState;
            PutOutputToSignals(mState);
        }
    }
};

struct Conjunction : public Module
{
    Conjunction(const std::string& name, std::vector<std::string> targets)
    : Module(name, targets)
    {
        mState = true;
    }

    void AddConnection(const Module* module) override
    {
        incommingConnections.push_back(module);
    }

    void Process(const bool) override
    {
        bool allHigh = true;
        for(const auto* module : incommingConnections)
        {
            allHigh &= module->Output();
            if(!allHigh)
                break;
        }

        if(allHigh)
        {
            mState = !allHigh;
            PutOutputToSignals(mState);
        }
        else
        {
            mState = true;
            PutOutputToSignals(mState);
        }
    }

    std::vector<const Module*> incommingConnections;
};

typedef std::map<std::string, Module*> Modules;

void AddToModules(const std::string& input, Modules& modules)
{
    std::vector<std::string> split;
    StringHandling::SplitOnString(input, split, " -> ");

    std::vector<std::string> targets;
    StringHandling::SplitOnString(split[1], targets, ", ");
    const std::string& name = split[0];
    if(name == "broadcaster")
    {
        Broadcaster* broadcaster = new Broadcaster(name, targets);
        modules[name] = broadcaster;
    }
    else
    {
        const char& type = name[0];
        const std::string typelessName = name.substr(1);
        if(type == '%')
        {
            FlipFlop* flipFlop = new FlipFlop(typelessName, targets);
            modules[typelessName] = flipFlop;
        }
        else if(type == '&')
        {
            Conjunction* conjunction = new Conjunction(typelessName, targets);
            for(const auto modulePair : modules)
            {
                const auto* module = modulePair.second;
               if(module->HasNameInTargets(typelessName))
               {
                    conjunction->AddConnection(module);
               }
            }
            modules[typelessName] = conjunction;
        }
    }
}

std::string GetOutputString(const bool value)
{
    if(value)
    {
        return " -high-> ";
    }

    return " -low-> ";
}

void PushButton(Modules& modules, uint64_t& high, uint64_t& low)
{
    modules["button"]->Process(false);
    while(!g_signals.empty())
    {
        auto& signalIterator = g_signals.front();
        std::cout << signalIterator.source << GetOutputString(signalIterator.value) << signalIterator.target << "\n";

        try
        {
            modules.at(signalIterator.target)->Process(signalIterator.value);
        }
        catch(const std::out_of_range&)
        {
            // Do nothing?
            std::cout << "Unknown target: " << signalIterator.target << "\n";
        }

        if(signalIterator.value)
        {
            ++high;
        }
        else
        {
            ++low;
        }
        
        g_signals.pop();
    }
}

void FirstPart()
{
    std::cout << "= First part =\n";
    Utils::FileIo fileIo("C:/projects/AdventOfCode/src/input.txt");
    const std::vector<std::string> filePerLine = fileIo.GetFileContent();

    std::string boadcast;
    Modules modules;
    modules["button"] = new Button();

    std::vector<std::string> addAtEnd;
    for(const auto& line : filePerLine)
    {
        if(line.find("&") != line.npos)
        {
            addAtEnd.push_back(line);
        }
        else
        {
            AddToModules(line, modules);
        }
    }

    for(const auto& line : addAtEnd)
    {
        AddToModules(line, modules);
    }

    uint64_t high = 0u;
    uint64_t low = 0u;
    for(size_t buttonPushes = 0u; buttonPushes != 1000; ++buttonPushes)
    {
        PushButton(modules, high, low);
    }

    std::cout << "result: " << high * low << "\n";

    for(auto& module : modules)
    {
        delete module.second;
    }
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