#include <algorithm>
#include <iostream>
#include <deque>
#include <string>
#include <vector>

#include "Utils/FileIO.hpp"

struct Level
{
    Level(const std::string height, const int x, const int y)
    : mHeight(height)
    , mX(x)
    , mY(y)
    {
    }

    bool AllowedToStepInto(const Level& other) const
    {
        if(other.mHeight == "S")
        {
            return mHeight == "a";
        }
        if(mHeight == "E")
        {
            return other.mHeight == "z";
        }

        return ((mHeight[0] - other.mHeight[0]) <= 1);
    }

    std::vector<Level> GetAdjacentLevels(const std::vector<std::vector<Level>>& map) const
    {
        std::vector<Level> adjacent;
        if(mX > 0)
        {
            const Level& temp = map[mY][mX-1];
            adjacent.push_back(temp);
        }
        if(mX < map[0].size()-1)
        {
            const Level& temp = map[mY][mX+1];
            adjacent.push_back(temp);
        }
        if(mY > 0)
        {
            const Level& temp = map[mY-1][mX];
            adjacent.push_back(temp);
        }
        if(mY < map.size()-1)
        {
            const Level& temp = map[mY+1][mX];
            adjacent.push_back(temp);
        }
        return adjacent;
    }

    bool operator<(const Level& other) const
    {
        if(other.mHeight == "E")
        {
            return true;
        }

        return mHeight[0] < other.mHeight[0]; 
    }

    bool operator==(const std::string& other) const { return mHeight == other; }
    bool operator!=(const Level& other) const 
    { 
        return !(mX == other.mX && mY == other.mY); 
    }

    const std::string mHeight;
    const int mX = 0;
    const int mY = 0;
};

typedef std::vector<std::vector<Level>> HeightMap;

HeightMap GetHeightMapFromFile()
{
    Utils::FileIo fileIo("../src/day12-input.txt");
    const auto filePerLine = fileIo.GetFileContent();

    HeightMap heightMap;
    for(int rowIndex = 0; rowIndex != filePerLine.size(); ++rowIndex)
    {
        std::vector<Level> levelRow;
        for(int columnIndex = 0; columnIndex != filePerLine[rowIndex].size(); ++columnIndex)
        {
            levelRow.push_back(Level{ filePerLine[rowIndex].substr(columnIndex,1), columnIndex, rowIndex });
        }
        heightMap.push_back(levelRow);
    }

    return heightMap;
}

const Level& FindLevel(const std::string toFind, const HeightMap& map)
{
    for(const auto& row: map)
    {
        const auto iter = std::find_if(row.begin(), row.end(), [&toFind](const Level& level){ return level == toFind;});
        if(iter != row.end())
        {
            return *iter;
        }
    }

    return map[0][0];
}

int GetShortestPathFromLevelToEnd(const Level& level, const HeightMap heightMap)
{
    std::vector<std::vector<int>> lengthMap(heightMap.size(), std::vector<int>(heightMap[0].size(), -1));

    std::deque<Level> toCheck;
    toCheck.push_back(level);
    lengthMap[level.mY][level.mX] = 0;

    while(toCheck.size() > 0)
    {
        const Level& currentLevel = toCheck.front();
        toCheck.pop_front();

        const auto adjacents = currentLevel.GetAdjacentLevels(heightMap);
        for(const auto& adjacent : adjacents)
        {
            if(currentLevel.AllowedToStepInto(adjacent))
            {
                const int currentValue = lengthMap[adjacent.mY][adjacent.mX];
                const int nextValue = lengthMap[currentLevel.mY][currentLevel.mX]+1;
                if((currentValue == -1) || (currentValue > nextValue))
                {
                    lengthMap[adjacent.mY][adjacent.mX] = lengthMap[currentLevel.mY][currentLevel.mX]+1;
                    toCheck.push_back(adjacent);
                }
            }
        }
    }

    const Level start = FindLevel("S", heightMap);
    return lengthMap[start.mY][start.mX];
}

void Print(std::vector<std::vector<int>> map)
{
    for(const auto& row : map)
    {
        for(const auto& column : row)
        {
            std::cout << column << " ";
        }
        std::cout << "\n";
    }
}

int GetShortestPathFromMultipleLowerLevelsToEnd(const Level& level, const HeightMap heightMap)
{
    std::vector<std::vector<int>> lengthMap(heightMap.size(), std::vector<int>(heightMap[0].size(), -1));

    std::deque<Level> toCheck;
    toCheck.push_back(level);
    lengthMap[level.mY][level.mX] = 0;

    while(toCheck.size() > 0)
    {
        const Level& currentLevel = toCheck.front();
        toCheck.pop_front();

        const auto adjacents = currentLevel.GetAdjacentLevels(heightMap);
        for(const auto& adjacent : adjacents)
        {
            const int currentValue = lengthMap[adjacent.mY][adjacent.mX];
            const int nextValue = lengthMap[currentLevel.mY][currentLevel.mX]+1;

            if(currentLevel.AllowedToStepInto(adjacent))
            {
                if((currentValue == -1) || (currentValue > nextValue))
                {
                    lengthMap[adjacent.mY][adjacent.mX] = lengthMap[currentLevel.mY][currentLevel.mX]+1;
                    toCheck.push_back(adjacent);
                }
            }
        }
    }

    std::vector<int> lengthsFromLowestLevels;
    for(const auto& row : heightMap)
    {
        for(const auto& level : row)
        {
            if((level.mHeight == "a" || level.mHeight == "S") && lengthMap[level.mY][level.mX] != -1)
            {
                lengthsFromLowestLevels.push_back(lengthMap[level.mY][level.mX]);
            }
        }
    }
      
    return *(std::min_element(lengthsFromLowestLevels.begin(), lengthsFromLowestLevels.end()));
}

void FirstPart()
{
    std::cout << "= First part =\n";
    const auto heightMap = GetHeightMapFromFile();
    const Level& end = FindLevel("E", heightMap);

    std::cout << "\tShortest path: " << GetShortestPathFromLevelToEnd(end, heightMap) << "\n";
}

void SecondPart()
{
    std::cout << "= Second part =\n";
    const auto heightMap = GetHeightMapFromFile();
    const Level& end = FindLevel("E", heightMap);

    std::cout << "\tShortest path: " << GetShortestPathFromMultipleLowerLevelsToEnd(end, heightMap) << "\n";
}

int main()
{
    std::cout << "Advent of code main\n";

    FirstPart();
    SecondPart(); 

    return 0;
}