#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

#include "Utils/FileIO.hpp"

typedef std::vector<std::vector<char>> Grid;

struct Position
{
    Position(const int xPosition, const int yPosition)
    : x(xPosition)
    , y(yPosition)
    {

    }

    int x = 0;
    int y = 0;
};

bool operator==(const Position& left, const Position& right)
{
    return left.x == right.x && left.y == right.y;
}

int GetDistance(const Position first, const Position second)
{
    return std::abs(first.x - second.x) + std::abs(first.y - second.y);
}

struct Line
{
    int x1 = 0;
    int y1 = 0;
    int x2 = 0;
    int y2 = 0;
};

struct Beacon : public Position
{
    Beacon(const int xPosition, const int yPosition)
    : Position(xPosition, yPosition)
    {}
};

struct Sensor : public Position
{
    Sensor(const int xPosition, const int yPosition, const Beacon closestBeacon)
    : Position(xPosition, yPosition)
    , beacon(closestBeacon)
    , distanceToBeacon(GetDistance(*this, beacon))
    {
        borders.push_back({x, y - distanceToBeacon-1, x + distanceToBeacon+1, y});
        borders.push_back({x + distanceToBeacon+1, y, x, y + distanceToBeacon+1});
        borders.push_back({x, y + distanceToBeacon+1, x - distanceToBeacon-1, y});
        borders.push_back({x - distanceToBeacon-1, y, x, y - distanceToBeacon-1});
    }

    void Print() const
    {
        std::cout << "Sensor: " << x << ", " << y << " Beacon: " << beacon.x << ", " << beacon.y << "\n";
    }

    const Beacon beacon;
    const int distanceToBeacon = 0;
    std::vector<Line> borders;
};

int xOffset = 0;
int yOffset = 0;


int GetSensorX(const std::string& line)
{
    const auto sensorX = line.find("x=")+2;
    return std::stoi(line.substr(sensorX, line.find(",")-sensorX));
}

int GetSensorY(const std::string& line)
{
    const auto sensorY = line.find("y=")+2;
    return std::stoi(line.substr(sensorY, line.find(":")-sensorY));
}

int GetBeaconX(const std::string& line)
{
    const auto sensorX = line.find("x=")+2;
    return std::stoi(line.substr(sensorX, line.find(",")-sensorX));
}

int GetBeaconY(const std::string& line)
{
    const auto sensorY = line.find("y=")+2;
    return std::stoi(line.substr(sensorY, line.find(":")-sensorY));
}

std::vector<Sensor> GetSensorsFromFileContent(const std::vector<std::string>& filePerLine)
{
    std::vector<Sensor> sensors;

    for(auto line : filePerLine)
    {
        // Sensor at x=2, y=18: closest beacon is at x=-2, y=15
        const auto sensorX = GetSensorX(line);
        const auto sensorY = GetSensorY(line);
        line = line.substr(line.find(":"), line.length());
        const auto beaconX = GetBeaconX(line);
        const auto beaconY = GetBeaconY(line);

        const auto distance = std::abs(sensorX - beaconX) + std::abs(sensorY - beaconY);

        const Beacon beacon(beaconX, beaconY);
        const Sensor sensor(sensorX, sensorY, beacon);
        sensors.push_back(sensor);
    }

    return sensors;
}

std::vector<int> GetAllXCoordinates(const std::vector<Sensor>& sensors)
{
    std::vector<int> xCoordinates;
    
    for(const auto& sensor : sensors)
    {
        xCoordinates.push_back(sensor.x);
        xCoordinates.push_back(sensor.beacon.x);
    }
    return xCoordinates;
}

int GetMinX(const std::vector<Sensor>& sensors)
{
    const auto allXes = GetAllXCoordinates(sensors);
    return *std::min_element(allXes.begin(), allXes.end());
}

int GetMaxX(const std::vector<Sensor>& sensors)
{
    const auto allXes = GetAllXCoordinates(sensors);
    return *std::max_element(allXes.begin(), allXes.end());
}

int GetMaxDistance(const std::vector<Sensor>& sensors)
{
    std::vector<int> distances;
    
    for(const auto& sensor : sensors)
    {
        distances.push_back(sensor.distanceToBeacon);
    }
    return *std::max_element(distances.begin(), distances.end());
}

bool IsPositionWithinRangeOfSensor(const Position& position, const std::vector<Sensor>& sensors)
{
    for(const auto& sensor : sensors)
    {
        const int distance = GetDistance(position, sensor);
        if(distance <= sensor.distanceToBeacon)
        {
            return true;
        }
    }
    return false;
}

