#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#include "Utils/FileIO.hpp"

typedef int Tree;
typedef std::vector<Tree> TreeLine;
typedef std::vector<TreeLine> TreeGrid;

TreeGrid GetTreeGridFromFile()
{
    Utils::FileIo fileIo("../src/day8-input.txt");
    const auto filePerLine = fileIo.GetFileContent();

    TreeGrid treeGrid;
    for(const auto& line : filePerLine)
    {
        TreeLine treeLine;
        for(const auto& character : line)
        {
            Tree treeHeight = character - '0'; 
            treeLine.push_back(treeHeight);
        }
        treeGrid.push_back(treeLine);
    }

    return treeGrid;
}

int GetAlwaysVisibleTreeCount(const TreeGrid& treeGrid)
{
    constexpr int overlappingTreesInCorners = 4;

    int count = 0;
    count += treeGrid.size() * 2;
    count += treeGrid[0].size() * 2;
    count -= overlappingTreesInCorners;
    return count;
}

bool IsVisibleFromTheLeft(const int x, const int y, const TreeGrid& treeGrid) 
{
    const Tree& tree = treeGrid[x][y];
    const auto heighestTreeToLeft = std::max_element(treeGrid[x].begin(), treeGrid[x].begin()+y);
    
    return tree > *heighestTreeToLeft;
}

bool IsVisibleFromTheRight(const int x, const int y, const TreeGrid& treeGrid) 
{
    const Tree& tree = treeGrid[x][y];
    const auto heighestTreeToRight = std::max_element(treeGrid[x].begin()+(y+1), treeGrid[x].end());
    
    return tree > *heighestTreeToRight;
}

TreeLine GetTreeRowForIndex(const int index, const TreeGrid& treeGrid)
{
    TreeLine treeRow;
    for(const auto& treeLine : treeGrid)
    {
        treeRow.push_back(treeLine[index]);
    }

    return treeRow;
}

bool IsVisibleFromTheTop(const int x, const int y, const TreeGrid& treeGrid) 
{
    const Tree& tree = treeGrid[x][y];
    const auto treeRow = GetTreeRowForIndex(y, treeGrid);
    const auto heighestTreeToTop = std::max_element(treeRow.begin(), treeRow.begin()+x);
    
    return tree > *heighestTreeToTop;
}

bool IsVisibleFromTheBottom(const int x, const int y, const TreeGrid& treeGrid) 
{
    const Tree& tree = treeGrid[x][y];
    const auto treeRow = GetTreeRowForIndex(y, treeGrid);
    const auto heighestTreeToBottom = std::max_element(treeRow.begin()+(x+1), treeRow.end());
    
    return tree > *heighestTreeToBottom;
}

int GetNumberOfVisibleTreesInTreeGrid(const TreeGrid& treeGrid)
{
    int visibleTrees = GetAlwaysVisibleTreeCount(treeGrid);
    for(auto treeLineIndex = 1; treeLineIndex != treeGrid.size()-1; ++treeLineIndex)
    {
        const TreeLine& treeLine = treeGrid[treeLineIndex];
        for(auto treeIndex = 1; treeIndex != treeLine.size()-1; ++treeIndex)
        {
            const Tree& tree = treeLine[treeIndex];
            if(IsVisibleFromTheLeft(treeLineIndex, treeIndex, treeGrid) || IsVisibleFromTheRight(treeLineIndex, treeIndex, treeGrid) 
                || IsVisibleFromTheTop(treeLineIndex, treeIndex, treeGrid) || IsVisibleFromTheBottom(treeLineIndex, treeIndex, treeGrid))
            {
                ++visibleTrees;
            }
        }
    }

    return visibleTrees;
}

int GetNumberOfTreesVisibleToLeft(const int x, const int y, const TreeGrid& treeGrid) 
{
    if(y == 0)
    {
        return 0;
    }

    const Tree& tree = treeGrid[x][y];
    int count = 0;
    for(auto index = y-1; index >= 0; --index)
    {   
        if(tree > treeGrid[x][index])
        {
            ++count;
        }
        else if(tree <= treeGrid[x][index])
        {
            return ++count;
        }
    }
    
    return count;
}

