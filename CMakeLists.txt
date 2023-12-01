cmake_minimum_required(VERSION 3.15)

project(AdventOfCode)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(sources
    src/Utils/FileIO.hpp
    src/main.cpp
)

configure_file(src/input.txt input.txt @ONLY)

add_executable(${PROJECT_NAME} ${sources})