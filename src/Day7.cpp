#include <algorithm>
#include <numeric>
#include <iostream>
#include <string>
#include <vector>
#include <stack>

#include "Utils/FileIO.hpp"

struct FileSystemItem
{
    FileSystemItem(const std::string name)
    : mName(name)
    {}

    virtual ~FileSystemItem(){}

    std::string mName;
};

struct File : public FileSystemItem
{
    File(const std::string& name)
    : FileSystemItem(name)
    {}
    
    File& operator=(const File& other)
    {
        mName = other.mName;
        size = other.size;

        return *this;
    }

    int size = 0;
};

struct Directory : public FileSystemItem
{
    Directory(const std::string name, Directory* upper)
    : FileSystemItem(name)
    , mUpper(upper)
    {}

    std::vector<Directory> GetDirs() const
    {
        std::vector<Directory> dirs;
        
        if(GetSize() <= 100000)
        {
            dirs.push_back(*this);
        }

        for(const auto& dir : mDirs)
        {
            const auto dirsOfDir = dir.GetDirs();
            dirs.insert(dirs.end(), dirsOfDir.begin(), dirsOfDir.end());
        }

        return dirs;
    }

    std::vector<int> GetDirsLargerOrEqualTo(const int number) const
    {
        std::vector<int> sizes;
        
        if(GetSize() >= number)
        {
            sizes.push_back(this->GetSize());
        }

        for(const auto& dir : mDirs)
        {
            const auto numbersOfDir = dir.GetDirsLargerOrEqualTo(number);
            sizes.insert(sizes.end(), numbersOfDir.begin(), numbersOfDir.end());
        }

        return sizes;
    }

    int GetSize() const
    {
        int size = GetFileSize();
        for(const auto& dir : mDirs)
        {
            size += dir.GetSize();
        }

        return size;
    }

    int GetFileSize() const
    {
        int size = 0;
        for(const auto& file : mFiles)
        {
            size += file.size;
        }

        return size;
    }

    void Print() const
    {
        std::cout << mName << "\n";
        for(const auto& file : mFiles)
        {
            std::cout << "\t" << file.mName << "\n";
        }

        for(const auto& item : mDirs)
        {
            item.Print();
        }
    };

    Directory* mUpper = nullptr;
    std::vector<File> mFiles;
    std::vector<Directory> mDirs;
};

const std::string cdKey = "$ cd ";
const std::string lsKey = "$ ls";
const std::string dirKey = "dir ";

bool IsChangeDirectoryCommand(const std::string& line)
{
    return line.find(cdKey) != line.npos;
}

bool IsDirItem(const std::string& line)
{
    return line.find(dirKey) != line.npos;
}

bool IsFileItem(const std::string& line)
{
    return line.find(lsKey) == line.npos;
}

Directory GetFileSystemStructureFromCommandLine(const std::vector<std::string>& commandLineOutput)
{
    std::vector<FileSystemItem> fileSystem;
    Directory baseDir("/" , nullptr);
    std::stack<Directory*> currentDir;
    currentDir.push(&baseDir);
    for(const auto& line : commandLineOutput)
    {
        if(IsChangeDirectoryCommand(line))
        {
            const std::string cdTarget = line.substr(cdKey.length(), line.length());
            if(cdTarget == "..")
            {
                currentDir.pop();
            }
            else
            {
                auto dirIterator = std::find_if(currentDir.top()->mDirs.begin(), currentDir.top()->mDirs.end(), 
                    [&cdTarget](const Directory& item){ return item.mName == cdTarget; });
                
                if(dirIterator == currentDir.top()->mDirs.end())
                {
                    std::cout << "Dir not found: " << line << "\t" << cdTarget << "\n";
                    baseDir.Print();
                    std::cout << " ==== \n";
                    std::cout << line << "\n";
                    std::cout << " current dir: " << currentDir.top()->mName << " numberOfSubDirs: " << currentDir.top()->mDirs.size() << " Number of files: " << currentDir.top()->mFiles.size() << "\n";
                    std::cout << "Upper: " << currentDir.top()->mUpper->mName << "\n";
                }

                currentDir.push(static_cast<Directory*>(&(*dirIterator)));
                
            }
        }
        else if(IsDirItem(line))
        {
            const std::string dirName = line.substr(dirKey.length(), line.length());
            Directory dir(dirName, &(*currentDir.top()));
            currentDir.top()->mDirs.push_back(dir);
        }
        else if(IsFileItem(line))
        {
            const std::string fileName = line.substr(line.find(" "), line.length());
            const int fileSize = std::stoi(line.substr(0, line.find(" ")));
            
            File file(fileName);
            file.size = fileSize;

            currentDir.top()->mFiles.push_back(file);
        }
    }

    return baseDir;
}

void FirstPart()
{
    std::cout << "= First part =\n";
    Utils::FileIo fileIo("../src/day7-input.txt");
    const auto filePerLine = fileIo.GetFileContent();

    const auto fileSystemStructure = GetFileSystemStructureFromCommandLine(filePerLine);
    const std::vector<Directory> allDirsWithAtMostSize = fileSystemStructure.GetDirs();
    
    int accumulate = 0;
    for(const auto& dir : allDirsWithAtMostSize)
    {
        accumulate += dir.GetSize();
    }

    std::cout << " result " << accumulate << "\n";
}

void SecondPart()
{
    std::cout << "= Second part =\n";
    Utils::FileIo fileIo("../src/day7-input.txt");
    const auto filePerLine = fileIo.GetFileContent();

    const auto fileSystemStructure = GetFileSystemStructureFromCommandLine(filePerLine);
    const auto totalDiskUsage = fileSystemStructure.GetSize();
    const auto sizeToRemove = 30000000 - (70000000 - totalDiskUsage);
    std::cout << "totalDiskUsage " << totalDiskUsage << " Size required: " << sizeToRemove << "\n";

    const auto deleteCandidates = fileSystemStructure.GetDirsLargerOrEqualTo(sizeToRemove);
    const auto smallest = std::min_element(deleteCandidates.begin(), deleteCandidates.end());
    std::cout << "Smallest possible dir to remove: " << *smallest << "\n";
}

int main()
{
    std::cout << "Advent of code main\n";

    FirstPart();
    SecondPart(); 

    return 0;
}