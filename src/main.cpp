#include <iostream>
#include <sstream>
#include <vector>

#include "Utils/FileIO.hpp"
#include "Utils/StringHandling.hpp"

typedef std::string Row;
typedef std::vector<Row> View;

typedef std::vector<View> Views;

View GetVerticalMirrorOfView(const View& view)
{
    View mirroredView;
    for(const auto& line : view)
    {
        std::stringstream mirroredString;
        for(auto iterator = line.rbegin(); iterator != line.rend(); ++iterator)
        {
            mirroredString << *iterator;
        }
        mirroredView.push_back(mirroredString.str());
    }

    return mirroredView;
}

bool HasVerticalReflection(const View& view, int& reflectionOffset)
{
    View verticalMirroredView = GetVerticalMirrorOfView(view);

    int offset = 0;
    while(offset < view[0].length() - 1)
    {
        bool isSame = true;
        for(size_t lineIndex = 0u; lineIndex != view.size() && isSame; ++lineIndex)
        {
            const auto& line = view[lineIndex];
            for (size_t characterIndex = offset; characterIndex != line.size() && isSame; ++characterIndex)
            {
                const auto& character = line[characterIndex];
                const auto& mirroredChar = verticalMirroredView[lineIndex][characterIndex - offset];
                isSame &= (character == mirroredChar);
            }
        }
        if (isSame)
        {
            reflectionOffset = offset;
            return true;
        }
        ++offset;
    }

    return false;
}

View GetHorizontalMirroredView(const View& view)
{
    View mirroredView;
    for(auto iterator = view.rbegin(); iterator != view.rend(); ++iterator)
    {
        mirroredView.push_back(*iterator);
    }

    return mirroredView;
}

bool HasHorizontalReflection(const View& view, int& reflectionOffset)
{
    View horizontalMirroredView = GetHorizontalMirroredView(view);

    int offset = 0;
    while(offset < view.size() - 1)
    {
        bool isSame = true;
        for(size_t lineIndex = offset; lineIndex != view.size() && isSame; ++lineIndex)
        {
            const auto& line = view[lineIndex];
            const auto& mirroredLine = horizontalMirroredView[lineIndex - offset];
            isSame &= (line == mirroredLine);
        }
        if (isSame)
        {
            reflectionOffset = offset;
            return true;
        }
        ++offset;
    }

    return false;
}

void FirstPart()
{
    std::cout << "= First part =\n";
    Utils::FileIo fileIo("C:/projects/AdventOfCode/src/input.txt");
    const std::vector<std::string> filePerLine = fileIo.GetFileContent();

    Views views;
    std::vector<Row> view;
    for(const auto& line : filePerLine)
    {
        if(line.length() != 0)
        {
            view.push_back(line);
        }
        else
        {
            views.push_back(view);
            view.clear();
        }
    }
    views.push_back(view);

    int result = 0;
    for(const auto& view : views)
    {
        int reflecionOffset = 0;
        if(HasVerticalReflection(view, reflecionOffset))
        {
            const int answer = (view[0].length() / 2) + reflecionOffset;
            result += answer;
        }
        else if(HasHorizontalReflection(view, reflecionOffset))
        {
            const int answer = ((view.size() / 2) + reflecionOffset) * 100;
            result += answer;
        }
    }
    std::cout << "Result: " << result << "\n";
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