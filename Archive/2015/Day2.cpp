#include <algorithm>
#include <array>
#include <cstdint>
#include <iterator>
#include <numeric>
#include <iostream>
#include <string>
#include <vector>

#include "FileIO.hpp"

constexpr std::uint8_t NumberOfDimenstions = 3u;
constexpr std::uint8_t numberOfDelimiters = 2u;

struct Package
{
    Package(const int l, const int w, const int h)
    : length(l)
    , width(w)
    , height(h)
    {
        std::vector<int> packageSides { (length * width), (length * width), (width * height), (width * height), (height * length), (height * length) };
        const auto smallestSideDimension = *(std::min_element(packageSides.begin(), packageSides.end()));
        requiredWrapperSize = std::accumulate(packageSides.begin(), packageSides.end(), smallestSideDimension);
    }

    int GetRequiredRibbonLength() const
    {
        const int smallestPossibleCircumference = GetSmallestPossibleCircumference();
        const int bowRibbonLength = length * width * height;

        return smallestPossibleCircumference + bowRibbonLength;
    }

    int GetSmallestPossibleCircumference() const
    {
        std::vector<int> dimensions {length, width, height};
        const auto firstSmallestDimensionIterator = std::min_element(dimensions.begin(), dimensions.end());
        const auto smallestDimension = *firstSmallestDimensionIterator;
        dimensions.erase(firstSmallestDimensionIterator);
        const auto secondSmallestDimensionIterator = std::min_element(dimensions.begin(), dimensions.end());
        const auto secondSmallestDimenstion = *secondSmallestDimensionIterator;
        dimensions.erase(secondSmallestDimensionIterator);

        const int smallestPossibleCircumference = (smallestDimension * 2) + (secondSmallestDimenstion * 2);

        return smallestPossibleCircumference;
    }

    const int length = 0u;
    const int width = 0u;
    const int height = 0u;
    int requiredWrapperSize = 0;
};

std::array<int, NumberOfDimenstions> GetPackageDimensionsFromString(const std::string& input)
{
    std::array<int, NumberOfDimenstions> packageDimensions {};

    std::string inputCopy = input;
    for(std::size_t index = 0; index != 3; ++index)
    {
        const auto position = inputCopy.find('x');
        const auto substr = inputCopy.substr(0, position);
        packageDimensions[index] = std::stoi(substr);
        inputCopy.erase(0, position+1);
    }

    return packageDimensions;
}

std::vector<Package> GetAllPackageDimensionsFromFile(const std::string fileName)
{
    Utils::FileIo fileIo("../day2-input.txt");
    std::vector<Package> packagDimensions;

    const std::vector<std::string> fileContent = fileIo.GetFileContent();
    for(const auto& line : fileContent)
    {
        const auto packageDimensions = GetPackageDimensionsFromString(line);
        packagDimensions.push_back(Package(packageDimensions[0], packageDimensions[1], packageDimensions[2]));
    }
    
    return packagDimensions;
}

void FirstPart()
{
    std::cout << "= First part =\n";
    const auto packages = GetAllPackageDimensionsFromFile("../day2-input.txt");
    const auto requiredSquareFeetOfWrappingPaper = std::accumulate(packages.begin(), packages.end(), 0, [](int accumulate, const Package& package){ return accumulate +  package.requiredWrapperSize; });

    std::cout << " \tRequired Square feet of wrapping paper: " << std::to_string(requiredSquareFeetOfWrappingPaper) << "\n";
}

void SecondPart()
{
    std::cout << "= Second part =\n";
    const auto packages = GetAllPackageDimensionsFromFile("../day2-input.txt");
    const auto requiredRibbonLength = std::accumulate(packages.begin(), packages.end(), 0, [](int accumulate, const Package& package)
    {   
        return accumulate + package.GetRequiredRibbonLength();
    });

    std::cout << " \tRequired length of ribbon: " << std::to_string(requiredRibbonLength) << "\n";
}

int main()
{
    std::cout << "Advent of code main\n";

    FirstPart();
    SecondPart(); 

    return 0;
}