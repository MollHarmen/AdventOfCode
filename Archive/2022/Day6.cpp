#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#include "Utils/FileIO.hpp"

int FindStartOfMessageIndex(const std::string& line)
{
    constexpr size_t StartMessageLength = 14;

    for(int slidingWindowStartIndex = 0; slidingWindowStartIndex != line.length() - StartMessageLength; ++ slidingWindowStartIndex)
    {
        const std::string window = line.substr(slidingWindowStartIndex, StartMessageLength);
        bool isMessageStart = true;
        for(int index = 0; index != StartMessageLength-1; ++index)
        {
            const char character = window[index];
            const std::string innerWindow = window.substr(index+1, StartMessageLength-1-index);
            isMessageStart &= (innerWindow.find(character) == innerWindow.npos);
        }   

        if(isMessageStart)
        {
            return slidingWindowStartIndex + StartMessageLength;
        }
    }

    return 0;
}

int FindStartMarkerIndex(const std::string& line)
{
    constexpr size_t StartMarkerLength = 4;

    for(int slidingWindowStartIndex = 0; slidingWindowStartIndex != line.length() - StartMarkerLength; ++ slidingWindowStartIndex)
    {
        const std::string window = line.substr(slidingWindowStartIndex, StartMarkerLength);
        bool isMessageStartMarker = true;
        for(int index = 0; index != StartMarkerLength-1; ++index)
        {
            const char character = window[index];
            const std::string innerWindow = window.substr(index+1, StartMarkerLength-1-index);
            isMessageStartMarker &= (innerWindow.find(character) == innerWindow.npos);
        }   

        if(isMessageStartMarker)
        {
            return slidingWindowStartIndex + StartMarkerLength;
        }
    }

    return 0;
}

void FirstPart()
{
    std::cout << "= First part =\n";
    Utils::FileIo fileIo("../src/day6-input.txt");
    const auto filePerLine = fileIo.GetFileContent();

    for(const auto& line : filePerLine)
    {
        const auto index = FindStartMarkerIndex(line);
        std::cout << "\tStart Marker at index: " << index << "\n";
    }
}

void SecondPart()
{
    std::cout << "= Second part =\n";
    Utils::FileIo fileIo("../src/day6-input.txt");
    const auto filePerLine = fileIo.GetFileContent();

    for(const auto& line : filePerLine)
    {
        const auto index = FindStartOfMessageIndex(line);
        std::cout << "\tStart Message at index: " << index << "\n";
    }    
}

int main()
{
    std::cout << "Advent of code main\n";

    FirstPart();
    SecondPart(); 

    return 0;
}