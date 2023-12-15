#ifndef STRINGHANDLING_HPP
#define STRINGHANDLING_HPP

#include <string>
#include <vector>

namespace StringHandling
{
    
    size_t SplitStringOnCharacter(const std::string &inputString, std::vector<std::string> &splittedStrings, char splitOnChar)
    {
        size_t position = inputString.find(splitOnChar);
        size_t initialPos = 0;
        splittedStrings.clear();
    
        while( position != std::string::npos ) {
            splittedStrings.push_back( inputString.substr( initialPos, position - initialPos ) );
            initialPos = position + 1;
    
            position = inputString.find( splitOnChar, initialPos );
        }
    
        splittedStrings.push_back( inputString.substr( initialPos, std::min( position, inputString.size() ) - initialPos + 1 ) );
    
        return splittedStrings.size();
    }

    size_t SplitOnString(const std::string &inputString, std::vector<std::string> &splittedStrings, std::string splitOnString)
    {
        size_t position = inputString.find(splitOnString);
        size_t initialPos = 0;
        splittedStrings.clear();
    
        while( position != std::string::npos ) {
            splittedStrings.push_back( inputString.substr( initialPos, position - initialPos ) );
            initialPos = position + splitOnString.size();
    
            position = inputString.find( splitOnString, initialPos );
        }
    
        splittedStrings.push_back( inputString.substr( initialPos, std::min( position, inputString.size() ) - initialPos + 1 ) );
    
        return splittedStrings.size();
    }

} // namespace StringHandling


#endif