project(AdventOfCode)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(sources
    FileIO.hpp
    main.cpp
)

add_executable(${PROJECT_NAME} ${sources})