bool PositionIsBeacon(const Position& position, const std::vector<Sensor>& sensors)
{
    for(const auto& sensor : sensors)
    {
        if(static_cast<Position>(sensor.beacon) == position)
        {
            return true;
        }
    }

    return false;
}

int CountNumberOfCharsInString(const std::string& string, const char characterToCount)
{
    int count = 0;
    for(const auto& character : string)
    {
        if(character == characterToCount)
        {
            ++count;
        }
    }

    return count;
}

std::vector<Line> GetAllLinesFromSensors(const std::vector<Sensor>& sensors)
{
    std::vector<Line> lines;
    for(auto& sensor : sensors)
    {
        lines.insert(lines.end(), sensor.borders.begin(), sensor.borders.end());
    }

    return lines;
}

Position GetIntersection(const Line& left, const Line& right)
{
    float m1 = 0.0f;
    float c1 = 0.0f;
    float m2 = 0.0f;
    float c2 = 0.0f;
    float dx = 0.0f;
    float dy = 0.0f;

    int intersectionX = 0;
    int intersectionY = 0;

    dx = left.x2 - left.x1;
    dy = left.y2 - left.y1;

    m1 = dy / dx;

    // y = mx + c
    // intercept c = y - mx
    c1 = left.y1 - m1 * left.x1; // which is same as y2 - slope * x2

    dx = right.x2 - right.x1;
    dy = right.y2 - right.y1;

    m2 = dy / dx;

    c2 = right.y2 - m2 * right.x2; // which is same as y2 - slope * x2

    if( (m1 - m2) == 0)
    {
        // No intersection
        return {-1, -1};
    }
    
    intersectionX = (c2 - c1) / (m1 - m2);
    intersectionY = m1 * intersectionX + c1;
        
    return {intersectionX, intersectionY};
}

const int gridSize = 4000000;

bool IsPositionWithinGridBoundaries(const Position& position)
{
    return position.x >= 0 && position.x <= gridSize && position.y >= 0 && position.y <= gridSize;
}

std::vector<Position> GetLineIntersectionPoints(const std::vector<Line>& lines)
{
    std::vector<Position> intersectionPoints;
    for(int index = 0; index != lines.size(); ++index)
    {
        for(int compareIndex = index + 1; compareIndex != lines.size(); ++compareIndex)
        {
            const Position intersection = GetIntersection(lines[index], lines[compareIndex]);
            if(IsPositionWithinGridBoundaries(intersection))
            {
                intersectionPoints.push_back(intersection);
            }
        }
    }

    return intersectionPoints;
}

void FirstPart()
{
    std::cout << "= First part =\n";
    Utils::FileIo fileIo("../src/day15-input.txt");
    const auto filePerLine = fileIo.GetFileContent();
    const auto sensors = GetSensorsFromFileContent(filePerLine);

    std::vector<char> line;

    Position position(GetMinX(sensors) - GetMaxDistance(sensors), 2000000);
    const int maxX = GetMaxX(sensors) + GetMaxDistance(sensors);
    for(; position.x != maxX+1; ++position.x)
    {
        if(!PositionIsBeacon(position, sensors))
        {
            if(IsPositionWithinRangeOfSensor(position, sensors))
            {
                line.push_back('#');
            }
            else
            {
                line.push_back('.');
            }
        }
        else
        {
            line.push_back('X');
        }
    }

    const std::string lineString(line.begin(), line.end());
    // std::cout << lineString << "\n";
    std::cout << CountNumberOfCharsInString(lineString, '#') << "\n";
}

void SecondPart()
{
    std::cout << "= Second part =\n";
    Utils::FileIo fileIo("../src/day15-input.txt");
    const auto filePerLine = fileIo.GetFileContent();
    const auto sensors = GetSensorsFromFileContent(filePerLine);

    const auto allLinesFromSensors = GetAllLinesFromSensors(sensors);
    const auto intersections = GetLineIntersectionPoints(allLinesFromSensors);

    for(const auto intersection : intersections)
    {
        if(!IsPositionWithinRangeOfSensor(intersection, sensors))
        {
            std::cout << "Found: " << intersection.x << ", " << intersection.y << "\n";
            const unsigned long long result = (static_cast<unsigned long long>(intersection.x) * 4000000ull) + static_cast<unsigned long long>(intersection.y);
            std::cout << result << "\n";
            return;
        }
    }
}

int main()
{
    std::cout << "Advent of code main\n";

    // FirstPart();
    SecondPart(); 

    return 0;
}