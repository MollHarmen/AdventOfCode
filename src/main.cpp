#include <iostream>
#include <string>
#include <vector>

#include "Utils/FileIO.hpp"

struct FileSystemItem
{
    FileSystemItem(const std::string name)
    : mName(name)
    {}

    const std::string mName;
    int size = 0;
};

struct File : public FileSystemItem
{
};

struct Directory : public FileSystemItem
{
    Directory(const std::string name, FileSystemItem* upper)
    : FileSystemItem(name)
    , mUpper(upper)
    {}

    const FileSystemItem* mUpper;
    const std::vector<FileSystemItem> mDiretoryContent;
};

const std::string cdKey = "$ cd ";

bool IsChangeDirectoryCommand(const std::string& line)
{
    return line.find("$ cd ") != line.npos;
}

int GetFileSystemStructureFromCommandLine(const std::vector<std::string>& commandLineOutput)
{
    std::vector<FileSystemItem> fileSystem;
    for(const auto& line : commandLineOutput)
    {
        if(line[0] == '$')
        {
            if(IsChangeDirectoryCommand(line))
            {
                const std::string cdTarget = line.substr(cdKey.length(), line.length());
                std::cout << "\tChange Dir to " << cdTarget << "\n";
            }
        }
        else
        {
            std::cout << "Result: " << line << "\n";
        }
    }

    return 0;
}

void FirstPart()
{
    std::cout << "= First part =\n";
    Utils::FileIo fileIo("../src/day7-input.txt");
    const auto filePerLine = fileIo.GetFileContent();

    const auto fileSystemStructure = GetFileSystemStructureFromCommandLine(filePerLine);
}

void SecondPart()
{
    std::cout << "= Second part =\n";
}

int main()
{
    std::cout << "Advent of code main\n";

    FirstPart();
    SecondPart(); 

    return 0;
}