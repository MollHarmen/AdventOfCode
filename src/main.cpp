#include "Utils/FileIO.hpp"
#include "Utils/StringHandling.hpp"

#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>
#include <tuple>
#include <vector>

std::tuple<int, int> GetStartPosition(const std::vector<std::string>& map) {
    for(size_t y = 0u; y != map.size(); ++y) {
        const auto& line = map[y];
        const auto found = std::find(line.begin(), line.end(), '^');
        if(found != line.end()) {
            return std::make_tuple(y, std::distance(line.begin(), found));
        }
    }

    return {0, 0};
}

int GetSteps(const int x, const int y, const std::vector<std::string>& map) {
    
}

void FirstPart() {
    auto input_file = Utils::FileIo("input.txt");
    const auto file_content = input_file.GetFileContent();

    auto [x, y] = GetStartPosition(file_content);
    int number_of_steps = GetSteps(x, y, file_content);


    std::cout << x << ", " << y << "\n";
}

void SecondPart() {
}

int main()
{
    std::cout << "Advent of code main\n";

    FirstPart();
    // SecondPart();

    return 0;
} 