#include "Utils/FileIO.hpp"
#include "Utils/StringHandling.hpp"

#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

const std::string xmas = "XMAS";

std::vector<std::string> GetHorizontalWords(const std::vector<std::string>& file_content, const size_t line_index, const size_t character_index) {
    std::vector<std::string> words;
    if(character_index >= (xmas.size()-1)) {
        std::string word; 
        for(size_t index = character_index; index != character_index-xmas.size(); --index) {
            word += file_content[line_index][index];
        }
        words.push_back(word);
        std::cout << "Reverse XMAS: " << word << "\n";
    }
    if(character_index <= (file_content[line_index].size() - xmas.size())) {
        std::string word;
        for(size_t index = character_index; index != character_index+xmas.size(); ++index) {
            word += file_content[line_index][index];
        }
        words.push_back(word);
        std::cout << "XMAS: " << word << "\n";
    }

    return words;
}
std::vector<std::string> GetDiagonalWords(const std::vector<std::string>& file_content, const size_t line_index, const size_t character_index) {
    std::vector<std::string> words;
    if(line_index >= (xmas.size()-1) && character_index >= (xmas.size()-1)) {
        std::string word; 
        auto row = character_index;
        for(size_t index = line_index; index != line_index-xmas.size(); --index) {
            word += file_content[index][row--];
        }
        words.push_back(word);
        std::cout << "Diagonal top left XMAS: " << word << "\n";
    }
    if(line_index <= (file_content.size() - xmas.size()) && character_index >= (xmas.size()-1)) {
        std::string word;
        auto row = character_index;
        for(size_t index = line_index; index != line_index+xmas.size(); ++index) {
            word += file_content[index][row--];
        }
        words.push_back(word);
        std::cout << "Diagonal bottom left XMAS: " << word << "\n";
    }
    if(line_index >= (xmas.size()-1) && character_index <= (file_content[line_index].size() - xmas.size())) {
        std::string word; 
        auto row = character_index;
        for(size_t index = line_index; index != line_index-xmas.size(); --index) {
            word += file_content[index][row++];
        }
        words.push_back(word);
        std::cout << "Diagonal top right XMAS: " << word << "\n";
    }
    if(line_index <= (file_content.size() - xmas.size()) && character_index <= (file_content[line_index].size() - xmas.size())) {
        std::string word;
        auto row = character_index;
        for(size_t index = line_index; index != line_index+xmas.size(); ++index) {
            word += file_content[index][row++];
        }
        words.push_back(word);
        std::cout << "Diagonal bottom right XMAS: " << word << "\n";
    }

    return words;
}

std::vector<std::string> GetVerticalWords(const std::vector<std::string>& file_content, const size_t line_index, const size_t character_index) {
    std::vector<std::string> words;
    if(line_index >= (xmas.size()-1)) {
        std::string word; 
        for(size_t index = line_index; index != line_index-xmas.size(); --index) {
            word += file_content[index][character_index];
        }
        words.push_back(word);
        std::cout << "Vertical top XMAS: " << word << "\n";
    }
    if(line_index <= (file_content.size() - xmas.size())) {
        std::string word;
        for(size_t index = line_index; index != line_index+xmas.size(); ++index) {
            word += file_content[index][character_index];
        }
        words.push_back(word);
        std::cout << "Vertical bottom XMAS: " << word << "\n";
    }

    return words;
}


void FirstPart() {
    auto input_file = Utils::FileIo("input.txt");
    const auto file_content = input_file.GetFileContent();

    int result = 0;

    for(size_t line_index = 0u; line_index != file_content.size(); ++line_index) {
        const auto& line = file_content[line_index];
        for(size_t character_index = 0u; character_index != line.size(); ++character_index) {
            const auto& character = line[character_index];
            if(character == 'X') {
                std::vector<std::string> horizontal_words = GetHorizontalWords(file_content, line_index, character_index);
                std::vector<std::string> vertical_words = GetVerticalWords(file_content, line_index, character_index);
                std::vector<std::string> diagonal_words = GetDiagonalWords(file_content, line_index, character_index);

                std::vector<std::string> combined;
                combined.insert(combined.end(), horizontal_words.begin(), horizontal_words.end());
                combined.insert(combined.end(), vertical_words.begin(), vertical_words.end());
                combined.insert(combined.end(), diagonal_words.begin(), diagonal_words.end());

                result += std::count_if(combined.begin(), combined.end(), [](const auto& item){ return item == xmas;});
            }
        }
    }

    std::cout << result << "\n";
}

std::vector<std::string> GetCrossWords(const std::vector<std::string>& file_content, const size_t line_index, const size_t character_index) {
    std::vector<std::string> words;
    
    std::string word_one; 
    auto row = character_index - 1;
    for(size_t index = line_index-1; index != line_index+2; ++index) {
        word_one += file_content[index][row++];
    }
    
    words.push_back(word_one);

    std::string word; 
    auto row_number = character_index + 1;
    for(size_t index = line_index-1; index != line_index+2; ++index) {
        word += file_content[index][row_number--];
    }

    words.push_back(word);
    return words;
}

bool IsXMax(const std::vector<std::string>& file_content, const size_t line_index, const size_t character_index) {
    if(line_index > 0 && line_index < file_content.size()-1) {
        if(character_index > 0 && character_index < file_content[line_index].size()-1) {
            
            const auto words = GetCrossWords(file_content, line_index, character_index);
            return std::all_of(words.begin(), words.end(), [](const std::string& word){ std::cout << "Word: " << word << "\n"; return word == "MAS" || word == "SAM"; });
        }
    }
    return false;
}

void SecondPart() {
    auto input_file = Utils::FileIo("input.txt");
    const auto file_content = input_file.GetFileContent();

    int result = 0;

    for(size_t line_index = 0u; line_index != file_content.size(); ++line_index) {
        const auto& line = file_content[line_index];
        for(size_t character_index = 0u; character_index != line.size(); ++character_index) {
            const auto& character = line[character_index];
            if(character == 'A') {
                if(IsXMax(file_content, line_index, character_index)) {
                    ++result;
                }
            }
        }
    }

    std::cout << result << "\n";
}

int main()
{
    std::cout << "Advent of code main\n";

    // FirstPart();
    SecondPart();

    return 0;
} 