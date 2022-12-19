#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <stack>

#include "Utils/FileIO.hpp"

struct Packet
{
    template<typename Stream>
    void Print(Stream& stream) const
    {
        if(isList)
        {
            stream << "[";
            for(int index = 0; index != packets.size(); ++index)
            {
                packets[index].Print(stream);
                if((index+1) != packets.size())
                {
                    stream << ",";
                }
            }
            stream << "]";
        }
        else
        {
            stream << value;
        }
    }

    friend bool operator==(const Packet& lhs, const Packet& rhs);

    bool isList = false;

    int value;
    std::vector<Packet> packets;
};

bool operator==(const Packet& lhs, const Packet& rhs)
{
    if(!lhs.isList && !rhs.isList)
    {
        return lhs.value == rhs.value;
    }
    else if(lhs.isList && rhs.isList)
    {
        return lhs.packets == rhs.packets;
    }

    return false;
}

typedef std::vector<std::pair<Packet, Packet>> Packets;

Packet GetPacketFromLine(const std::string& line)
{
    Packet packet;
    std::stack<Packet*> packetStack;
    packetStack.emplace(&packet);

    for(auto lineIterator = line.begin(); lineIterator != line.end(); ++lineIterator)
    {
        if(*lineIterator == '[')
        {
            packetStack.top()->isList = true;
            Packet packet;
            packet.isList = true;
            packetStack.top()->packets.push_back(packet);
            packetStack.emplace(&packetStack.top()->packets.back());
        }
        else if(*lineIterator != ',' && *lineIterator != ']')
        {
            Packet valuePacket;
            std::string value;
            auto nextIter = lineIterator + 1;
            if(*nextIter != ',' && *nextIter != '[' && *nextIter != ']')
            {
                value = std::string(lineIterator, lineIterator+2);
                ++lineIterator;
            }
            else
            {
                value = std::string(lineIterator, lineIterator+1);
            }
            valuePacket.value = std::stoi(value);
            packetStack.top()->packets.push_back(valuePacket);
        }
        else if(*lineIterator == ']')
        {
            packetStack.pop();
        }
    }

    return packet.packets[0];
}

Packets GetPacketsFromFile()
{
    Utils::FileIo fileIo("../src/day13-input.txt");
    const auto filePerLine = fileIo.GetFileContent();

    Packets packets;
    for(auto lineIterator = filePerLine.begin(); lineIterator != filePerLine.end(); ++lineIterator)
    {
        if((*lineIterator).length() > 0)
        {
            Packet firstPacket = GetPacketFromLine(*lineIterator);
            Packet secondPacket = GetPacketFromLine(*(++lineIterator));

            packets.push_back(std::make_pair(firstPacket, secondPacket));
        }
    }   

    return packets;
}

std::vector<Packet> GetPacketListFromFile()
{
    Utils::FileIo fileIo("../src/day13-input.txt");
    const auto filePerLine = fileIo.GetFileContent();

    std::vector<Packet> packets;
    for(auto lineIterator = filePerLine.begin(); lineIterator != filePerLine.end(); ++lineIterator)
    {
        if((*lineIterator).length() > 0)
        {
            packets.push_back(GetPacketFromLine(*lineIterator));
        }
    }   

    return packets;
}

int ComparePackets(const Packet& first, const Packet& second)
{
    Packet left = first;
    Packet right = second;

    if(!first.isList && !second.isList)
    {
        if(first.value > second.value)
        {
            return -1;
        }
        else if(first.value < second.value)
        {
            return 1;
        }
        return 0;
    }

    if(left.isList && right.isList)
    {
        int index = 0;
        while(index != left.packets.size() && index != right.packets.size())
        {
            const int result = ComparePackets(left.packets[index], right.packets[index]);
            if(result == 1 || result == -1)
            {
                return result;
            }

            ++index;
        }

        if(left.packets.size() != right.packets.size())
        {
            if(left.packets.size() < right.packets.size())
            {
                return 1;
            }
            else if(left.packets.size() > right.packets.size())
            {
                return -1;
            }
        }
    }

    if(first.isList && !second.isList)
    {
        Packet temp;
        temp.packets.push_back(second);
        temp.isList = true;

        left = first;
        right = temp;
        return ComparePackets(left, right);
    }

    if(!first.isList && second.isList)
    {
        Packet temp;
        temp.packets.push_back(first);
        temp.isList = true;

        left = temp;
        right = second;
        return ComparePackets(left, right);
    }

    return 0;
}

bool ComparePacketsBool(const Packet& first, const Packet& second)
{
    if(ComparePackets(first, second) == 1)
    {
        return true;
    }
    return false;
}

void FirstPart()
{
    std::cout << "= First part =\n";

    const auto packets = GetPacketsFromFile();    
    int result = 0;
    for(int index = 0; index != packets.size(); ++index)
    {
        const auto& firstPacket = packets[index].first;
        const auto& secondPacket = packets[index].second;

        const auto compareResult = ComparePackets(firstPacket, secondPacket);
        if(compareResult == 1)
        {
            result += (index + 1);
        }
    }

    std::cout << "\tResult: " << result << "\n";
}

void SecondPart()
{
    std::cout << "= Second part =\n";

    auto packetList = GetPacketListFromFile();
    const auto dividerPacketStart = GetPacketFromLine("[[2]]");
    const auto dividerPacketEnd = GetPacketFromLine("[[6]]");
    
    packetList.push_back(dividerPacketStart);
    packetList.push_back(dividerPacketEnd);

    std::sort(packetList.begin(), packetList.end(), ComparePacketsBool);

    int result = 1;
    for(int index = 0; index != packetList.size(); ++index)
    {
        if(packetList[index] == dividerPacketStart || packetList[index] == dividerPacketEnd)
        {
            result *= index+1;
        }
    }
    std::cout << "\tResult: " << result << "\n";
}

int main()
{
    std::cout << "Advent of code main\n";

    FirstPart();
    SecondPart(); 

    return 0;
}