#ifndef FILEIO_HPP
#define FILEIO_HPP

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace Utils
{
    class FileIo
    {
    public:
        FileIo(const std::string pathToFile)
        :file(pathToFile)
        {
        }

        ~FileIo()
        {
            file.close();
        }

        std::vector<std::string> GetFileContent()
        {
            if (file.is_open()) 
            {
                std::vector<std::string> fileContent;
                std::string line;
                while(std::getline(file, line))
                {
                    fileContent.push_back(line);
                }
                
                return fileContent;
            }
            return {};
        }

    private:
        std::ifstream file;
    };
}

#endif