int GetNumberOfTreesVisibleToRight(const int x, const int y, const TreeGrid& treeGrid) 
{
    const auto& treeLine = treeGrid[x];
    if(y >= treeLine.size()-1)
    {
        return 0;
    }
    const Tree& tree = treeGrid[x][y];
    int count = 0;
    for(auto index = y+1; index <= treeLine.size(); ++index)
    {
        if(treeGrid[x][index] < tree)
        {
            ++count;
        }
        else if(treeGrid[x][index] >= tree)
        {
            return ++count;
        }
    }
    
    return count;
}

int GetNumberOfTreesVisibleToTop(const int x, const int y, const TreeGrid& treeGrid) 
{
    if(x == 0)
    {
        return 0;
    }
    
    const Tree& tree = treeGrid[x][y];
    int count = 0;
    for(auto index = x-1; index >= 0; --index)
    {
        if(treeGrid[index][y] < tree)
        {
            ++count;
        }
        else if(treeGrid[index][y] >= tree)
        {
            return ++count;
        }
    }
    
    return count;
}

int GetNumberOfTreesVisibleToBottom(const int x, const int y, const TreeGrid& treeGrid) 
{
    if(x >= treeGrid.size()-1)
    {
        return 0;
    }

    const Tree& tree = treeGrid[x][y];
    int count = 0;
    for(auto index = x+1; index < treeGrid.size(); ++index)
    {
        if(treeGrid[index][y] < tree)
        {
            ++count;
        }
        else if(treeGrid[index][y] >= tree)
        {
            return ++count;
        }
    }
    
    return count;
}

TreeGrid GetScenicScoresForEachTree(const TreeGrid& treeGrid)
{
    TreeGrid scenicGrid;
    for(auto treeLineIndex = 0; treeLineIndex != treeGrid.size(); ++treeLineIndex)
    {
        const TreeLine& treeLine = treeGrid[treeLineIndex];
        TreeLine scenicLine;
        for(auto treeIndex = 0; treeIndex != treeLine.size(); ++treeIndex)
        {
            const auto numberOfTreesVisibleToLeft = GetNumberOfTreesVisibleToLeft(treeLineIndex, treeIndex, treeGrid);
            const auto numberOfTreesVisibleToRight = GetNumberOfTreesVisibleToRight(treeLineIndex, treeIndex, treeGrid);
            const auto numberOfTreesVisibleToTop = GetNumberOfTreesVisibleToTop(treeLineIndex, treeIndex, treeGrid);
            const auto numberOfTreesVisibleToBottom = GetNumberOfTreesVisibleToBottom(treeLineIndex, treeIndex, treeGrid);

            scenicLine.push_back(numberOfTreesVisibleToLeft*numberOfTreesVisibleToRight*numberOfTreesVisibleToTop*numberOfTreesVisibleToBottom);
        }
        scenicGrid.push_back(scenicLine);
    }

    return scenicGrid;
}

void FirstPart()
{
    std::cout << "= First part =\n";
    const auto treeGrid = GetTreeGridFromFile();
    const auto numberOfVisibleTrees = GetNumberOfVisibleTreesInTreeGrid(treeGrid);

    std::cout << "\tNumber of visible trees: " << numberOfVisibleTrees << "\n";
}

int GetMaxScoreFromScenicGrid(const TreeGrid& scenicGrid)
{
    TreeLine maxScoreForEachLine;
    for(const auto& treeLine : scenicGrid)
    {
        const auto maxIterator = std::max_element(treeLine.begin(), treeLine.end());
        maxScoreForEachLine.push_back(*maxIterator);
    }

    return *(std::max_element(maxScoreForEachLine.begin(), maxScoreForEachLine.end()));
}

void SecondPart()
{
    std::cout << "= Second part =\n";
    const auto treeGrid = GetTreeGridFromFile();
    const auto scenicGrid = GetScenicScoresForEachTree(treeGrid);
    const auto maxScore = GetMaxScoreFromScenicGrid(scenicGrid);

    std::cout << "\t Max score for grid: " << maxScore << "\n";
}

int main()
{
    std::cout << "Advent of code main\n";

    FirstPart();
    SecondPart(); 

    return 0;
}